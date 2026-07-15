#!/usr/bin/env perl

# This script goes thru a hypermail html archive and outputs the file
# name and the message-id associated with each message to stdout

# usage hyp-msgid-scrapper /path/to/hypermail/archive

use strict;
use warnings;

sub usage_and_quit {
    print "This script goes thru a hypermail html archive and outputs the file\n"
	. "name and the message-id associated with each message to stdout\n\n";
    print "$0  /path/to/hypermail/archive\n\n";
    exit(0)
}

sub get_msgid {
    my $filename = shift;
    my $msgid;

    open (my $FILE, "<$filename") || die "Cannot open $filename: $!\n";
    while (<$FILE>) {
	chomp;
	if (/^<!-- id=".*" -->$/) {
	    $msgid = $_;
	    last;
	}
    }
    close ($FILE);

    $msgid =~ s/^<!-- id="//;
    $msgid =~ s/" -->$//;

    return $msgid;
}

my ($hm_archive_path) = @ARGV;

if (not defined $hm_archive_path) {
    usage_and_quit();
}

if ($hm_archive_path =~ /.*\/$/) {
    $hm_archive_path =~ s/\/$//;
}


opendir(my $ARCHIVE, $hm_archive_path) || die "Can't open directory $hm_archive_path: $!\n";
foreach my $cursor (sort readdir($ARCHIVE)) {
    # skip files not created by hypermail
    my $filename = "$hm_archive_path/$cursor";
    if ((-d $filename)
	|| (-l $filename)
	|| (-z $filename)
	|| ($filename !~ /\/?[0-9]+\.html$/)) {
	next;
    }

    # extract msgid from file
    my $msgid = get_msgid($filename);
    # replace @ substitution
    print "$cursor:$msgid\n";
}

closedir($ARCHIVE);
