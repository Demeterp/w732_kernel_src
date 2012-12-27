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
use bytes;
use File::Path;
use Compress::Zlib;

my $version = "MTK-Tools by Bruno Martins\nMT65xx unpack script (last update: 29-11-2012)\n";
my $usage = "unpack-MT65xx.pl <infile>\n  Unpacks boot, recovery or logo image\n\n";

print "$version";
die "\nUsage: $usage" unless $ARGV[0];

my $inputfile = $ARGV[0];

my $slurpvar = $/;
undef $/;
open (INPUTFILE, "$inputfile") or die "\nError: could not open the specified file: $inputfile\n";
my $input = <INPUTFILE>;
close INPUTFILE;
$/ = $slurpvar;

if ((substr($input, 0, 4) eq "\x88\x16\x88\x58") & (substr($input, 8, 4) eq "LOGO")) {
	# if the input file contains the logo signature, try to unpack it
	print "\nValid logo signature found...\n";
	unpack_logo($input);
} elsif (substr($input, 0, 7) eq "\x41\x4e\x44\x52\x4f\x49\x44") {
	# else, a valid Android signature is found, try to unpack boot or recovery image
	print "\nValid Android signature found...\n";
	unpack_boot($input);
} else {
	die "\nError: the input file does not appear to be a valid file.";
}

sub unpack_boot {
    my $bootimg = $_[0];
	my($bootMagic, $kernelSize, $kernelLoadAddr, $ram1Size, $ram1LoadAddr, $ram2Size, $ram2LoadAddr, $tagsAddr, $pageSize, $unused1, $unused2, $bootName, $cmdLine, $id) = unpack('a8 L L L L L L L L L L a16 a512 a8', $bootimg);

        print  "Kernel CMDLine = $cmdLine \n";
	my($kernel) = substr($bootimg, $pageSize, $kernelSize);

	open (KERNELFILE, ">$ARGV[0]-kernel.img");
	binmode(KERNELFILE);
	print KERNELFILE $kernel or die;
	close KERNELFILE;

	print "\nKernel written to $ARGV[0]-kernel.img\n";

	my($kernelAddr) = $pageSize;
	my($kernelSizeInPages) = int(($kernelSize + $pageSize - 1) / $pageSize);

	my($ram1Addr) = (1 + $kernelSizeInPages) * $pageSize;

	my($ram1) = substr($bootimg, $ram1Addr, $ram1Size);

	# chop ramdisk header
	$ram1 = substr($ram1, 512);

	if (substr($ram1, 0, 2) ne "\x1F\x8B") {
		die "\nError: the boot image does not appear to contain a valid gzip file";
	}

	open (RAMDISKFILE, ">$ARGV[0]-ramdisk.cpio.gz");
	binmode(RAMDISKFILE);
	print RAMDISKFILE $ram1 or die;
	close RAMDISKFILE;

	print "Ramdisk written to $ARGV[0]-ramdisk.cpio.gz\n";

	if (-e "$ARGV[0]-ramdisk") {
		rmtree "$ARGV[0]-ramdisk";
		print "Removed old ramdisk directory $ARGV[0]-ramdisk\n";
	}

	mkdir "$ARGV[0]-ramdisk" or die;
	chdir "$ARGV[0]-ramdisk" or die;
	die "\nError: cpio not found!\n" unless ( -e "/usr/bin/cpio") || ( -e "/usr/local/bin/cpio") || ( -e "/bin/cpio") ; 
	print "Ramdisk size: ";
	system ("gzip -d -c ../$ARGV[0]-ramdisk.cpio.gz | cpio -i");

	print "Extracted ramdisk contents to directory $ARGV[0]-ramdisk\n";
	print "\nSuccessfully unpacked kernel and ramdisk.\n";
}

sub unpack_logo {
    my $logobin = $_[0];
	# get logo header
	my $header = substr($logobin, 0, 512);
	my ($header_sig, $logo_length, $logo_sig) = unpack('a4 V A4', $header);

	# just one more check to make sure that the logo image is valid
	my $sizelogobin = -s $inputfile;
	if ($logo_length != $sizelogobin - 512) {
		die "Error: the logo image does not appear to be valid.";
	}

	# chop the header and extract logo information
	my $logo = substr($logobin, 512);
	my $num_blocks = unpack('V', $logo);

	my $i = 0;
	my (@raw_addr, @zlib_raw) = ();
	print "\nNumber of raw images found: $num_blocks\n";
	# get the starting address of each raw file
	do {
		$raw_addr[$i] = unpack('L', substr($logo, 8+$i*4, 4));
		$i++;
	} while $i < $num_blocks;

	$i = 0;
	my $num;
	# extract rgb565 raw files (uncompress zlib rfc1950)
	do {
		if ($i < $num_blocks-1) {
			$zlib_raw[$i] = substr($logo, $raw_addr[$i], $raw_addr[$i+1]-$raw_addr[$i]);
		} else {
			$zlib_raw[$i] = substr($logo, $raw_addr[$i]);
		}
		$num = sprintf ("%02d",$i);
		open (RAWFILE, ">$ARGV[0]-raw[$num].rgb565");
		binmode(RAWFILE);
		print RAWFILE uncompress($zlib_raw[$i]) or die;
		close RAWFILE;
		print "Raw image #$i written to $ARGV[0]-raw[$num].rgb565\n";

		$i++;
	} while $i < $num_blocks;

	print "\nSuccessfully extracted all raw images.\n";
}
