#!/usr/local/bin/perl

#
# Compare 2 directories with archives produced by Hypermail.
# Intended primarily to test changes to Hypermail.
# Written by Peter McCluskey (pcm@rahul.net).
#

$dir1 = $ARGV[0];
$dir2 = $ARGV[1];

opendir(FDDIR,$dir1) || die("cant open $dir1");
@files1 = sort readdir(FDDIR);

foreach $file (@files1)
{
    my $file1name = "$dir1/$file";
    my $file2name = "$dir2/$file";
    if(!(-e $file2name))
    {
	print "$file2name is missing\n";
	next;
    }
    if(-d $file1name)
    {
	print "$file2name is not a directory\n" if(!(-d $file2name));
	next;
    }
    open(FD, "diff $file1name $file2name |") || die("cannot diff $file1name $file2name");
    while($line = <FD>)
    {
	if($line =~ /\d+c\d+/)
	{
	    if(&non_date_diffs($diff1, $diff2))
	    {
		print "$file: $diffs";
	    }
	    $diffs = '';
	    $diff1 = '';
	    $diff2 = '';
	}
	$diffs .= $line;
	$diff1 .= $line if(substr($line,0,1) eq '<');
	$diff2 .= $line if(substr($line,0,1) eq '>');
    }
    if(&non_date_diffs($diff1, $diff2))
    {
	print "$file: $diffs";
    }
    $diffs = '';
    $diff1 = '';
    $diff2 = '';
}

sub non_date_diffs
{
    my ($diff1, $diff2) = @_;
    my $i;
    return 1 if(length($diff1) != length($diff2));
    for($i = 0; $i < length($diff2); ++$i)
    {
	my $c1 = substr($diff1, $i, 1);
	my $c2 = substr($diff2, $i, 1);
	next if($c1 eq $c2);
	next if(($c1 eq "<") && ($c2 eq ">"));
	next if(($c1 =~ /\d/) && ($c2 =~ /\d/));
	return 1;
    }
    return 0;
}
