#!/usr/bin/env python
"""Take one email and put it in the right archive using hypermail. v%(version)s


Example usage:

cat email | %(progname)s [OPTIONS] -L <listname>

OPTIONS:
    -h  --help          print this usage information
    -L  --listname=     (required) archive for this list
    -M  --month=        use this month instead of current, e.g. 4 for April
    -v  --verbose       report actions
    -Y  --year=         use year (e.g. 2001) instead of current (needs -M, too)

This is a Free Software reimplementation of the little programs 
living in hypermail/archive/.

    Copyright (C) 2002  Bernhard Reiter <bernhard@gnu.org>

    This is Free Software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
"""

__version__="0.2"
# initial 3.8.2002  Bernhard Reiter <bernhard@gnu.org>
#  5. 8.2002    Bernhard
#   added -M and -Y options; made yearstring and month local variables
#   improved error message when list unknown
#   added a proper copyright message

import getopt
import sys
import os
import time

months=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]


verbose=0

class listconfig:
    """Keep configuration options for one list."""
    def __init__(self,
                listname,archive,label,aboutlink="../",options="",
                mailboxdir="",hypermail="/usr/bin/hypermail"):
        self.listname=listname
        self.archive=archive

        if mailboxdir:
            self.mailboxdir=mailboxdir
        else:
            self.mailboxdir=os.path.join(self.archive,"mailbox")

        self.label=label
        self.aboutlink=aboutlink
        self.options=options
        self.hypermail=hypermail


# configuration section
lists={}

l=listconfig(   listname="hypermail-test",
                archive="../tests/mail-archive",
                label="Hypermail Test Archive",
                options="-X -L en",
                hypermail="../src/hypermail"
            )
lists[l.listname]=l

l=listconfig(   listname="testmailman",
                archive="/var/www/mail-archive",
                label="Testmailman Archive",
                aboutlink="../",
                options="-X -L de"
            )
lists[l.listname]=l

# end configuration section


def usage():
    sys.stderr.write("\n"+ __doc__ %
        {"progname":sys.argv[0], "version":__version__})

    sys.stderr.write("\nThe following mailinglist configurations are known:\n")
    listnames=lists.keys()
    listnames.sort()
    for l in listnames:
        sys.stderr.write(l+" ")
    sys.stderr.write("\n")

def openmailboxfile(list,yearstring,month):
    """Construct mailbox file, open and return it.

    To get the old original msg2archive.c behaviour you would
    have to replace yearstring with:   ("%.2d" % int(yearstring)-1900)
    """
    n=list.listname+"."+yearstring+("%.2d"%month)
    fn=os.path.join(list.mailboxdir,n)

    if verbose:
        sys.stderr.write("Will append message to [%s]...\n" % fn)

    f=open(fn,"a")
    return f


def openhypermailpipe(list,yearstring, month):
    """Construct hypermail command line and open pipe for it."""

    d=os.path.join(list.archive,yearstring,months[month-1])
    cmd=( "%s -u -i -d \"%s\" -l \"%s\" -b \"%s\" %s\n" %
        (list.hypermail,d,list.label,list.aboutlink,list.options))

    if verbose:
        sys.stderr.write("Will pipe message into [%s]...\n" % cmd)

    p=os.popen(cmd,"w")
    return p


def archivemail(listname,inputfile, yearstring, month):
    """Write one email to the mailbox and the hypermail pipe.

    The email is read from inputfile and the non header "From "s
    are quoted for the mboxfile, but not for the hypermail pipe.
    """

    mboxfile=openmailboxfile(listname,yearstring,month)
    pipe=openhypermailpipe(listname,yearstring,month)

    inheader=1

    try:
        line=inputfile.readline()
        while line:
            if line=="\n" or line == "\r\n":
                inheader=0

            if not inheader:
                if line[:5]=="From ":
                    mboxfile.write("> ")

            mboxfile.write(line)
            pipe.write(line)

            line=inputfile.readline()

        # seperate mails
        mboxfile.write("\n")
    finally:
        mboxfile.close()
        piperesult=pipe.close()

    if piperesult:
        sys.stderr.write("Pipe to hypermail failed. Popen() returned "+ 
                    repr(piperesult)+ "\n")
        sys.exit(3)

    if verbose:
        sys.stderr.write("done.\n")


def main():
    "Process arguments and call functions doing the real work."
    global verbose
    yearstring=""
    month=0


    try:
        opts, args = getopt.getopt(sys.argv[1:], "hL:M:vY:", 
            ["help", "listname=","month=","verbose", "year="])
    except getopt.GetoptError:
        # print help information and exit:
        usage()
        sys.exit(2)
    listname = None
    for o, a in opts:

        if o in ("-h", "--help"):
            usage()
            sys.exit()

        if o in ("-L", "--listname"):
            listname = a

        if o in ("-M","--month"):
            month=int(a)
            if month<1 or month>12:
                sys.stderr.write("Month [%s] out of range!\n"%a)
                sys.exit(1)

        if o in ("-v", "--verbose"):
            verbose = 1

        if o in ("-Y", "--year"):
            if int(a) >1950 and int(a) <3002:
                yearstring=a
            else:
                sys.stderr.write("Year [%s] out of sensible range!\n"%a)
                sys.exit(1)


    if yearstring and not month:
        sys.stderr.write("Whem specifying year you also must specify month!\n")
        usage()
        sys.exit(1)

    now=time.localtime(time.time())

    if not yearstring:
        yearstring=time.strftime("%Y",now)

    if not month:
        month=now[1] # note time() returns 1 .. 12 in this field


    if not listname or not lists.has_key(listname):
        sys.stderr.write("List [%s] not known!\n" % listname)
        usage()
        sys.exit(1)

    archivemail(lists[listname],sys.stdin,yearstring, month)


if __name__=="__main__":
            main()
