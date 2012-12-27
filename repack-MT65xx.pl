#!/usr/bin/perl

#
# script from Android-DLS WiKi
#
# changes by Bruno Martins:
#   - modified to work with MT6516 boot and recovery images (17-03-2011)
#   - included support for MT65x3 and eliminated the need of header files (16-10-2011)
#   - added cygwin mkbootimg binary and propper fix (17-05-2012)
#   - included support for MT65xx logo images (31-07-2012)
#   - fixed problem unpacking logo images containing more than nine packed rgb565 raw files (29-11-2012)
#

use strict;
use warnings;
use Cwd;
use Compress::Zlib;

my $dir = getcwd;

my $version = "MTK-Tools by Bruno Martins\nMT65xx repack script (last update: 29-11-2012)\n";
my $usage = "repack-MT65xx.pl COMMAND [...]\n\nCOMMANDs are:\n\n  -boot <kernel> <ramdisk-directory> <outfile>\n    Repacks boot image\n\n  -recovery <kernel> <ramdisk-directory> <outfile>\n    Repacks recovery image\n\n  -logo <logo-directory> <outfile>\n    Repacks logo image\n\n";

print "$version";
die "\nUsage: $usage" unless $ARGV[0] && $ARGV[1] && $ARGV[2];

if ( $ARGV[0] eq "-boot" ) {
	die "\nUsage: $usage" unless $ARGV[3] && !$ARGV[4];
	repack_boot("ROOTFS");
} elsif ( $ARGV[0] eq "-recovery" ) {
	die "\nUsage: $usage" unless $ARGV[3] && !$ARGV[4];
	repack_boot("RECOVERY");
} elsif ( $ARGV[0] eq "-logo" ) {
	die "\nUsage: $usage" unless !$ARGV[3];
	repack_logo("LOGO");
} else {
	die "\nUsage: $usage";
}

sub repack_boot {
	# initilization
	my $kernel = $ARGV[1];
	my $ramdiskdir = $ARGV[2];
	my $outfile = $ARGV[3];
	my $signature = $_[0];
	$ARGV[0] =~ s/-//;
	
	chdir $ramdiskdir or die "\n$ramdiskdir $!";

	print "\nRepacking $ARGV[0] image...\nRamdisk size: ";
	system ("find . | cpio -o -H newc | gzip > $dir/ramdisk-repack.cpio.gz");

	chdir $dir or die "\n$ramdiskdir $!";;

	my $slurpvar = $/;
	undef $/;
	open (RAMDISKFILE, "ramdisk-repack.cpio.gz") or die "Error: could not open ramdisk file: ramdisk-repack.cpio.gz\n";
	my $ramdisk = <RAMDISKFILE>;
	close RAMDISKFILE;
	$/ = $slurpvar;

	# generate the header according to the ramdisk size
	my $sizeramdisk = length($ramdisk);
	my $header = gen_header($signature, $sizeramdisk);

	# attach the header to ramdisk
	my $newramdisk = $header . $ramdisk;

	open (RAMDISKFILE, ">new-ramdisk-repack.cpio.gz");
	print RAMDISKFILE $newramdisk or die;
	close RAMDISKFILE;

	# create the output file
	if ( $^O eq "cygwin" ) {
		system ("./mkbootimg.exe --kernel $kernel --ramdisk new-ramdisk-repack.cpio.gz -o $outfile");
	} else {
		system ("../mkbootimg --kernel $kernel --ramdisk new-ramdisk-repack.cpio.gz -o $outfile");
	}

	# cleanup
	unlink("ramdisk-repack.cpio.gz") or die $!;
	system("rm new-ramdisk-repack.cpio.gz");

	print "\nRepacked $ARGV[0] image into $outfile\n";
}

sub repack_logo {
	# initilization
	my $input;
	my $logodir = $ARGV[1];
	my $outfile = $ARGV[2];
	my $signature = $_[0];
	$ARGV[0] =~ s/-//;
	
	chdir $logodir or die "\n$logodir $!";

	my (@raw_addr, @zlib_raw) = ();
	print "\nRepacking $ARGV[0] image...\n";

	my $i = 0;
	my $slurpvar = $/;
	undef $/;
	for my $inputfile ( glob "./*.rgb565" ) {
		open (INPUTFILE, "$inputfile") or die "Error: could not open raw image: $inputfile\n";
		$input = <INPUTFILE>;
		close INPUTFILE;

		# deflate all rgb565 raw files found (compress zlib rfc1950)
		$zlib_raw[$i] = compress($input,Z_BEST_COMPRESSION);

		$i++;
	}
	die "Error: could not find any .rgb565 file under the specified folder: $logodir\n" unless $i > 0;

	chdir $dir or die "\n$logodir $!";;
	
	$/ = $slurpvar;
	my $num_blocks = $i;
	print "Number of raw images found in the specified folder: $num_blocks\n";
	
	my $logo_length = (4 + 4 + $num_blocks * 4);
	# calculate the start address of each raw image and the new file size
	$i = 0;
	do {
		$raw_addr[$i] = $logo_length;
		$logo_length += length($zlib_raw[$i]);
		$i++;
	} while $i < $num_blocks;

	# generate logo header according to the logo size
	my $logo_header = gen_header($signature, $logo_length);
	
	my $logobin = pack('LL', $num_blocks, $logo_length);
	
	$i = 0;
	do {
		$logobin .= pack('L', $raw_addr[$i]);
		$i++;
	} while $i < $num_blocks;

	$i = 0;
	do {
		$logobin .= $zlib_raw[$i];
		$i++;
	} while $i < $num_blocks;

	$logobin = $logo_header . $logobin;

	# create the output file
	open (RAWFILE, ">$outfile");
	binmode(RAWFILE);
	print RAWFILE $logobin or die;
	close RAWFILE;

	print "\nRepacked $ARGV[0] image into $outfile\n";
}

sub gen_header {
	my $header_type = $_[0];
	my $length = $_[1];

	return pack('a4La32H*', "\x88\x16\x88\x58", $length, $header_type, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
}
