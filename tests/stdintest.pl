#!/usr/local/bin/perl

require POSIX;
use POSIX;

$sec = time();

print `rm -f testdir/*`;
$hypermail="../src/hypermail -i -1 -u -d testdir -l foo ";

for(1 .. 15) {

    @now=localtime($sec);
    $nowfrom = strftime("%a %b %d %H:%M:%S %Y", @now);
    $nowdate = strftime("%a, %d %b %Y %H:%M:%S +0100 (%Z)", @now);

    open(HMAIL, "|$hypermail") ||
        die "can't run it";

    print HMAIL "From anon0\@anon.net $nowfrom\n",
    "Date: $nowdate\n",
    "Message-ID: <Marcel-1.26-010719335$_-0b0iukG\@sherwood.demon.co.uk>\n",
    "Subject: What about a message number $_\n",
    "From: $_ Loser <anon0\@anon.net>\n",
    "To: $_ Anonymous <anon1@\anon.net>\n",
    "\n",
    "DUMMY MESSAGE BODY $_\n";

    close(HMAIL);

    $sec += 1;
}

