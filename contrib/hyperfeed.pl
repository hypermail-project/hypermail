#!/usr/bin/perl5 
#
# hyperfeed.pl
#
#   Feed messages from an mbox into hypermail one at a time so as to test the
#   incremental mode of hypermail.
#
#   -a <args>  call hypermail with these arguments instead of the default
#   -e <num>   stop feeding messages after the one with this index (1-based)
#   -f <path>  use this as the mbox to draw messages from
#   -h <path>  use this as hypermail instead of the default
#   -s <num>   start feeding messages with the one with this index (1-based)
#
# Notes:
#
#   You can use this to extract a single contiguous range of messages from an
#   mbox by using a command line such as
#
#     hyperfeed.pl -s 10 -e 20 -h /bin/cat -a '' -f mbox.all > mbox.some
#
#   That command line will open an mbox named mbox.all and run separate
#   invocations of cat on messages 10 through 20-- eleven messages in all,
#   skipping the first nine. By using /bin/cat instead of hypermail, the 
#   effect is to send the messages, still in mbox format, to the standard
#   output. Why you might care to do that is left as an excercise for the
#   reader.
#
# Author: Scott Rose, rose@cs.washington.edu. 19 Oct 1999.

# She configures me...

$default_args = '-uid .';
$default_mbox = 'mbox';
$default_prog = '/var/adm/sm.bin/hypermail';

# She configures me not!

use Getopt::Std;

getopts('f:h:a:s:e:v');
$| = 1;

if(defined $opt_a) {
    $args = $opt_a;
} else {
    $args = '-guid .';
}
if(defined $opt_f) {
    $file = $opt_f;
} else {
    $file = $default_mbox;
}
if(defined $opt_h) {
    $hypermail = $opt_h;
} else {
    $hypermail = $default_prog;
}

open MBOX, $file or die "can't read the mbox.";

$msg = 0;

# Loop on lines in the mbox.

while(<MBOX>) {
    if(/^From / && defined @lines) {
	$msg++;
	exit if(defined $opt_e && $msg > $opt_e);

	# encountered the start of a new message with the content of the
	# previous one in @lines; emit it

	&h unless(defined $opt_s && $msg < $opt_s);
	undef @lines;
    }
    push @lines, $_;
}

$msg++;
exit if(defined $opt_e && $msg > $opt_e);

if(defined @lines) {
    &h unless((defined $opt_s && $msg < $opt_s) ||
	      (defined $opt_e && $msg > $opt_e));
}

    END {
	close MBOX or die "can't close mbox";
	print "\n" if(defined $opt_v);
    }

sub h {
    open HYPERMAIL, "|$hypermail $args" or
	die "can't pipe to hypermail.";
    print HYPERMAIL @lines;
    close HYPERMAIL or die "can't close hypermail";
    print '.' if(defined $opt_v);
}
