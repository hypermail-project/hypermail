#!/usr/bin/env python
# -*- mode:Python; tab-width: 3 -*-
"""Corrects dates in a mailbox archives.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
"""

__author__ = "Grzegorz Adam Hankiewicz"
__date__ = "$Date$"
__version__ = "$Revision$"
__email__ = "gradha@users.sourceforge.net"
__credits__ = ""


import time
import re
import sys
import os
import string
from stat import *
try:
   import mailbox_reader
except ImportError, msg:
   print msg
   print "For this program to work you need the mailbox_reader module."
   print "You should be able to get it from one of these places:"
   print "\n\thttp://gradha.sdf-eu.org/program/mailbox_reader.en.html"
   print "\thttp://www.vex.net/parnassus/"
   print "\thttp://freshmeat.net/\n"
   sys.exit(1)


# Exception hierarchy
class Error(Exception): pass
class Parse_error(Error): pass
class Parse_time_error(Parse_error): pass
   

HUMAN_VERSION = "1.0.1"
DAY_STRIPPER = re.compile(r"\d+.*")
DATE_FINDER = re.compile((r"(?P<day>\d+)\s+(?P<month>\w\w\w)\s+"
   "(?P<year>\d\d\d\d+)\s+(?P<time>\d\d:\d\d:\d\d)\s+"
   "(?P<zone>\+\d+|-\d+)?"))
TIME_FORMAT = "%d %b %Y %H:%M:%S"
WEEK_TIME = 60 * 60 * 24 * 7
MONTH_TIME = WEEK_TIME * 4


def complete_time_string(time_string):
   """Returns string `xx:xx' into `xx:xx:00' if needed.

   While the rfc says loud and clear that times should be written
   with second precision, there are many mail programs which only
   have minute precission. This function takes one time string and
   verifies that it is made of three pieces separated by a colon.

   If the string is made of three parts, it is returned untouched. If
   it is made of two parts, zeros are used to fill the seconds
   field. In all other cases a Parse_time_error exception is raised.
   """
   chunks = string.split(time_string, ":")
   if len(chunks) == 2:
      return "%s:00" % time_string
   if len(chunks) != 3:
      raise Parse_time_error("Couldn't parse `%s'" % time_string)
   return time_string
   

def parse_date(date_string):
   """Returns the time in seconds of date_string.

   If date_string cannot be parsed, Parse_error will be
   raised. Currently this function ignores timezone, resulting in
   times with poor precision.
   """
   if not date_string:
      raise Parse_error("`%s' not a valid date string" % date_string)
   # strip unnecessary commas
   date_string = string.replace(date_string, ",", "")
   # strip unnecessary day name
   match = DAY_STRIPPER.search(date_string)
   if not match:
      raise Parse_error("Didn't find any date in `%s'" % date_string)
   date_string = date_string[match.start():]
   # split into daynumber, month, year, time and optionally timezone
   components = string.split(date_string)
   c = components[:5]
   try:
      fake_date = "%s %s %s %s" % (c[0], c[1], c[2], complete_time_string(c[3]))
   except IndexError:
      raise Parse_error("Not enough values (4) in `%s' " % date_string)
   try:
      date_time = time.mktime(time.strptime(fake_date, TIME_FORMAT))
   except (ValueError, OverflowError), msg:
      raise Parse_time_error("Unable to parse time `%s'\n%s" % (msg, fake_date))

   # todo, parse timezone?
   return date_time


def extract_header_dates(headers):
   """Given a list of headers returns a list of pairs with date info.

   The returned list of pairs is the time of the date in seconds
   since Epoch and the string representing this time in the
   appropriate format (see RFC2822). Note that the returned time
   in seconds is way off inaccurate, since I don't care about
   timezones or conversions from/to localtime. It doesn't matter,
   since comarisons are usually done with a margin of weeks.
   """
   results = []
   for line in headers:
      if line[:5] == "Date:":
         return results
      match = DATE_FINDER.search(line)
      if match:
         fake_date = "%s %s %s %s" % match.group("day", "month", "year", "time")
         try:   
            date_time = time.mktime(time.strptime(fake_date, TIME_FORMAT))
         except ValueError:
            # Oops, couldn't extract date? Stupid server...
            continue
         if match.group("zone"):
            fake_date = "%s %s" % (fake_date, match.group("zone"))
            time.localtime(date_time)
         # prepend day in string format
         fake_date = "%s, %s" % (time.strftime("%a",
            time.gmtime(date_time)), fake_date)
         results.append((date_time, fake_date))
   return results


def time_to_string(time_in_seconds):
   """Returns time_in_seconds as an RFC2822 date string."""
   return time.strftime("%a, %d %b %Y %H:%M:%S +0000",
      time.gmtime(time_in_seconds))

   
def get_acceptable_date(reference_time, time_range, headers):
   """Returns first date in the acceptable time frame.

   reference_time is the time time_range is added or substracted
   from to check if a date is in the acceptable time frame. headers
   is a list with the email headers of the message. If there are
   no dates found IndexError is raised. If none of the available
   dates is acceptable, returns the first one anyway.
   """
   candidates = extract_header_dates(headers)
   candidates.reverse()
   for time, date in candidates:
      if abs(reference_time - time) <= time_range:
         return time, date
   # return possibly closest match
   return candidates[0]


def replace_date_header(email, new_date, verbose):
   """Updates the current date header in email with new_date.

   The previous date (if any) is stored as X-DT header. The date
   is not really replaced, new_date is appended to the headers.
   verbose is a boolean flag indicating whether extra headers have
   to be generated.
   """
   for f in range(len(email.headers)):
      if string.find(email.headers[f], "Date:") == 0:
         if verbose:
            email.headers[f] = "X-DT:%s" % email.headers[f][5:]
            break
         else:
            email.headers[f] = "Date: %s\n" % new_date
            return
            
   email.headers.append("Date: %s\n" % new_date)
   email.header_lines = email.header_lines + 1

   
def usage_information(exit_code = 0, binary_name = "mailbox_date_trimmer.py"):
   """Prints usage information and terminates execution."""
   print """Usage: %s [-hv -i xxx -o xxx]

-h, --help
   Print this help screen.
-v, --version
   Print version number and exit.
-i xxx, --input xxx
   The mailbox you want to use as input. If you don't specify any
   mailbox file, the mailbox will be read through standard input.
-o xxx, --output xxx
   Where you want the filtered mailbox to be written. If you don't
   specify any output, the filtered mailbox will be dumped to
   standard output.
-V, --verbose
   Add to the generated mailbox extra headers which explain what
   changes were made. By default no extra headers are added.


Usage examples:
 %s --input my_mailbox > filtered_mailbox
 cat my_mailbox | %s --output filtered_mailbox
""" % (binary_name, binary_name, binary_name)
   sys.exit(exit_code)


def process_command_line(argv = None):
   """Extracts from argv the options and returns them in a tuple.

   This function is a command line wrapper against main_process,
   it returns a tuple which you can `apply' calling main_process. If
   something in the command line is missing, the program will exit
   with a hopefully helpfull message.

   args should be a list with the full command line. If it is None
   or empty, the arguments will be extracted from sys.argv. The
   correct format of the accepted command line is documented by
   usage_information.
   """
   import getopt
   if not argv:
      argv = sys.argv

   short = "hvi:o:V"
   long = ["help", "version", "input=", "output=", "verbose"]

   try:
      opts, args = getopt.getopt(argv[1:], short, long)
   except getopt.GetoptError, msg:
      print "Error processing command line: %s\n" % msg
      usage_information(2)

   input = None
   output = None
   verbose = 0

   for option, value in opts:
      if option in ("-h", "--help"):
         usage_information()
      elif option in ("-v", "--version"):
         print HUMAN_VERSION
         sys.exit(0)
      elif option in ("-i", "--input"):
         input = value
      elif option in ("-o", "--output"):
         output = value
      elif option in ("-V", "--verbose"):
         verbose = 1

   if output is None:
      output = sys.stdout

   if input is None:
      input = sys.stdin

   return (input, output, verbose)


def main_process(input, output, verbose):
   """Goes over the input filtering to the output.

   input can be a string with a filename or an already opened file
   with a mailbox. output has to be a file like object supporting
   the write method. verbose is a boolean flag indicating if extra
   headers are to be appended to the generated mail archive.
   """
   mbox = mailbox_reader.Mailbox(input)
   email = mbox.read_mail()
   if not email:
      return
   last_header = email.get_header("date")
   # special case for first email, get its header/time
   try:
      last_time = parse_date(last_header)
   except Parse_error, msg:
      try: # oops, see if other timestamps are acceptable
         last_time, last_header = get_acceptable_date(0,
            MONTH_TIME, email.headers)
      except IndexError:
         # I can't feel my legs
         sys.stderr.write("%s\nUnable to process first date! Please fix it.\n" % msg)
         email.close()
         mbox.close()
         return

   # loop over available emails
   while email:
      reason = ""
      current_header = email.get_header("date")
      try:
         current_time = parse_date(current_header)
      except Parse_error, e:
         current_time = 0
         reason = " %s" % e

      # did we detect a big time difference?
      if abs(last_time - current_time) > MONTH_TIME:
         try:
            current_time, current_header = get_acceptable_date(last_time,
               MONTH_TIME, email.headers)
            reason = "%s Got acceptable date" % reason
         except IndexError:
            reason = "%s No acceptable header date" % reason
            current_time = last_time + 1
            current_header = time_to_string(current_time)

         # did we really improve the date field?
         if abs(last_time - current_time) <= MONTH_TIME:
            replace_date_header(email, current_header, verbose)
            if verbose:
               email.headers.append("X-pi:%s - Previous mail %s\n" %
                  (reason, last_header))
               email.header_lines = email.header_lines + 1

      output.write(string.join(email.headers, ""))
      output.write("\n")
      output.write(string.join(email.body, ""))

      last_header = current_header
      last_time = current_time
      
      email.close()
      email = mbox.read_mail()
      
   mbox.close()


if __name__ == "__main__":
   """Entry point of the script."""
   input, output, verbose = process_command_line()
   if not getattr(output, "close", None):
      output = open(output, "wt")
   main_process(input, output, verbose)
   output.close()
