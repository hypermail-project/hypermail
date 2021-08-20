#!/usr/bin/perl

use strict;
use warnings;

# This script reads a CSS file in stdin and outputs in stdout the C
# code for the src/printcss.c file. Replace that file  with the
# output from this script.
#
# print_default_css_file() will generate the default CSS file for
# an hypermail mailing list archive

# Usage: css_to_c.pl <../docs/hypermail.css >../src/printcss.c
#

sub beginning {

    print << "__HERE__";
/* 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 2 
** of the License, or any later version. 
** 
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of 
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
** GNU (Library) General Public License for more details. 
** 
** You should have received a copy of the GNU (Library) General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA 
*/
			      
#include "hypermail.h"
#include "setup.h"

#include "proto.h"

void print_default_css_file(char *filename)
{
    FILE *fp;
  
    if ((fp = fopen(filename, "w")) == NULL) {
        snprintf(errmsg, sizeof(errmsg), "%s \\"%s\\".", lang[MSG_COULD_NOT_WRITE], filename);
        progerr(errmsg);
    }

__HERE__
}

sub end {
    print << "__HERE__";
     
    fclose(fp);
    
    if (chmod(filename, set_filemode) == -1) {
        snprintf(errmsg, sizeof(errmsg), "%s \\"%s\\": %o.", lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }
    
} /* end print_default_css_file */

__HERE__
}

sub convert_css {
    while (<>) {
	chomp;
	# escape the few chars that may interfere with fprintf
	# % == %%
	# " == \"
	# \ == \\
	s#%#%%#g;
	s#(["\\])#\\$1#g;
	print "    fprintf(fp, \"" . $_ . "\\n\");\n";
    }
}

# main
{
    beginning();
    convert_css();
    end();
}
