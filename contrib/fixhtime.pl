#!/usr/bin/perl

require 5.000;
require Exporter;
use Carp;

#
# This program is intended to modify the dates on files in a Hypermail
# archive to be the times when the messages were sent rather than when
# Hypermail last did something to them.
#
# The recommended usage is to cd to the archive directory and run:
# fixhtime.pl [0-9]???.html
#
# If it doesn't do anything, first check that you have write permission on
# the files.
#
# I've tested it under Linux. It should work on virtually any Unix system.
# It might work on other operating systems.
#
# Send bug fixes or enhancements to pcm@rahul.net.
# The latest version will probably be in the contrib/ directory of the latest
# Hypermail distribution (http://sourceforge.net/projects/hypermail/
# or www.hypermail.org).
#

# This program is free software; you can redistribute it and/or modify it
# under the same terms as Perl itself.
#
# Some code below is duplicated from Time::Local.pm. For some strange reason,
# having it here rather than calling the Time::Local module avoids an infinite
# loop on some systems (specifically, on a Linux box with Perl 5.005_03
# and a 2.0.25 kernel).
#

BEGIN {
    $SEC  = 1;
    $MIN  = 60 * $SEC;
    $HR   = 60 * $MIN;
    $DAY  = 24 * $HR;
    $epoch = (localtime(2*$DAY))[5];	# Allow for bugs near localtime == 0.

    $YearFix = ((gmtime(946684800))[5] == 100) ? 100 : 0;

    my $t = time;
    my @lt = localtime($t);
    my @gt = gmtime($t);

    $tzsec = ($gt[1] - $lt[1]) * $MIN + ($gt[2] - $lt[2]) * $HR;

    my($lday,$gday) = ($lt[7],$gt[7]);
    if($lt[5] > $gt[5]) {
	$tzsec -= $DAY;
    }
    elsif($gt[5] > $lt[5]) {
	$tzsec += $DAY;
    }
    else {
	$tzsec += ($gt[7] - $lt[7]) * $DAY;
    }

  $tzsec += $HR if($lt[8]);
}

sub timegm {
    $ym = pack(C2, @_[5,4]);
    $cheat = $cheat{$ym} || &cheat;
    return -1 if $cheat<0 and $^O ne 'VMS';
    $cheat + $_[0] * $SEC + $_[1] * $MIN + $_[2] * $HR + ($_[3]-1) * $DAY;
}

sub timelocal {
    $time = &timegm + $tzsec;
    return -1 if $cheat<0 and $^O ne 'VMS';
    @test = localtime($time);
    $time -= $HR if $test[2] != $_[2];
    $time;
}

sub cheat {
    $year = $_[5];
    $year -= 1900
    	if $year > 1900;
    $month = $_[4];
    croak "Month '$month' out of range 0..11"	if $month > 11 || $month < 0;
    croak "Day '$_[3]' out of range 1..31"	if $_[3] > 31 || $_[3] < 1;
    croak "Hour '$_[2]' out of range 0..23"	if $_[2] > 23 || $_[2] < 0;
    croak "Minute '$_[1]' out of range 0..59"	if $_[1] > 59 || $_[1] < 0;
    croak "Second '$_[0]' out of range 0..59"	if $_[0] > 59 || $_[0] < 0;
    $guess = $^T;
    @g = gmtime($guess);
    $year += $YearFix if $year < $epoch;
    $lastguess = "";
    while ($diff = $year - $g[5]) {
	$guess += $diff * (363 * $DAY);
	@g = gmtime($guess);
	if (($thisguess = "@g") eq $lastguess){
	    return -1; #date beyond this machine's integer limit
	}
	$lastguess = $thisguess;
    }
    while ($diff = $month - $g[4]) {
	$guess += $diff * (27 * $DAY);
	@g = gmtime($guess);
	if (($thisguess = "@g") eq $lastguess){
	    return -1; #date beyond this machine's integer limit
	}
	$lastguess = $thisguess;
    }
    @gfake = gmtime($guess-1); #still being sceptic
    if ("@gfake" eq $lastguess){
	return -1; #date beyond this machine's integer limit
    }
    $g[3]--;
    $guess -= $g[0] * $SEC + $g[1] * $MIN + $g[2] * $HR + $g[3] * $DAY;
    $cheat{$ym} = $guess;
}


$mon{"Jan"} = 0;
$mon{"Feb"} = 1;
$mon{"Mar"} = 2;
$mon{"Apr"} = 3;
$mon{"May"} = 4;
$mon{"Jun"} = 5;
$mon{"Jul"} = 6;
$mon{"Aug"} = 7;
$mon{"Sep"} = 8;
$mon{"Oct"} = 9;
$mon{"Nov"} = 10;
$mon{"Dec"} = 11;

foreach $file (@ARGV)
{
    open(FD,$file) || warn("can't open $file");
    while($line1 = <FD>)
    {
	if($line1 =~ /^\<\!\-\- isosent\=\"(\d\d\d\d)(\d\d)(\d\d)(\d\d)(\d\d)(\d\d)/)
	{
	    $year = $1;
	    $month = $2 - 1;
	    $day = $3;
	    $hour = $4;
	    $min = $5;
	    $sec = $6;
	    $date = timegm($sec,$min,$hour,$day,$month,$year);
	    utime $date,$date,$file;
	    print $line1;
	    last;
	}
	if($line1 =~ /(\d+)\s+(\S+)\s+(\d+)\s+(\d+):(\d+):(\d+)[\s\"]+(\S+)/)
	{
	    $day = $1;
	    $monthname = $2;
	    $year = $3;
	    $hour = $4;
	    $min = $5;
	    $sec = $6;
	    $tzone = $7;
	    $date = timegm($sec,$min,$hour,$day,$mon{$monthname},$year);
	    $tzone /= 100 if(abs($tzone) >= 100);
	    $date -= $tzone*60*60;
	    utime $date,$date,$file;
	    last;
	}
    }
}
