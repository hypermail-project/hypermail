#!/usr/bin/perl
#
# Author: Scott Rose <srose@direct.ca>
#
# $Id$
#
# Comparing two archives is complicated by the fact that each message is
# marked by the time at which it's archived. Attached is a script that
# standardizes that date, if weakly, useful for such comparisons.
# 
# NOTE:
#     - Must be in the html storage directory to use this.
#     - If your files are stored with a different suffix than .html
#       you will need to correct it below.
#     - If the date is really important to you, fix this script and
#       rebaseline it. 
#

for $file (glob '[0-9][0-9][0-9][0-9].html') {
    open F, "$file" or die;
    @lines = <F>;
    close F;
    open F, ">$file" or die;
    $did = 0;
    for (@lines) {
	if(m#^: <EM>\w{3} \w{3}\s+\d{1,2} \d{4} - \d{2}:\d{2}:\d{2} \w{3}</EM>$#) {
	    die 'oops' if($did);
	    print F ": <EM>Sat Oct 16 1999 - 06:00:00 PDT</EM>\n";
	    $did++;
	}
	else {
	    print F;
	}
    }
    die "Oops" unless($did);
    close F;
    print "$file\n"
}


