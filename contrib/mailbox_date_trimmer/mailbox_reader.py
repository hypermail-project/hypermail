# -*- mode:Python; tab-width: 3 -*-
"""Python module to read UNIX mailboxes sequentially.

The module provides two classes: Mailbox, a file-like object
which allows you to iterate through the contents of a mailbox,
and Email, an object which holds the individual emails returned by
Mailbox. Mailbox inherits from Mailbox_base, but usually you don't
need to even know about this class. Mailbox_base implements the
file like mailbox reading part of the class, and the Email object
creation. Use the Mailbox class in your code, which will always
maintain the same API for backwards compatibility.

This module has been written with simplicity in mind, and low memory
consumption. Unless you do something bad, I estimate maximum memory
consumption as twice the memory required by the largest email of
the opened mailbox. But I'm guessing, maybe it is lower, like one
time your biggest email.

Reasons why you would want to use this module:

* python 1.5.2 support.

* last time I checked I was unable to use the mailbox module to read
  a mailbox, write it, and have them byte to byte identical. Didn't
  bother to check if this has changed, though:

 http://groups.google.com/groups?threadm=f4c749a5.0201291538.2cd15616%40posting.google.com

* MUCH easier to use, open and read, you can't do much more.

* MUCH smaller. Easier to read the source and understand it.

Here is a COMPLETE example code which shows correct usage:

   import mailbox_reader
   mbox = mailbox_reader.Mailbox("whatever")
   try:
      try:
         email = mbox.read_mail()
      except mbox.Parse_error, msg:
         print msg
         return
         
      while email:
         do_stuff_with_email_object(email)
         email.close()
         email = mbox.read_mail()
   finally:
      mbox.close()

This module comes with a setup.py file for installation, so running
"python setup.py install" will copy the module to your system's
python library path. You need the Distutils package to do so
(usually Python 1.5.2 users). More info at:

   http://www.python.org/sigs/distutils-sig/.

This module is known to work under python versions 1.5.2 and 2.2.3.
There are other modules implementing mailbox reader, usually
with greater complexity than what I needed. Go to the Vault
of Parnassus (http://www.vex.net/parnassus/, also linked from
http://www.python.org/) and search for `mailbox'.

This module is announced at comp.lang.python and found at:

   http://gradha.sdf-eu.org/program/mailbox_reader.en.html
   http://www.vex.net/parnassus/
   http://freshmeat.net/
   http://www.linuxlinks.com/

License
=======

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
__email__ = "gradha@users.sourceforge.net"
__date__ = "$Date$"
__version__ = "$Revision$"
__credits__ = ""


import string
import re
import rfc822


# Exception hierarchy
class Error(Exception): pass
class Parse_error(Error): pass

Mailbox_reader_error = Error # DEPRECATED

HUMAN_VERSION = "1.0.4"

# A valid from line is "From return-path weekday month day time
# [timezone] year. This info was taken from http://www.mutt.org/.
_from_re = re.compile((r"From \s*[^\s]+\s+\w\w\w\s+\w\w\w\s+\d?\d\s+"
   r"\d?\d:\d\d(:\d\d)?(\s+[^\s]+)?\s+\d\d\d\d\s*$"))


class Mailbox_base:
   """File like object mailbox reader.

   Wrapper around a file like object which should provide the
   contents of a mailbox file. After object creation, you are
   supposed to call .read_mail() until it returns None or you are
   tired of it. The wrapper keeps a method reference to the input
   file like object used to create the wrapper, so you better call
   .close() once you are done with the mail reading to break the
   reference and ease garbage collection.
   
   There is no way to go back through a file like object. You
   have to open a new wrapper around one or keep yourself a copy
   of the messages in memory if you want to iterate back through
   the returned messages.
   """

   # Exceptions
   Error = Mailbox_reader_error = Error
   Parse_error = Parse_error
   
   def __init__(self, file_object):
      """Creates a Mailbox object around the given file like object.

      file_object has to be a file like object implementing the
      .readline() method. If the parameter doesn't support this
      interface, an AttributeError exception will be thrown during
      the constructor call.

      Mailbox files have to start with a `From...' like line to
      be valid.  There is no way to know this until you try to read
      the first email from the file_object with .read_mail().
      """
      self._line = None
      self._input_readline = file_object.readline

   def __del__(self):
      """Calls the close method, in case the user forgot."""
      self.close()

   def read_mail(self):
      """Returns the next mail from the mailbox as Email object.

      If there are no more mails, or .close() has been previously
      called, None is returned.

      If it is the first time you call this method on a recently
      opened mailbox, it can raise Parse_error if the first line is
      not a mailbox `From...' like line. This can happen only with
      the first call, posterior calls never raise this exception.
      """
      if self._input_readline is None:
         return None
         
      headers = []
      body = []
      reading_body = 0

      # initial mailbox reading
      if not self._line:
         self._line = self._input_readline()
         if not _from_re.match(self._line):
            self.close()
            raise Parse_error("First line in mailbox has to "
               "begin with `From...'")
         
      while self._line:
         if reading_body and _from_re.match(self._line):
            return Email(headers, body)
   
         if reading_body:
            body.append(self._line)
         elif not reading_body and not string.strip(self._line):
            """Matched the start of the body"""
            reading_body = 1
         else:
            headers.append(self._line)

         self._line = self._input_readline()

      # Ok, we read last line
      self.close()

      if len(headers) and len(body):
         return Email(headers, body)
      else:
         return None

   def close(self):
      """Looses the method reference to the input file object.

      After this call, the .read_mail() method will be unable to
      return more messages. Usually you don't need to call this
      method manually, it's quite unlikely that the .readline()
      method reference creates a cyclic reference. Call it if you
      can't sleep well.
      """
      self._input_readline = None
         
      
class Mailbox(Mailbox_base):
   """Simple mailbox reader.

   This class is an interface wrapper around Mailbox_base, which
   provides the real methods to read mail messages.
   """

   def __init__(self, filename_or_file_object):
      """Creates a wrapper around the given parameter.

      The input parameter can be a filename or a file like object
      supporting the .readline() method. First the .readline() method
      will be tried, and if it doesn't exist, filename_or_file_object
      will be used as filename to open a file.
      """
      try:
         self._file = None
         Mailbox_base.__init__(self, filename_or_file_object)
      except AttributeError:
         self._file = open(filename_or_file_object, "rt")
         Mailbox_base.__init__(self, self._file)

   def read_mail(self):
      """Returns the next mail from the mailbox as Email object.

      The only difference of this method with regards to
      Mailbox_base.read_mail() is that if Parse_error is raised,
      .close() is automatically called before reraising again the
      exception. The rationale for this is that you don't need to
      keep opened mailboxes which you can't read or extract messages
      from them, and you don't have to remember calling the .close()
      method in your except block.

      Read the documentation of Mailbox_base.read_mail() for details.
      """
      try:
         return Mailbox_base.read_mail(self)
      except Parse_error:
         self.close()
         raise

   def close(self):
      """Closes the opened mailbox forever.

      You don't need to call this method if you wrapped a file
      like object, because in this situation only a reference to
      its .readline() method is held, and that is quite unlikely
      to create a cyclic reference which can't be cleaned up by
      the garbage collector.

      If you opened the Mailbox with a filename, however, you have
      to call this method to close the opened file and release
      it for good. Otherwise you could consume all your OS' file
      resources maintaining opened mailboxes around.
      """
      Mailbox_base.close(self)
      if self._file:
         self._file.close()
         self._file = None


class Email:
   """Holds the headers and body of an individual mail.

   You can read the body lines of an email with the readline()
   method.  You can retrieve the headers and body from the
   Email object just by accessing the same attributes which hold
   them. There are two more attributes you might want to use: the
   size attribute counts the size of the email in bytes. The rfc822
   attribute is a Message instance of Python's rfc822 module. You
   could access it to get specific mail headers, though there's
   the get_header method to abbreviate.

   Note that once you are done with an email, deleting it or keeping
   zero references to it will cause a memory leak unless you call
   its close() method.  This is because the self.rfc822 object
   holds references to the Email, which turn to be cyclic.
   """
   
   def __init__(self, headers, body):
      """Construct an email holder, passing headers and body as lists.

      You should not try to build Email objects yourself, only get
      built ones from the Mailbox class.
      """
      self._reading_line = 0
      self.headers = headers
      self.body = body
      self.size = 1 # Line separating body from headers not included later
      self.header_lines = len(self.headers)
      for f in range(len(self.headers)): self.size = self.size + len(self.headers[f]) + 1
      for f in range(len(self.body)): self.size = self.size + len(self.body[f]) + 1

      # Create method to retrieve generic headers
      self.rfc822 = rfc822.Message(self)
      # Retrieve commond headers, some lowered case
      self.subject = string.strip(self.rfc822.get("subject", ""))
      self.ffrom = string.strip(self.rfc822.get("from", ""))
      self.to = string.strip(self.rfc822.get("to", ""))

   def readline(self):
      """Use this method to get lines from the message header/body.

      This method returns one line at a time from the
      header/body. When you reach the end of the body, None is
      returned in successive calls. You can return to the beginning
      of the message calling .readline_reset().

      Usually you will prefer to access directly the headers and body
      of the Email object through their respective attributes. This
      method exists for other functions accepting file like objects
      which implement the standard .readline() method.
      """
      if self._reading_line < self.header_lines:
         result = self.headers[self._reading_line]
      elif self._reading_line == self.header_lines:
         result = "\n"
      else:
         try:
            result = self.body[self._reading_line - self.header_lines - 1]
         except IndexError:
            return None

      self._reading_line = self._reading_line + 1
      return result

   def readline_reset(self):
      """Resets the .readline() method, starting from headers again.

      Call this if you reached the end of the mail and want to read
      it again with the .readline() method. Available since 1.0.2.
      """
      self._reading_line = 0

   def all_lines(self):
      r"""Returns the whole message as a list of lines.

      The returned list is composed of headers + "\n" + body.
      You can call this method as many times as you want, and
      it won't interfere with the .readline() method. Available
      since 1.0.2.
      """
      return self.headers + ["\n"] + self.body

   def close(self):
      """Call this to free a reference cycle, avoiding amemory leak.

      This frees the rfc822 object, which holds a reference to the
      header lines.
      """
      if self.rfc822:
         self.rfc822 = None

   def get_header(self, header_name):
      """Simple wrapper around the rfc822 object held by the email.

      self.get_header(...) is equal to self.rfc822.get(..., None)
      """
      return self.rfc822.get(header_name, None)

