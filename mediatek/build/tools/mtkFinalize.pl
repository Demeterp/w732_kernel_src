#!/usr/bin/perl
($#ARGV == 2 || $#ARGV == 1 || $#ARGV == 0) || &Usage;
($prj, $platform, $emmc_support) = @ARGV;

(exit 0) if ($prj eq "generic");
$logDir = "out/target/product/$prj";
#die "Can NOT find $logDir" if (!-d $logDir);
(exit 0) if (!-d $logDir);

#print "plat = $platform\n";
my @lnmatrix = ();
   @lnmatrix = (
  "mediatek/source/preloader/preloader_${prj}.bin",
  "bootable/bootloader/uboot/uboot_${prj}.bin",
  "bootable/bootloader/uboot/logo.bin",
  "kernel/kernel_${prj}.bin",
  );
 
if ($emmc_support eq "yes")
{
  push(@lnmatrix,"mediatek/source/misc/${platform}_Android_scatter_emmc.txt");
  push(@lnmatrix,"mediatek/source/misc/MBR");
  push(@lnmatrix,"mediatek/source/misc/EBR1");
  push(@lnmatrix,"mediatek/source/misc/EBR2");
  push(@lnmatrix,"mediatek/source/misc/EBR3");
  push(@lnmatrix,"mediatek/build/tools/ptgen/mobile_info.img");
  push(@lnmatrix,"mediatek/build/tools/ptgen/fat.img");
}
else{
  push(@lnmatrix,"mediatek/source/misc/${platform}_Android_scatter.txt");	
}

chdir($logDir);
$relDir = $logDir;
$relDir =~ s/[^\/]+/../g;

foreach $i (@lnmatrix) {
  $lnfile = "${relDir}/$i";
  $i =~ /([^\/]+)$/;
  $j = $1;
  if ($j =~ /kernel\.bin/) {
    $j = "kernel.bin";
  }
  system("rm $j") if (-e $j);
  if (!-e $lnfile) {
     print("$lnfile does NOT exist!\n");
     next;
  }
  if ($lnfile =~ /kernel\.bin/) {
    system("ln -s $lnfile kernel.bin");
  } else {
    system("ln -s $lnfile .");
  }
}

exit 0;

sub Usage {
  warn << "__END_OF_USAGE";
Usage: (\$prj, \$platform, \$emmc_support) = @ARGV 
__END_OF_USAGE
  exit 1;
}

