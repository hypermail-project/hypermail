#!/usr/bin/env python
""" Creates a html sniplet foor hypermail directories. v%(version)s

Takes one directory name as argument, writes will create snipplet in
current directory, only if the overviewfile has changed after the snipplet.

USAGE %(progname)s directory_with_path number

"number" speficies how many directory levels of the path should be left
in the snipplet. 1 mean one and you can have more, if you want up to a limit.

The snipplet will be left in one directory over the given one.


This module requires the xml.sax module to be installed.
Per default is also uses the validating parser of xmlproc, see code 
to disable that. Search for "xmlproc".

In version 0.1 some code insite of this module is disabled.
"""
__version__=0.2
# 29.11.1999 initial Bernhard Reiter <bernhard@ffii.org>
#  6.12.1999 Bernhard
#   added replacement for os.path.getmtime(), because debian's python package
#   is still 1.5.1

from xml.sax import saxexts, saxlib, saxutils
from xml.sax.drivers import drv_xmlproc_val # for the xmlproc parsers
import sys,urllib
import os

try:
    getmtime=os.path.getmtime
except AttributeError, x:
    import stat
    def getmtime(file):
        statinfo=os.stat(file)
        return statinfo[stat.ST_MTIME]
        
    
    

overviewfilename="archive_overview.haof" 

class Mail:
    """ Saves info about one mail message. """
    def __init__(self):
        subject=""
        fromname=""
        fromemail=""
        date="" 
        messageid="" 
        filename=""

class SimpleDocHandler(saxlib.DocumentHandler):

    def __init__(self):
        self.indices={}
        self.maillist=[]
        self.mode=0
        self.current_name=""
        self.current_mail=None
        self.in_indices=0

    def startElement(self, name, attrs):
        if name=="mail":
            self.current_mail=Mail()
        elif name=="indices":
            self.in_indices=1
        self.current_name=name
        
#        sys.stdout.write( name + "starts.." )
#        for attr in attrs:
#            print "A %s %s" %(attr,attrs[attr])
#            print "("+name

    def endElement(self, name):
        #print name + "ends."
        if name=="mail":
            self.maillist.append(self.current_mail)
        if name=="indices":
            self.in_indices=0


    def characters(self, ch, start,length):
        #print " found " + self.current_name + ": " + ch[start:start+length]
        if self.current_name=="subject":
            self.current_mail.subject=ch[start:start+length]
        elif self.current_name=="date":
            self.current_mail.date=ch[start:start+length]
##disable some of the next lines in v0.1 just for speed.
#        elif self.current_name=="fromname":
#            self.current_mail.fromname=ch[start:start+length]
#        elif self.current_name=="fromemail":
#            self.current_mail.fromemail=ch[start:start+length]
#        elif self.current_name=="message-id":
#            self.current_mail.messageid=ch[start:start+length]
#        elif self.current_name=="file":
#            self.current_mail.filename=ch[start:start+length]
        elif self.in_indices:
            self.indices[self.current_name]=ch[start:start+length]
                                                                             

    

def main(argv):

    # test the arguments
    path=os.path.normpath(argv[1])
    if len(argv)!=3 or not os.path.isdir(path) \
            or int(argv[2]) not in range(1,10):
        sys.stderr.write(__doc__ % \
            {"version": __version__, "progname": argv[0]} )
        sys.exit(1)

    filename=os.path.join(path,overviewfilename)

    # which snipplet file are we talking about
    dirname,enddirname=os.path.split(path)
    snippletfilename=os.path.join(dirname,enddirname+"_html.blk") 

    # do we have to work? 
    if not os.path.isfile(filename) or (os.path.isfile(snippletfilename) \
        and getmtime(snippletfilename) > getmtime(filename)):
        # nothing to be done
#        sys.stderr.write("Nothing to be done.")
        return 0


    # check what filename, we have to leave in the URL
    predir=enddirname
    for t in range(1,int(argv[2]),1):
        dirname,head=os.path.split(dirname)
        predir=os.path.join(head,predir)


    # parse the overview file

    #p=saxexts.make_parser()   # this is the normal parser coming with python
    p=drv_xmlproc_val.create_parser() # validating xmlproc parser

    dh=SimpleDocHandler()
    p.setErrorHandler(saxutils.ErrorPrinter())
    p.setDocumentHandler(dh)

    p.parse(filename)

#    sys.stderr.write("the indices: " + repr(dh.indices) + "\n")
#    sys.stderr.write("found %d mails\n" % len(dh.maillist))

    # write snipplet
    f=open(snippletfilename,"w")
    write=f.write

        
    def writetablefield(index,name,dh,write,predir):
        if dh.indices.has_key(index+"index"):
           write("    <td><a href=\"%s/%s\">%s</a></td>\n" \
                    %(predir,dh.indices[index+"index"],name))
        else: write("<td></td>\n")

    
    write("  <td>%s</td>\n" % (enddirname))
    writetablefield("thread","Threads",dh,write,predir)
    writetablefield("date","Dates",dh,write,predir)
    writetablefield("subject","Subjects",dh,write,predir)
    writetablefield("author","Authors",dh,write,predir)
    write("  <td>%d</td>\n" % len(dh.maillist))

    f.close()
                

if __name__=="__main__":
    main(sys.argv)       

