#!/usr/bin/perl
#
# Concept:
#
#  Convert a hypermail archive to an mbox, weakly.
#
# Author:
#
#  Scott Rose, rose@cs.[wisc,washington].edu.
#
#     With modifications by Kent Landfield (kent@landfield.com). 
#     Further modifications by Peter McCluskey (pcm@rahul.net).
#     If it's broke, it's my fault.
#
# Usage:
#
#   hypetombox.pl [-d <directory>] [-m <output_filename>]
#
# $Header$

require 5.000;
use Getopt::Std;
getopts('d:m:');

# This is a list of the fields in the comment header of each message.

@fields = ('received', 'sent', 'name', 'email', 'subject', 'id', 'inreplyto');

# Get a list of the message files.

$fpat= '[0-9][0-9][0-9][0-9].html';
$fpat = "$opt_d/$fpat" if(defined $opt_d);

@msgs = sort glob($fpat);

# Open the output file for write.

$mbox_name = $opt_m || 'mbox';

(open MBOX, ">$mbox_name") || die "can't open $mbox_name";

# Loop on the input files.

# count the messages in the file
$cntr = 0;
$boguscntr = 0;

foreach $msg (@msgs) {

    # Open the message file for read.

    $cntr += 1;

    (open M, $msg) || die "can't open $msg";

    # Loop on lines in the file

    $state = 'HeaderComments';

    my $line;

    while($line = <M>) {

	# This is a header comment; save the value in a variable with the
	# same name.

	if($state eq 'HeaderComments'
	   && ($line =~ /^<!-- (\w+)="([^\"]+)" -->$/)) {
            $key = $1;
            $value = $2;
            $value =~ s/&amp;/&/g;
            $value =~ s/&lt;/</g;
            $value =~ s/&gt;/>/g;
            $$key = $value;
	    ++$header_count;
        }
        elsif($header_count > 0) {
            if($state eq 'HeaderComments') {
                $state = 'LeadingGoo';
                if($received =~ /^(\w{3} \w{3} {1,2}\d{1,2} \d{2}:\d{2}:\d{2} \d{4})/) {
                    $date = $1;
                }
		elsif($received =~ /^(\d{1,2} \w{3} {1,2}\d{2,4} \d{2}:\d{2}:\d{2} \w{3})/)
		{
		    $date = $1;
		}
                else {
                    $date = 'Bogus date';
                    $boguscntr += 1;
		    warn("received $received, $line\n") if($boguscntr < 3);
                }
                print MBOX "From $email  $date\n";
                print MBOX "Date: $sent\n";
		print MBOX "Message-Id: <$id>\n";
		print MBOX "To: bogus\n";
		print MBOX "From: $email ($name)\n";
		print MBOX "Subject: $subject\n";
                if ($inreplyto) {
		    print MBOX "In-Reply-To: <$inreplyto>\n";
		}
		print MBOX "\n";
            }
            $state = 'Body' if($line =~ /^<!-- body="start" -->/);
            $state = 'TrailingGoo' if(($line =~ /^<!-- body="end" -->/)
				     || ($line =~ /^<P><!-- body="end" -->/));
            next if(($line =~ /^<!--/) || 
                    $state eq 'LeadingGoo' || $state eq 'TrailingGoo');

            # This is a body line.

	    next if($line =~ /^<br>$/i);
	    next if($line =~ m|^</EM><BR>$|i);
            $line =~ s/<br>$//; # lose the trailing <br>
            $line =~ s/<BR>$//; # lose the trailing <br>
            $line =~ s/<pre>$//; # lose the <pre>formatted tags
            $line =~ s/<PRE>$//; # lose the <PRE>formatted tags
            $line =~ s/<\/pre>$//; # lose the </pre>formatted tags
            $line =~ s/<\/PRE>$//; # lose the </PRE>formatted tags
            $line =~ s/<P>$//;  # lose the paragraph tags
	    $line =~ s/<p>$//;  # lose the paragraph tags
            $line =~ s%<a href=[^>]+>([^<]+)</a>%\1%g; # lose hyperlinks
            $line =~ s%<A HREF=[^>]+>([^<]+)</A>%\1%g; # lose hyperlinks
	    $line =~ s/&lt;/</g; # reverse map special characters
            $line =~ s/&gt;/>/g;
            $line =~ s/&amp;/&/g;
	    $line =~ s/&quot;/\"/g;
	    $line =~ s/&nbsp;/ /g;
            $line =~ s%^<i>(.+)</i>$%\1%;
            $line =~ s%^<EM>(.+)</EM>$%\1%;
            $line =~ s%<EM>%%;
            $line =~ s/^From />From/; # Don't let bogus message start lines in!
            while($line =~ /^\s*<P>/)
            {
	        $line =~ s/^\s*<P>/\n/;
	    }
            print MBOX $line;
        }
    } # end loop on message file lines
    print MBOX "\n";

    (close M) || die "can't close $msg";


# ... then, the body:

    print MBOX @body;

} # end loop on messages

(close MBOX) || die "can't close $mbox_name";

$good = $cntr-$boguscntr;
print "$cntr messages processed ($good good messages : $boguscntr bogus date messages)\n";

if ($boguscntr) {
    print "Bogus Date encountered, hand edit and search for \"Bogus Date\"\n";
}
