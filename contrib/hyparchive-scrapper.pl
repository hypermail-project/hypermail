#!/usr/bin/env perl

# hyparchive-scrapper
#
# This script goes thru a hypermail generated mailing list HTML
# archive and outputs a summary that can be used to compare archives
# that are generated from the same mailbox using different versions of
# hypermail.
#
# By default the script will output a line for each message in the archive
# showing the filename and its associated Message-ID.
#
# here are different CLI options you can use to change this output,
# e.g., for telling the script to also outputs all the attachments
# associated with a message and if you want the filenames to include
# the full path to the archive
#
# Use the -h to view usage and options
#
# Author: J. KAHAN
# 11/Sep/2026
#

use strict;
use warnings;

use Getopt::Long;

sub usage_and_quit {
    print <<'HERE';
This script goes thru a hypermail html archive and outputs information
that can be used to compare archives that are generated from the same
mailbox using different versions of hypermail.

Usage: hyparchive-scrapper [-apu] /path/to/hypermail/archive

Options:
   -a | --show-attachments

   outputs all MIME attachment names, content-type and file size that
   are associated with a message. This option is disabled by default.

   -c | --show-message-charset

   outputs the message's charset

   -p | --show-fullpath

   outputs the fullpath to each message file and their attachments (if
   -a is used).  This option is disabled by default.

   -u | --unescape-message-ids

   Reverts the escaping that hypermail systematically applies to Message-IDs. This
   may be useful if you want to automatize searching messages in your mailbox using
   the unescaped Message-ID.

HERE
exit(0);
}

# extracts a msgid and charset from a hypermail generated message file.
sub get_message_data {
    my $filename = shift;
    my $msgid;
    my $charset="";

    open (my $FILE, "<$filename") || die "Cannot open $filename: $!\n";
    while (<$FILE>) {
	chomp;
	if (m/^<meta (?:(?:(?:.*?; )charset=)|(?:charset="))(.*?)"/) {
	    $charset = $1;
	}
	elsif (m/^<!-- id=".*" -->$/) {
	    $msgid = $_;
	    last;
	}
    }
    close ($FILE);

    $msgid =~ s/^<!-- id="//;
    $msgid =~ s/" -->$//;

    return ($msgid, $charset);
}

# reverts the string escapes hypermail may add to a Message-ID
sub unescape_msgid {
    $_ = shift;
    s/_at_/@/g;
    s/&ndash;/-/g;

    return $_;
}

# scraps the attachments associated with a message file;
# for each attachment, prints attachment name, content-type info, and file size
sub scrap_attachments {
    my ($filename, $show_fullpath) = @_;

    # are there any attachments?
    my $att_dir = $filename =~ s#(\d+)\.html$#att-$1#r;
    if (-d $att_dir) {
	my %file_meta;
	my $meta_dir = "$att_dir/.meta";

	# parse the meta dir if it exists; if it does,
	# read and store the content-type for each file
	if (-d $meta_dir) {
	    opendir(my $META_DIR, $meta_dir) || die "Can't open directory $meta_dir: $!\n";
	    foreach my $cursor (sort readdir($META_DIR)) {

		next if -d $cursor;

		my $meta_file_content;
		open(my $META_FILE, "<", "$meta_dir/$cursor");
		$meta_file_content = <$META_FILE>;
		close($META_FILE);
		chomp $meta_file_content;
		$meta_file_content =~ s/Content-Type: //;
		#print "$meta_file_content\n";
		my $key = $cursor =~ s/\.meta$//r;
		$file_meta{"$key"} = $meta_file_content;
	    }
	    closedir($META_DIR);
	}

	opendir(my $ATT_DIR, $att_dir) || die "Can't open directory $att_dir: $!\n";
	foreach my $cursor (sort readdir($ATT_DIR)) {

	    # skip files not created by hypermail
	    next if -d "$att_dir/$cursor";

	    my $filename = "$att_dir/$cursor";

	    if ($show_fullpath) {
		print $filename;
	    } else {
		my $partial_filename = $filename =~ s#^(?:.*/att-)#att-#r;
		print $partial_filename;
	    }

	    if (exists $file_meta{$cursor}) {
		print ": $file_meta{$cursor}";
	    }

	    # print the attachment's file size in bytes
	    my $file_size = (stat($filename))[7];
	    print ": $file_size";
	    print "\n";
	}
	closedir($ATT_DIR);
    }
}

##
## main
##
{
    my $show_attachments = 0;
    my $show_msg_charset = 0;
    my $show_fullpath = 0;
    my $unescape_msgid = 0;

    GetOptions("show-attachments|a" => \$show_attachments,
	       "show-message-charset|c" => \$show_msg_charset,
	       "show-fullpath|p" => \$show_fullpath,
	       "unescape-message-id|u" => \$unescape_msgid,
	       "help|h" => sub { usage_and_quit(); } )
	or usage_and_quit();

    my ($hm_archive_path) = @ARGV;

    usage_and_quit() unless defined $hm_archive_path;

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

	# extract msgid, charset from file
	my ($msgid, $charset) = get_message_data($filename);

	# replace @ substitution
	if ($unescape_msgid) {
	    $msgid = unescape_msgid($msgid);
	}

	if ($show_fullpath) {
	    print $filename;
	}
	else {
	    print $cursor;
	}
	print ":$msgid";

	if ($show_msg_charset) {
	    print "; charset=\"$charset\"";
	}

	print "\n";

	if ($show_attachments) {
	    scrap_attachments ($filename, $show_fullpath);
	}

    }
    closedir($ARCHIVE);
}
