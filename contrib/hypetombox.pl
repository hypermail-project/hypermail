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
#     With modifications by Kent Landfield. 
#     Further modifications by Peter McCluskey (pcm@rahul.net).
#     If it's broke, it's my fault.
#     For the very latest version, check the hypermail cvs repository:
#     http://cvs.hypermail.org/cgi-bin/cvsweb.cgi/hypermail/contrib/#hypetombox.pl
#     Or get the version in a recent hypermail distribution at
#     http://sourceforge.net/projects/hypermail/ or http://www.hypermail-project.org.
#
#     A few more mods by Fyodor (fyodor@insecure.org) [9/24/00]:
#        * Added -H (additional header) option (useful for filtering messages)
#        * Extended the date formats accepted in "Received:" headers
#        * Added 'a' option to append to mailbox rather than clobbering it
#        * Updated usage info
#
# Usage:
#
#   hypetombox.pl [-a] [-H <header(s) to add to each msg] [-d <directory>] [-m <output_filename>] [-n <to_address>] [-S <html suffix>]
#   -a means append to mailbox instead of clobbering it
#   -H adds the header(s) you specify.  If more than one, put the three
#      characters %0A between them.
#   -R removes carriage returns (\r) from the end of lines; they sometimes get
#      added for reasons I haven't been able to explain.
#   -S defaults to 'html'.
#
#
# $Header$

require 5.000;
use Getopt::Std;
use MIME::Base64;
getopts('ad:H:m:n:RS:');

# This is a list of the fields in the comment header of each message.

@fields = ('received', 'sent', 'name', 'email', 'subject', 'id', 'inreplyto');

# Get a list of the message files.

$htmlsuffix = $opt_S || "html";

$fpat= "[0-9][0-9][0-9][0-9].$htmlsuffix";
$fpat = "$opt_d/$fpat" if(defined $opt_d);

@msgs = sort glob($fpat);

$to_address = $opt_n || 'bogus';

# Open the output file for write.

$mbox_name = $opt_m || 'mbox';

if ($opt_H) {
	$opt_H =~ s/%0A/\n/g;
        chomp($opt_H);
	$opt_H .= "\n";
}

$DIR_PREFIXER = "att-";		# should be same as DIR_PREFIXER in parse.h
$PATH_SEPARATOR = '/';

if ($opt_a) {
	$openflag = ">>";
} else {
	$openflag = ">";
}
(open MBOX, "$openflag$mbox_name") || die "can't open $mbox_name";

# Loop on the input files.

# count the messages in the file
$cntr = 0;
$boguscntr = 0;

foreach $msg (@msgs) {

    # Open the message file for read.

    # reset the header count for each message.
    # added by Erik Peterson 1/3/00.
    $header_count=0;

    $cntr += 1;

    $msgnum = int(substr($msg, length($opt_d) + ($opt_d ne ''), 99999));
    $boundary = "=---------HYPETOMBOXCREATEDBOUNDARY$msgnum";
    $attdir = sprintf("%s%s$DIR_PREFIXER%04d", $opt_d,
		      $opt_d ne '' ? $PATH_SEPARATOR : '', $msgnum);
    @attfiles = glob("$attdir$PATH_SEPARATOR*");
    undef %att_type;
    undef %att_desc;

    (open M, $msg) || die "can't open $msg";

    # Loop on lines in the file

    $state = 'HeaderComments';
    foreach $fld (@fields)
    {
	$$fld = '';
    }

    my $line;
    my $msg_text;

    while($line = <M>) {

	# This is a header comment; save the value in a variable with the
	# same name.
	$line =~ s|\r(\n)$|\n| if($opt_R);

	if($state eq 'HeaderComments'
	   && ($line =~ /^<!-- (\w+)="(.+)" -->(\s*)$/)) {
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
		elsif($received =~ /^(\w{3}), (\d{1,2}) (\w{3}) (\d{4}) (\d{2}:\d{2}:\d{2})/) {
		    $date = sprintf("%s %s %.2d %s %s", $1, $3, $2, $5, $4);
		}
                else {
                    $date = 'Bogus date';
                    $boguscntr += 1;
		    warn("received $received, $line\n") if($boguscntr < 3);
                }
		$email = &unspamify($email);
                print MBOX "From $email  $date\n";
                print MBOX "Date: $sent\n";
		$id = &unspamify($id);
		print MBOX "Message-Id: <$id>\n";
		print MBOX "To: $to_address\n";
		print MBOX "From: $email ($name)\n";
		print MBOX "Subject: $subject\n";
 		if ($opt_H) {
                   print MBOX $opt_H;
                 }
                if ($inreplyto) {
		    $inreplyto = &unspamify($inreplyto);
		    print MBOX "In-Reply-To: <$inreplyto>\n";
		}
		if (@attfiles)
		{
		    print MBOX "Mime-Version: 1.0\n";
		    print MBOX "Content-Type: multipart/mixed; boundary=\"$boundary\"\n";
		}
		print MBOX "\n";
		if (@attfiles)
		{
		    print MBOX "This is a multi-part message in MIME format.\n";
		    print MBOX "--$boundary\n";
		    print MBOX "Content-Type: text/plain; charset=us-ascii\n";
		    print MBOX "Content-Transfer-Encoding: 7bit\n";
		}
            }
	    if($line =~ m|^<!-- attachment=\"(\d+)\-([^\"]+)\" -->|)
	    {
		my $attfilename = $2;
		my $part_no = $1;
		if($msg_text =~ s|<ul>\n<li>(\S+) attachment: ([^\n]+)\n</ul>(\s*)$||s)
		{
		    $att_type{$attfilename} = $1;
		    $att_desc{$attfilename} = $2 if($2 ne 'stored');
		}
		$msg_text =~ s|<hr noshade>\n$||s;
		$msg_text .= &get_att($attdir, $part_no, $attfilename);
		next;
	    }
            $state = 'Body' if($line =~ /^<!-- body="start" -->/);
            $state = 'TrailingGoo' if(($line =~ /^<!-- body="end" -->/)
				     || ($line =~ /^<([pP])><!-- body="end" -->/));
            next if(($line =~ /^<!--/) || 
                    $state eq 'LeadingGoo' || $state eq 'TrailingGoo');

            # This is a body line.

	    next if($line =~ /^<br>\s*$/i);
	    next if($line =~ m|^</EM><BR>\s*$|i);
            $line =~ s/<br>$\s*//; # lose the trailing <br>
            $line =~ s/<BR>$\s*//; # lose the trailing <br>
            $line =~ s/<pre>$\s*//; # lose the <pre>formatted tags
            $line =~ s/<PRE>$\s*//; # lose the <PRE>formatted tags
            $line =~ s/<\/pre>$\s*//; # lose the </pre>formatted tags
            $line =~ s/<\/PRE>$\s*//; # lose the </PRE>formatted tags
            $line =~ s/<P>$\s*/\n/;  # lose the paragraph tags
	    $line =~ s/<p>$\s*/\n/;  # lose the paragraph tags
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
            $line =~ s%^<em>(.+)</em>$%\1%;
            $line =~ s%<em>%%;
	    $line =~ s/\<a name=\"(\d+)qlink(\d+)\"\>//g;
            $line =~ s/^From />From/; # Don't let bogus message start lines in!
            while($line =~ /^\s*<P>/)
            {
	        $line =~ s/^\s*<P>/\n/;
	    }
	    $msg_text .= $line;
        }
    } # end loop on message file lines
    $msg_text =~ s|<a href=\"(\d+).$htmlsuffix\#(\d+)qlink(\d+)\">([^<]*)\</a\>|$4|sg;
    print MBOX $msg_text;
    print MBOX "\n";
    print MBOX "--$boundary--\n\n" if(@attfiles);

    (close M) || die "can't close $msg";


# ... then, the body: (this doesn't seem to have any function - pcm 2000-10-03)

    print MBOX @body;

} # end loop on messages

(close MBOX) || die "can't close $mbox_name";

$good = $cntr-$boguscntr;
print "$cntr messages processed ($good good messages : $boguscntr bogus date messages)\n";

if ($boguscntr) {
    print "Bogus Date encountered, hand edit and search for \"Bogus Date\"\n";
}

exit(0);

sub get_att
{
    my ($attdir, $part_no, $file1) = @_;
    my $attfile = "$attdir$PATH_SEPARATOR$part_no-$file1";
    my $text;
    open(FD_A, $attfile) || die "can't open $attfile";
    my $type = $att_type{$file1} || 'application/octet-stream';
    my $desc;
    $desc = "Content-Description: $att_desc{$file1}\n" if($att_desc{$file1});

    my $s = "--$boundary\nContent-Type: $type; name=\"$file1\"\n"
	. "Content-Transfer-Encoding: base64\n$desc"
	    . "Content-Disposition: attachment; filename=\"$file1\"\n\n";

    while($line = <FD_A>)
    {
	$text .= $line;
    }
    return $s . encode_base64($text) . "\n\n";
}

sub unspamify
{
    my ($s) = @_;
    if(index($s, '@') == -1)
    {
	$s =~ s/_at_/\@/;
    }
    return $s;
}
