#!/usr/bin/env python
""" Runs over the File system and builds a .html from snipplets.  v%(version)s

USAGE %(progname)s directory [ indexfilename ]

The argument has to be a directory. The html will be left in the
directory under index.html unless a second argument is given.
The indexfilename is relative to the directory, "-" for stdout is recognised.

directory has to contain top_html.hdr and footer_html.hdr.
"""
__version__=0.3

#29.11.1999 initial Bernhard Reiter
# 6.12.1999 Bernhard
#   fix: table row not created, if no snipplet there
#   added: second argument
#25.12.1999 Bernhard
#   cleanup: extracted code into new functions add_header(), add_footer()
#20. 7.2002 Bernhard
#   finished the cleanup

import sys
import os
import re
import create_archive_snipplet

topdirre= re.compile (r"^[12]\d{3,3}$")

months=["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]

def add_header(outputfile,path):
    """ Read and dump header file use standard html header."""
    fname=os.path.join(path,"top_html.hdr")
    if os.path.isfile(fname):
        f=open(fname,"r")
        outputfile.writelines(f.readlines())
        f.close()
    else:
        outputfile.write("<html>\n<body>\n")
        

def add_footer(outputfile,path):
    """ Read and dumb footer file or write standard html footer. """
    fname=os.path.join(path,"footer_html.hdr")
    if os.path.isfile(fname):
        f=open(fname,"r")
        outputfile.writelines(f.readlines())
        f.close()
    else:
        outputfile.write("</body>\n</html>\n")


def main(argv):

    # check arguments
    if (not (len(argv)>=2 and len(argv)<=3)) or not os.path.isdir(argv[1]):
        sys.stderr.write(__doc__ % \
            {"version": __version__, "progname": argv[0]} )
        sys.exit(1)

    # set default outputfilename
    path=os.path.normpath(argv[1])
    if len(argv)==3:
        outputfilename=argv[2]
    else:
        outputfilename="index.html"


    # use stdout for "-" 
    if outputfilename=="-":
        outputfile=sys.stdout
    else:
        outputfilename=os.path.join(path,outputfilename)
        outputfile=open(outputfilename,"w")

    write=outputfile.write
    writelines=outputfile.writelines

    add_header(outputfile,path)

    # get the real work done
    dirlist=os.listdir(path)

    def iswantedDirin(x,path=path):
        """Check if is directory and looks like an archive name."""
        return  os.path.isdir(os.path.join(path,x)) and topdirre.match(x)


    directories=filter(iswantedDirin, dirlist)
    if not len(directories):
        sys.exit("Found no year directories to work on!\n")

    directories.sort()
    directories.reverse()

    for dir in directories:

        write("<h2>%s</h2>\n\n" % dir)

        write("<table border=1>\n")
        write("<tr>" + \
                    "<th>Month</th> " + \
                    "<th colspan=4 align=center>Index Files</a></th>"+\
                    "<th align=right>Mails</th>"+\
              "</tr>\n")

        subpath=os.path.join(path,dir)
        dirlist=os.listdir(subpath)

        months.reverse()
        for month in months:
            if month in dirlist and os.path.isdir(os.path.join(subpath,month)):

                monthdir = os.path.join(subpath,month)
                create_archive_snipplet.main((argv[0],monthdir,"2"))
                snippletfilename=os.path.join(subpath,month+"_html.blk")

                if os.path.isfile(snippletfilename):
                    write("<tr>\n")
                    f=open(snippletfilename,"r")
                    writelines(f.readlines())
                    write("</tr>\n")

        write("</table>\n")
        

    add_footer(outputfile,path)


    if outputfile != sys.stdout:
        outputfile.close()


if __name__ =="__main__":
    main(sys.argv)
