#!/usr/bin/perl

use strict;
use warnings;

# This script reads a CSS file in stdin and outputs in stdout a the C
# code for print.c:write_css_file(). Replace that function with the
# output from this script.
#
# write_css_file() will generate the default CSS file for an hypermail
# mailing list archive

# Usage: css_to_c.pl <css_file > write_css_func.c
#

sub beginning {

    print << "__HERE__";

void write_css_file (char *filename)
{
    FILE *fp;
  
    if ((fp = fopen(filename, "w")) == NULL) {
        snprintf(errmsg, sizeof(errmsg), "%s \\"%s\\".", lang[MSG_COULD_NOT_WRITE], filename);
        progerr(errmsg);
    }

    fprintf(fp, "/* default CSS for hypermail mailing list archives */\n\n");

__HERE__
}

sub end {
    print << "__HERE__";
     
    fclose(fp);
    
    if (chmod(filename, set_filemode) == -1) {
        snprintf(errmsg, sizeof(errmsg), "%s \\"%s\\": %o.", lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }
    
} /* end write_css_file */

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
