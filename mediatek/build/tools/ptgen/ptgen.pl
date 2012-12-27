#!/usr/local/bin/perl -w
use File::Basename;

# arrays save excel field values, now, we are using only 4 columns, Partition, Start, SizeKB, and DL.

my @PARTITION_FIELD ;
my @START_FIELD_Byte ;
my @START_FIELD_Byte_HEX;
my @SIZE_FIELD_KB ;
my @TYPE_FIELD;
my @DL_FIELD ;
my @PARTITION_IDX_FIELD ;
my @REGION_FIELD ;
my @RESERVED_FIELD;

# define for columns
my $COLUMN_PARTITION                = 1 ;
my $COLUMN_TYPE                     = $COLUMN_PARTITION + 1 ;
my $COLUMN_START                    = $COLUMN_TYPE + 1 ;
my $COLUMN_END                      = $COLUMN_START + 1 ;
my $COLUMN_SIZE                     = $COLUMN_END + 1 ;
my $COLUMN_SIZEKB                   = $COLUMN_SIZE + 1 ;
my $COLUMN_SIZE2                    = $COLUMN_SIZEKB + 1 ;
my $COLUMN_SIZE3                    = $COLUMN_SIZE2 + 1 ;
my $COLUMN_DL                       = $COLUMN_SIZE3 + 1 ;
my $COLUMN_PARTITION_IDX            = $COLUMN_DL + 1 ;
my $COLUMN_REGION		    		= $COLUMN_PARTITION_IDX + 1;
my $COLUMN_RESERVED					= $COLUMN_REGION + 1;

my $SECRO_SIZE;
my $USERDATA_SIZE;
my $SYSTEM_SIZE;
my $CACHE_SIZE;
# Okey, that are the arrays.
my $total_rows = 0 ; #total_rows in partition_table
my $User_Region_Size_KB; #emmc USER region start_address
my $MBR_Start_Address_KB;	#KB

my $Page_Size=2; #page_size of nand
my %preloader_alias; #alias for preloader c and h files modify
my %uboot_alias;
my %kernel_alias;

my $LOCAL_PATH;

BEGIN
{
  $LOCAL_PATH = dirname($0);
}

use lib "$LOCAL_PATH/../Spreadsheet";
use lib "$LOCAL_PATH/../";
require 'ParseExcel.pm';

my $DebugPrint    = 1; # 1 for debug; 0 for non-debug

#parse argv from alps/mediatek/build/makemtk.mk

if($ARGV[0]=~/MTK_PLATFORM=(.*)/){
	$PLATFORM = $1;
	$platform = lc($PLATFORM);
}

if($ARGV[1]=~/PROJECT=(.*)/){
	$PROJECT = $1;
}

if($ARGV[2]=~/MTK_LCA_SUPPORT=(.*)/){
	$LCA_PRJ = $1;
}

if($ARGV[3]=~/MTK_NAND_PAGE_SIZE=(.*)/){
	$PAGE_SIZE = $1;
}

if($ARGV[4]=~/MTK_EMMC_SUPPORT=(.*)/){
	$EMMC_SUPPORT= $1;
}

if($ARGV[5]=~/EMMC_CHIP=(.*)/){
	 $EMMC_CHIP= $1;
}

if($ARGV[6]=~/MTK_LDVT_SUPPORT=(.*)/){
	 $LDVT_SUPPORT= $1;
}

if($ARGV[7]=~/TARGET_BUILD_VARIANT=(.*)/){
	 $TARGET_BUILD_VARIANT= $1;
}

if($ARGV[8]=~/MTK_EMMC_OTP_SUPPORT=(.*)/){
	 $MTK_EMMC_OTP_SUPPORT= $1;
}

my $PART_TABLE_FILENAME         = "mediatek/build/tools/ptgen/partition_table.xls"; # excel file name
my $PARTITION_DEFINE_H_NAME     = "mediatek/custom/$PROJECT/common/partition_define.h"; # 
my $PARTITION_DEFINE_C_NAME	= "mediatek/platform/$platform/kernel/drivers/dum-char/partition_define.c";
my $EMMC_PART_SIZE_LOCATION	= "mediatek/config/$PROJECT/configs/EMMC_partition_size.mk" ; # store the partition size for ext4 buil
my $EMMC_COMPO			= "mediatek/config/$PROJECT/eMMC_Compo.pl" ;

#for autogen uboot preload and kernel partition struct
my $ProjectConfig		="mediatek/config/$PROJECT/ProjectConfig.mk";
my $UbootH	="mediatek/custom/$PROJECT/uboot/inc/mt65xx_partition.h";
my $UbootC	="mediatek/custom/$PROJECT/uboot/mt65xx_partition.c";
my $PreloaderH	="mediatek/custom/$PROJECT/preloader/inc/cust_part.h";
my $PreloaderC	="mediatek/custom/$PROJECT/preloader/cust_part.c";
my $KernelH 	="mediatek/custom/$PROJECT/kernel/core/src/partition.h";

my $SCAT_NAME_DIR   = "mediatek/source/misc/"; # 
mkdir($SCAT_NAME_DIR) if (!-d $SCAT_NAME_DIR);
my $SCAT_NAME;
if ($LCA_PRJ eq "yes")
{
    $SCAT_NAME = $SCAT_NAME_DIR . $PLATFORM ."_Android_scatter_LCA.txt" ;
}

if ($EMMC_SUPPORT eq "yes") 
{
     $SCAT_NAME = $SCAT_NAME_DIR . $PLATFORM ."_Android_scatter_emmc.txt" ;
}else{
     $SCAT_NAME = $SCAT_NAME_DIR . $PLATFORM ."_Android_scatter.txt" ;
}

#my $EMMC_XLS_FILENAME = 'mediatek/build/tools/ptgen/emmc_region.xls';

my $SHEET_NAME;
if ($PLATFORM eq "MT6516")
{
	if($LCA_PRJ eq "yes"){
		$SHEET_NAME = $PLATFORM ." LCA";
	}else{
		$SHEET_NAME = $PLATFORM;	
	}	
}

if($PLATFORM eq "MT6573"){
	if($EMMC_SUPPORT eq "yes"){
		$SHEET_NAME = $PLATFORM ." emmc" ;
	}else{
		$SHEET_NAME = $PLATFORM ." nand " . $PAGE_SIZE ;
	}
}

if($PLATFORM eq "MT6575"){
	if($EMMC_SUPPORT eq "yes"){
		$SHEET_NAME = $PLATFORM ." emmc" ;
		if($MTK_EMMC_OTP_SUPPORT eq "yes"){
			$SHEET_NAME = $SHEET_NAME ." otp" ;
		}
	}else{
		$SHEET_NAME = $PLATFORM ." nand " . $PAGE_SIZE ;
		if($PAGE_SIZE=~/(\d)K/){
			$Page_Size=$1;
		}else{
			$Page_Size=2;	
		}

		if($TARGET_BUILD_VARIANT eq "user"){
			$SHEET_NAME = $SHEET_NAME . " user";
		}else{
			$SHEET_NAME = $SHEET_NAME . " eng";	
		}
			
	}

	if($LDVT_SUPPORT eq "yes"){
		$SHEET_NAME = $PLATFORM . " ldvt";	
	}
	
}


print "*******************Arguments from /mediatek/build/makemtk.mk*********************\n" ;
foreach $arg (@ARGV){
	print "$arg\n";
}
print "SHEET_NAME=$SHEET_NAME\n";
print "SCAT_NAME=$SCAT_NAME\n" ;
if($EMMC_SUPPORT eq "yes"){
	if (-e $EMMC_COMPO)
	{
		`chmod 777 $EMMC_COMPO`;
		$MBR_Start_Address_KB = do "$EMMC_COMPO";
		
	}else{
		print "No $EMMC_COMPO\n";
		$MBR_Start_Address_KB=6*1024;	#6MB
	}

	print "[Ptgen]MBR_Start_Address_KB= $MBR_Start_Address_KB\n";
}
print "*******************Arguments from /mediatek/build/makemtk.mk*********************\n\n\n\n" ;

#****************************************************************************
# main thread
#****************************************************************************
# get already active Excel application or open new
$PartitonBook = Spreadsheet::ParseExcel->new()->Parse($PART_TABLE_FILENAME);

&InitAlians();

&ReadExcelFile () ;

&GenHeaderFile () ;

&GenScatFile () ;

if ($EMMC_SUPPORT eq "yes"){
	&GenMBRFile ();
	&GenPartSizeFile ();
}

&ModifyPreloaderCust_PartH();
&ModifyPreloaderCust_PartC();
&ModifyUbootMt65xx_partitionH();
&ModifyUbootMt65xx_partitionC();

unless ($EMMC_SUPPORT eq "yes"){
    print "2 gen kernel\n";
	&ModifyKernelPartitionC();
}

print "**********Ptgen Done********** ^_^\n" ;

print "\n\nPtgen modified or Generated files list:\n$SCAT_NAME\n$PARTITION_DEFINE_H_NAME\n$EMMC_PART_SIZE_LOCATION\n/out/MBR EBR1 EBR2 \n\n\n\n\n";

exit ;




#****************************************************************************
# subroutine:  InitAlians
# return:      
#****************************************************************************
sub InitAlians(){
	$preloader_alias{"SECCFG"}="SECURE";
	$preloader_alias{"SEC_RO"}="SECSTATIC";
	$preloader_alias{"ANDROID"}="ANDSYSIMG";
	$preloader_alias{"USRDATA"}="USER";

	$uboot_alias{"DSP_BL"}="DSP_DL";
	$uboot_alias{"SECCFG"}="SECURE";
	$uboot_alias{"SEC_RO"}="SECSTATIC";
	$uboot_alias{"EXPDB"}="APANIC";
	$uboot_alias{"ANDROID"}="ANDSYSIMG";
	$uboot_alias{"USRDATA"}="USER";

	$kernel_alias{"SECCFG"}="seccnfg";
	$kernel_alias{"BOOTIMG"}="boot";
	$kernel_alias{"SEC_RO"}="secstatic";
	$kernel_alias{"ANDROID"}="system";
	$kernel_alias{"USRDATA"}="userdata";
}

#****************************************************************************
# subroutine:  ReadExcelFile
# return:      
#****************************************************************************

sub ReadExcelFile()
{
    my $sheet = get_sheet($SHEET_NAME,$PartitonBook) ;

    unless ($sheet)
    {
		my $error_msg="Ptgen CAN NOT find sheet=$SHEET_NAME in $PART_TABLE_FILENAME\n";
		print $error_msg;
		die $error_msg;
    }
	my $row = 1 ;
    my $pt_name = &xls_cell_value($sheet, $row, $COLUMN_PARTITION,$SHEET_NAME);
	
	while($pt_name ne "END"){
		$PARTITION_FIELD[$row-1] = $pt_name;
		$SIZE_FIELD_KB[$row-1]    = &xls_cell_value($sheet, $row, $COLUMN_SIZEKB,$SHEET_NAME) ;
		$DL_FIELD[$row-1]        = &xls_cell_value($sheet, $row, $COLUMN_DL,$SHEET_NAME) ;
		$TYPE_FIELD[$row-1]		 = &xls_cell_value($sheet, $row, $COLUMN_TYPE,$SHEET_NAME) ;
		if ($EMMC_SUPPORT eq "yes")
		{
            $PARTITION_IDX_FIELD[$row-1] = &xls_cell_value($sheet, $row, $COLUMN_PARTITION_IDX,$SHEET_NAME) ;
			$REGION_FIELD[$row-1]        = &xls_cell_value($sheet, $row, $COLUMN_REGION,$SHEET_NAME) ;
			$RESERVED_FIELD[$row-1]		= &xls_cell_value($sheet, $row, $COLUMN_RESERVED,$SHEET_NAME) ;
		}
		$row++;
		$pt_name = &xls_cell_value($sheet, $row, $COLUMN_PARTITION,$SHEET_NAME);
	}
#init start_address of partition
	$START_FIELD_Byte[0] = 0;	
	for($row=1;$row < @PARTITION_FIELD;$row++){
		if($PARTITION_FIELD[$row] eq "MBR"){
			$START_FIELD_Byte[$row] = $MBR_Start_Address_KB*1024;
			$SIZE_FIELD_KB[$row-1] = ($START_FIELD_Byte[$row] - $START_FIELD_Byte[$row-1])/1024;
			next;
		}
		if($PARTITION_FIELD[$row] eq "BMTPOOL" || $PARTITION_FIELD[$row] eq "OTP"){
			$START_FIELD_Byte[$row] = &xls_cell_value($sheet, $row+1, $COLUMN_START,$SHEET_NAME);
			next; 
		}
		
		$START_FIELD_Byte[$row] = $START_FIELD_Byte[$row-1]+$SIZE_FIELD_KB[$row-1]*1024;
		
	}
#convert dec start_address to hex start_address
	$START_FIELD_Byte_HEX[0]=0;
	for($row=1;$row < @PARTITION_FIELD;$row++){
		if($PARTITION_FIELD[$row] eq "BMTPOOL" || $PARTITION_FIELD[$row] eq "OTP"){
			$START_FIELD_Byte_HEX[$row] = $START_FIELD_Byte[$row];
		}else{
			$START_FIELD_Byte_HEX[$row] = sprintf("%x",$START_FIELD_Byte[$row]);
		}
	}
	
	if($DebugPrint eq 1){
		for($row=0;$row < @PARTITION_FIELD;$row++){
			print "START=0x$START_FIELD_Byte_HEX[$row],		Partition=$PARTITION_FIELD[$row],		SIZE=$SIZE_FIELD_KB[$row],	DL_=$DL_FIELD[$row]" ;
			if ($EMMC_SUPPORT eq "yes"){
            	print ", 	Partition_Index=$PARTITION_IDX_FIELD[$row],	REGION =$REGION_FIELD[$row]";
        	} 
			print "\n";
		}

	}

    $total_rows = @PARTITION_FIELD ;
	
	if ($total_rows == 0)
    {
        die "error in excel file no data!\n" ;
    } 
    print "There are $total_rows Partition totally!.\n" ;
}
#****************************************************************************
# subroutine:  GenHeaderFile
# return:      
#****************************************************************************
sub GenHeaderFile ()
{
    my $iter = 0 ;
    my $temp ;
	my $t;

	if (-e $PARTITION_DEFINE_H_NAME)
	{
		`chmod 777 $PARTITION_DEFINE_H_NAME`;
	}
    open (PARTITION_DEFINE_H_NAME, ">$PARTITION_DEFINE_H_NAME") or &error_handler("Ptgen open PARTITION_DEFINE_H_NAME fail!\n", __FILE__, __LINE__);

#write header
    print PARTITION_DEFINE_H_NAME &copyright_file_header_for_c();
    print PARTITION_DEFINE_H_NAME "\n#ifndef __PARTITION_DEFINE_H__\n#define __PARTITION_DEFINE_H__\n\n" ;
    print PARTITION_DEFINE_H_NAME "\n\n\n#define KB  (1024)\n#define MB  (1024 * KB)\n#define GB  (1024 * MB)\n\n" ;

        
    for ($iter=0; $iter< $total_rows; $iter++)
    {
        if($PARTITION_FIELD[$iter] eq "BMTPOOL")
        {
			my $bmtpool=sprintf("%x",$SIZE_FIELD_KB[$iter]/64/$Page_Size);
			$temp = "#define PART_SIZE_$PARTITION_FIELD[$iter]\t\t\t(0x$bmtpool)\n" ;
    		print PARTITION_DEFINE_H_NAME $temp ;
        }else
        {
    		$temp = "#define PART_SIZE_$PARTITION_FIELD[$iter]\t\t\t($SIZE_FIELD_KB[$iter]*KB)\n" ;
			print PARTITION_DEFINE_H_NAME $temp ;
        }
        
    }
    
    print PARTITION_DEFINE_H_NAME "\n\n#define PART_NUM\t\t\t$total_rows\n\n";
	print PARTITION_DEFINE_H_NAME "#define MBR_START_ADDRESS_BYTE\t\t\t($MBR_Start_Address_KB*KB)\n\n";
	if($EMMC_SUPPORT eq "yes"){
		print PARTITION_DEFINE_H_NAME "#define WRITE_SIZE_Byte		512\n";
	}else{
		print PARTITION_DEFINE_H_NAME "#define WRITE_SIZE_Byte		($Page_Size*1024)\n";
	}
	my $ExcelStruct = <<"__TEMPLATE";
typedef enum  {
	EMMC = 1,
	NAND = 2,
} dev_type;

typedef enum {
	USER = 0,
	BOOT_1,
	BOOT_2,
	RPMB,
	GP_1,
	GP_2,
	GP_3,
	GP_4,
} Region;


struct excel_info{
	char * name;
	unsigned long long size;
	unsigned long long start_address;
	dev_type type ;
	unsigned int partition_idx;
	Region region;
};
__TEMPLATE

	print PARTITION_DEFINE_H_NAME $ExcelStruct;
	print PARTITION_DEFINE_H_NAME "extern struct excel_info PartInfo[PART_NUM];\n";
	print PARTITION_DEFINE_H_NAME "\n\n#endif\n" ; 
   	close PARTITION_DEFINE_H_NAME ;

	if (-e $PARTITION_DEFINE_C_NAME)
	{
		`chmod 777 $PARTITION_DEFINE_C_NAME`;
	}
 	open (PARTITION_DEFINE_C_NAME, ">$PARTITION_DEFINE_C_NAME") or &error_handler("Ptgen open PARTITION_DEFINE_C_NAME fail!\n", __FILE__, __LINE__);	
    	print PARTITION_DEFINE_C_NAME &copyright_file_header_for_c();
	print PARTITION_DEFINE_C_NAME "#include <linux/module.h>\n";
	print PARTITION_DEFINE_C_NAME "#include \"partition_define.h\"\n";
	print PARTITION_DEFINE_C_NAME "struct excel_info PartInfo[PART_NUM]={\n";
	
	for ($iter=0; $iter<$total_rows; $iter++)
    {
    	$t = lc($PARTITION_FIELD[$iter]);
		$temp = "\t\t\t{\"$t\",";
		$t = ($SIZE_FIELD_KB[$iter])*1024;
		$temp .= "$t,0x$START_FIELD_Byte_HEX[$iter]";
		
		if($EMMC_SUPPORT eq "yes"){
			$temp .= ", EMMC, $PARTITION_IDX_FIELD[$iter],$REGION_FIELD[$iter]";
		}else{
			$temp .= ", NAND";	
		}
		$temp .= "},\n";
		print PARTITION_DEFINE_C_NAME $temp;
	}
	print PARTITION_DEFINE_C_NAME " };\n"; 
	print PARTITION_DEFINE_C_NAME "EXPORT_SYMBOL(PartInfo);\n";
   	close PARTITION_DEFINE_C_NAME ;

}
#****************************************************************************
# subroutine:  GenScatFile
# return:      
#****************************************************************************
sub GenScatFile ()
{
    my $iter = 0 ;
	`chmod 777 $SCAT_NAME_DIR` if (-e $SCAT_NAME_DIR);
    open (SCAT_NAME, ">$SCAT_NAME") or &error_handler("Ptgen open $SCAT_NAME Fail!", __FILE__, __LINE__) ;

    for ($iter=0; $iter<$total_rows; $iter++)
    {   
		my $temp;
        if ($DL_FIELD[$iter] == 0)
        {
            $temp .= "__NODL_" ;
        }
		if($RESERVED_FIELD[$iter] == 1){
			$temp .= "RSV_";
		}
	$temp .= "$PARTITION_FIELD[$iter]" ;
	$temp .= " 0x$START_FIELD_Byte_HEX[$iter]\n{\n}\n";

        print SCAT_NAME $temp ;
    }

    print SCAT_NAME "\n\n" ;
    close SCAT_NAME ;
}

#****************************************************************************************
# subroutine:  GenMBRFile 
# return:
#****************************************************************************************

sub GenMBRFile {
	#my $eMMC_size_block = $User_Region_Size_KB*1024/512;
	my $iter = 0;
# MBR & EBR table init
#	
#	MBR
#			P1: extend partition, include SECRO & SYS
#			P2:	CACHE
#			P3: DATA
#			P4: VFAT
#	EBR1
#			P5: SECRO
#	EBR2
#			P6: SYS
#
	my $mbr_start;
	my $p1_start_block;
	my $p1_size_block;
	my $p2_start_block;
	my $p2_size_block;
	my $p3_start_block;
	my $p3_size_block;
	my $p4_start_block;
	my $p4_size_block;
	my $p5_start_block;
	my $p5_size_block;
	my $p6_start_block;
	my $p6_size_block;
	my $p7_start_block;
	my $p7_size_block;

my @BR = (
	["mediatek/source/misc/MBR", [	[0x5,	0x0,0x0],
						[0x83,0x0,0x0],
						[0x83,0x0,0x0],
						[0xb,0x0,0x0]]],
	["mediatek/source/misc/EBR1", [[0x83,0x0,0x0],
						[0x05,0x0,0x0]]],
	["mediatek/source/misc/EBR2", [[0x83,0x0,0x0],
						[0x05,0x0,0x0]]],
	["mediatek/source/misc/EBR3", [[0x83,0x0,0x0]]]
);

    #$sheet = get_sheet($SHEET_NAME,$PartitonBook) ;
# Fill MBR & EBR table -----------------------------------------------------
	for ($iter=0; $iter<@PARTITION_FIELD; $iter++) {	       
		if($PARTITION_FIELD[$iter] eq "MBR"){
			$mbr_start = $START_FIELD_Byte[$iter];
		}
		if($PARTITION_FIELD[$iter] eq "CACHE"){
			$p2_start_block = ($START_FIELD_Byte[$iter]-$mbr_start)/512;
			#sync with img size -1MB
			$p2_size_block =  ($SIZE_FIELD_KB[$iter]-1024)*1024/512; 
		}
		if($PARTITION_FIELD[$iter] eq "USRDATA"){
			$p3_start_block = ($START_FIELD_Byte[$iter]-$mbr_start)/512;
			$p3_size_block =  ($SIZE_FIELD_KB[$iter]-1024)*1024/512;
		}
		if($PARTITION_FIELD[$iter] eq "SEC_RO"){
			$p5_start_block = ($START_FIELD_Byte[$iter]-$mbr_start)/512;
			$p5_size_block =  ($SIZE_FIELD_KB[$iter]-1024)*1024/512;
		}
		if($PARTITION_FIELD[$iter] eq "MOBILE_INFO"){
			$p6_start_block = ($START_FIELD_Byte[$iter]-$mbr_start)/512;
			$p6_size_block =  ($SIZE_FIELD_KB[$iter]-1024)*1024/512;
		}
		if($PARTITION_FIELD[$iter] eq "ANDROID"){
			$p7_start_block = ($START_FIELD_Byte[$iter]-$mbr_start)/512;
			$p7_size_block =  ($SIZE_FIELD_KB[$iter]-1024)*1024/512;
		}
	}
	#MBR
	print "MBR start is $mbr_start\n";
	$BR[0][1][0][1] = $p1_start_block = 0x20;
	$BR[0][1][1][1] = $p2_start_block;
	$BR[0][1][2][1] = $p3_start_block;
	$BR[0][1][3][1] = $p4_start_block = $p3_start_block + $p3_size_block+2048;#0xCEF20;
	print "P1 start is $p1_start_block\n";
	print "P2 start is $p2_start_block\n";
	print "P3 start is $p3_start_block\n";
	print "P4 start is $p4_start_block\n";
	$BR[0][1][0][2] = $p1_size_block = $p2_start_block - $p1_start_block;
	$BR[0][1][1][2] = $p2_size_block;
	$BR[0][1][2][2] = $p3_size_block;
	#$BR[0][1][3][2] = $p4_size_block = $eMMC_size_block -$p4_start_block;
	$BR[0][1][3][2] = $p4_size_block =0xffffffff;
	print "P1 size is $p1_size_block\n";
	print "P2 size is $p2_size_block\n";
	print "P3 size is $p3_size_block\n";
	print "P4 size is $p4_size_block\n";

	print "P5 start is $p5_start_block\n";
	print "P5 size is $p5_size_block\n";
	print "P6 start is $p6_start_block\n";
	print "P6 size is $p6_size_block\n";
	print "P7 start is $p7_start_block\n";
	print "P7 size is $p7_size_block\n";
	#EBR1
	$BR[1][1][0][1] = $p5_start_block - $p1_start_block;
	$BR[1][1][0][2] = $p5_size_block;
	$BR[1][1][1][1] = $p6_start_block - 0x20 - 0x20;
	$BR[1][1][1][2] = $p6_size_block + 0x20;

	#EBR2
	$BR[2][1][0][1] = 0x20;
	$BR[2][1][0][2] = $p6_size_block;
	$BR[2][1][1][1] = $p7_start_block - 0x20 - 0x20;
	$BR[2][1][1][2] = $p7_size_block + 0x20;
	#EBR3
	$BR[3][1][0][1] = 0x20;
	$BR[3][1][0][2] = $p7_size_block;

# Generate MBR&EBR binary file -----------------------------------------------------
foreach my $sBR (@BR){
	print("Generate $sBR->[0] bin file\n");
	
	#create file
	open(FH,">$sBR->[0]")|| die "create $sBR->[0] file failed\n";
	print FH pack("C512",0x0);

	#seek to tabel
	seek(FH,446,0);

	foreach (@{$sBR->[1]}){
		#type
		seek(FH,4,1);
		print FH pack("C1",$_->[0]);
		#offset and length
		seek(FH,3,1);
		print FH pack("I1",$_->[1]);
		print FH pack("I1",$_->[2]);
	}
	
	#end label
	seek(FH,510,0);
	print FH pack("C2",0x55,0xAA);

	close(FH);
}

}

#****************************************************************************************
# subroutine:  GenPartSizeFile;
# return:      
#****************************************************************************************

sub GenPartSizeFile
{
	`chmod 777 $EMMC_PART_SIZE_LOCATION` if (-e $EMMC_PART_SIZE_LOCATION);
        open (EMMC_PART_SIZE_LOCATION, ">$EMMC_PART_SIZE_LOCATION") or &error_handler("CAN NOT open $EMMC_PART_SIZE_LOCATION", __FILE__, __LINE__) ;
	print EMMC_PART_SIZE_LOCATION "\#!/usr/local/bin/perl\n";
	print EMMC_PART_SIZE_LOCATION &copyright_file_header_for_shell();
	print EMMC_PART_SIZE_LOCATION "\nifeq (\$(MTK_EMMC_SUPPORT),yes)\n";
	
	my $temp;
	my $index=0;
	for($index=0;$index < $total_rows;$index++){
		if($PARTITION_FIELD[$index] eq "SEC_RO")
	     {
			#1MB for sec info check in with CR ALPS00073371 
		 	$temp = $SIZE_FIELD_KB[$index]/1024-1;
			print EMMC_PART_SIZE_LOCATION "BOARD_SECROIMAGE_PARTITION_SIZE:=$temp". "M\n" ;
	     }
	     if($PARTITION_FIELD[$index] eq "MOBILE_INFO")
	     {
		 	$temp=$SIZE_FIELD_KB[$index]/1024-1;
			print EMMC_PART_SIZE_LOCATION "BOARD_MOBILE_INFOIMAGE_PARTITION_SIZE:=$temp". "M\n" ;
	      }
	     if($PARTITION_FIELD[$index] eq "ANDROID")
	     {
		 	$temp=$SIZE_FIELD_KB[$index]/1024-1;
			print EMMC_PART_SIZE_LOCATION "BOARD_SYSTEMIMAGE_PARTITION_SIZE:=$temp". "M\n" ;
	      }
	     if($PARTITION_FIELD[$index] eq "CACHE")
	     {
		 	$temp=$SIZE_FIELD_KB[$index]/1024-1;
			print EMMC_PART_SIZE_LOCATION "BOARD_CACHEIMAGE_PARTITION_SIZE:=$temp". "M\n" ;
	      }
	     if($PARTITION_FIELD[$index] eq "USRDATA")
	     {		# Reserve 1 MB for "encrypt phone" function. It needs 16k for CRYPT_FOOTER.MBR include it.
			# 1MB for sec info check in with CR ALPS00073371
		 	$temp=$SIZE_FIELD_KB[$index]/1024-1-1;
			print EMMC_PART_SIZE_LOCATION "BOARD_USERDATAIMAGE_PARTITION_SIZE:=$temp". "M\n" ;
	      }
	}

 	print EMMC_PART_SIZE_LOCATION "endif \n" ;
    close EMMC_PART_SIZE_LOCATION ;
}

#****************************************************************************
# subroutine:  error_handler
# input:       $error_msg:     error message
#****************************************************************************
sub error_handler()
{
	   my ($error_msg, $file, $line_no) = @_;
	   my $final_error_msg = "Ptgen ERROR: $error_msg at $file line $line_no\n";
	   print $final_error_msg;
	   die $final_error_msg;
}

#****************************************************************************
# subroutine:  copyright_file_header_for_c
# return:      file header -- copyright
#****************************************************************************
sub copyright_file_header_for_c()
{
    my $template = <<"__TEMPLATE";
__TEMPLATE

   return $template;
}
#****************************************************************************
# subroutine:  copyright_file_header_for_shell
# return:      file header -- copyright
#****************************************************************************
sub copyright_file_header_for_shell()
{
    my $template = <<"__TEMPLATE";
 # Copyright Statement:
 #
 # This software/firmware and related documentation ("MediaTek Software") are
 # protected under relevant copyright laws. The information contained herein
 # is confidential and proprietary to MediaTek Inc. and/or its licensors.
 # Without the prior written permission of MediaTek inc. and/or its licensors,
 # any reproduction, modification, use or disclosure of MediaTek Software,
 # and information contained herein, in whole or in part, shall be strictly prohibited.
 #
 # MediaTek Inc. (C) 2010. All rights reserved.
 #
 # BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 # THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 # RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 # AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 # EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 # MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 # NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 # SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 # SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 # THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 # THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 # CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 # SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 # STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 # CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 # AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 # OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 # MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 #
 # The following software/firmware and/or related documentation ("MediaTek Software")
 # have been modified by MediaTek Inc. All revisions are subject to any receiver's
 # applicable license agreements with MediaTek Inc.
 #/
__TEMPLATE

   return $template;
}

#****************************************************************************************
# subroutine:  ModifyPreloaderCust_PartH
# return:		Modify Cust_Part.H in Preloader
# input:       no input
#****************************************************************************************
sub ModifyPreloaderCust_PartH {
	if (-e $PreloaderH)
	{`chmod 777 $PreloaderH`;}	
	open (SOURCE, "$PreloaderH") or &error_handler("Ptgen CAN NOT open $PreloaderH", __FILE__, __LINE__);
	
	my $index=0;
	my $iter=0;
	my $findpt=0;
	my $NeedAdd=0;	
	my @arry=<SOURCE>;
	my $partalians;
	my @AddPart;
	foreach $part (@PARTITION_FIELD){
		if($part eq "END" || $part eq "BMTPOOL" || $part eq "OTP"){
			next;
		}
		if($preloader_alias{$part}){
			$partalians = $preloader_alias{$part};
		}else{
			$partalians = $part;
		}
		$index=0;
		while($index < @arry){
			if($arry[$index]=~/#define.*PART_$partalians/){
				$foundpt=1;
				last;
			}
			$index++;
		}
		
		if($foundpt eq 0){
			$AddPart[$iter]=$part;
			$NeedAdd=1;
			$iter++;
		}
		$foundpt=0;
	}
	
	if($NeedAdd == 1){
		$PreloaderHBUF=$PreloaderH.".tmp";
		open (BUF,">$PreloaderHBUF");
		$index=0;
		while($index < @arry){
			print BUF $arry[$index];
			if($arry[$index]=~/#define.*CUST_PART_H/){
				my $i=0;
				print BUF "/*autogen by Ptgen(Kai.Zhu mtk81086)*/\n";
				while($i < @AddPart){
					print BUF "#define PART_$AddPart[$i]			\"$AddPart[$i]\" \n";
					print BUF "#define CFG_USE_$AddPart[$i]_PARTITION	\n";
					$i++;
				}
				print BUF "/*autogen by Ptgen(Kai.Zhu mtk81086)*/\n";
			}
		$index++;
		}
		print "$PreloaderH has been modified\n";
		close SOURCE;
		close BUF;
		rename($PreloaderHBUF,$PreloaderH) or &error_handler("CAN NOT rename $PreloaderHBUF\n",__FILE__, __LINE__);
	}else{
		close SOURCE;
	}
	
}

#****************************************************************************************
# subroutine:  ModifyPreloaderCust_PartC
# return:		Modify Cust_Part.C in Preloader
# input:       no input
#****************************************************************************************
sub ModifyPreloaderCust_PartC {
	if (-e $PreloaderC)
	{`chmod 777 $PreloaderC`;}	
	open (SOURCE, "$PreloaderC") or &error_handler("Ptgen CAN NOT open $PreloaderC", __FILE__, __LINE__);
	
	my $new=$PreloaderC.".tmp";
	open (NEW,">$new");

	my $index=0;
	my $start_replace=0;
	my $end_replace=0;
	my $br=0;
	my $matchBr=0;
	my $findbr=0;	
	my @arry=<SOURCE>;
	my $i;

	for($index=0;$index < @arry;$index++){
		#need replacement if match
		if($arry[$index]=~/void.*cust_part_init.*(.*void.*)/)
		{
			$start_replace = $index;
		}
		if($start_replace ne 0){
		#if number of { eq number of },stop the replacement
			my @leftbr;
			my @rightbr;
			@leftbr = $arry[$index] =~/\{/g ;
			$br += @leftbr;
			$matchBr +=@leftbr;
			@rightbr = $arry[$index] =~/\}/g ;
			$br += @rightbr;
			$matchBr -=@rightbr;
			if($br ne 0 && $matchBr eq 0){
				$end_replace = $index;
				last;	
			}
		}
	}
	
#write tmp file
	for($index=0;$index < @arry;$index++){
		if($index == $start_replace){
			print NEW "void cust_part_init(void)
{
	u32 index = 0;
    memset(platform_parts, 0, sizeof(part_t) * PART_MAX_COUNT);\n";
			for($i=0;$i < @PARTITION_FIELD;$i++){
				if($PARTITION_FIELD[$i] eq "BMTPOOL" || $PARTITION_FIELD[$i] eq "OTP"){
					next;
				}
				print NEW "	{\n";
				if($preloader_alias{$PARTITION_FIELD[$i]}){
					print NEW "		platform_parts[index].name = PART_$preloader_alias{$PARTITION_FIELD[$i]};\n"
				}else{
					print NEW "		platform_parts[index].name = PART_$PARTITION_FIELD[$i];\n";
				}
				
				print NEW "		platform_parts[index].size = PART_SIZE_$PARTITION_FIELD[$i];\n";
				print NEW "		platform_parts[index].flags = PART_FLAG_NONE;\n";
				
				if($i == 0){
					print NEW "		platform_parts[index].startblk = 0;\n";
				}
				
				print NEW "		index++;\n";
				print NEW "	}\n";
			}
			
			print NEW "
\#ifdef CFG_USE_DSP_ROM_PARTITION    
    {
        platform_parts[index].name = PART_DSP_ROM;
        platform_parts[index].size = 1 * MB;
        platform_parts[index].flags = PART_FLAG_NONE;
        index++;
    }
\#endif

\#ifdef CFG_USE_AP_ROM_PARTITION
    {
        platform_parts[index].name = PART_AP_ROM;
        platform_parts[index].size = 50 * MB;
        platform_parts[index].flags = PART_FLAG_NONE;
        index++;
    }
\#endif

\#ifdef CFG_USE_MD_ROM_PARTITION
    {
        platform_parts[index].name = PART_MD_ROM;
        platform_parts[index].size = 8 * MB;
        platform_parts[index].flags = PART_FLAG_NONE;
        index++;
    }
\#endif

    {
        platform_parts[index].name = NULL;
        platform_parts[index].size = 0;
        platform_parts[index].flags = PART_FLAG_END;
        index++;
    }\n";
		
			print NEW "}\n";
			
			$index=$end_replace;
		}else{
			print NEW $arry[$index];
		}
			
	}

	close SOURCE;
	close NEW;
	rename($new,$PreloaderC) or &error_handler("CAN NOT rename $new\n",__FILE__, __LINE__);
}
#****************************************************************************************
# subroutine:  ModifyUbootMt65xx_partitionH
# return:      
#****************************************************************************************
sub ModifyUbootMt65xx_partitionH()
{
	if (-e $UbootH)
	{`chmod 777 $UbootH`;}	
	open (SOURCE, "$UbootH") or &error_handler("Ptgen CAN NOT open $UbootH", __FILE__, __LINE__);
	
	my $index=0;
	my $iter=0;
	my $findpt=0;
	my $NeedAdd=0;	
	my @arry=<SOURCE>;
	my $partalians;
	my @AddPart;
	foreach $part (@PARTITION_FIELD){
		if($part eq "END" || $part eq "BMTPOOL" || $part eq "OTP"){
			next;
		}
		if($uboot_alias{$part}){
			$partalians = $uboot_alias{$part};
		}else{
			$partalians = $part;
		}
		$index=0;
		while($index < @arry){
			if($arry[$index]=~/#define.*PART_$partalians/){
				$foundpt=1;
				last;
			}
			$index++;
		}
		
		if($foundpt eq 0){
			print "$part not find\n";
			$AddPart[$iter]=$part;
			$NeedAdd=1;
			$iter++;
		}
		$foundpt=0;
	}
	
	if($NeedAdd == 1){
		$UbootHTmp=$UbootH.".tmp";
		open (BUF,">$UbootHTmp");
		$index=0;
		while($index < @arry){
			print BUF $arry[$index];
			if($arry[$index]=~/#define.*__MT65XX_PARTITION_H__/){
				my $i=0;
				print BUF "/*autogen by Ptgen(Kai.Zhu mtk81086)*/\n";
				while($i < @AddPart){
					print BUF "#define PART_$AddPart[$i]			\"$AddPart[$i]\"\n";
					print BUF "#define PART_BLKS_$AddPart[$i]		\"BLK_NUM(PART_SIZE_$AddPart[$i])\"\n";
					$i++;
				}
				print BUF "/*autogen by Ptgen(Kai.Zhu mtk81086)*/\n";
			}
		$index++;
		}
		print " $UbootH has been modified\n";
		close SOURCE;
		close BUF;
		rename($UbootHTmp,$UbootH) or &error_handler("CAN NOT rename $UbootHTmp\n",__FILE__, __LINE__);
	}else{
		close SOURCE;
	}
	
}
#****************************************************************************************
# subroutine:  ModifyUbootMt65xx_partitionC
# return:      
#****************************************************************************************
sub ModifyUbootMt65xx_partitionC(){
	if (-e $UbootC)
	{`chmod 777 $UbootC`;}	
	open (SOURCE, "$UbootC") or &error_handler("Ptgen CAN NOT open $UbootC", __FILE__, __LINE__);
	
	my $new=$UbootC.".tmp";
	open (NEW,">$new");

	my $index=0;
	my $start_replace=0;
	my $end_replace=0;
	my $br=0;
	my $matchBr=0;
	my $findbr=0;	
	my @arry=<SOURCE>;
	my $i;

	for($index=0;$index < @arry;$index++){
		#need replacement if match
		if($arry[$index]=~/part_t.*mt6575_parts\[.*\].*=/)
		{
			$start_replace = $index;
		}
		if($start_replace ne 0){
		#if number of { eq number of },stop the replacement
			my @leftbr;
			my @rightbr;
			@leftbr = $arry[$index] =~/\{/g ;
			$br += @leftbr;
			$matchBr +=@leftbr;
			@rightbr = $arry[$index] =~/\}/g ;
			$br += @rightbr;
			$matchBr -=@rightbr;
			if($br ne 0 && $matchBr eq 0){
				$end_replace = $index;
				last;	
			}
		}
	}
	

#write tmp file
	for($index=0;$index < @arry;$index++){
		if($index == $start_replace){
			print NEW "part_t mt6575_parts[] = {\n";
			for($i=0;$i < @PARTITION_FIELD;$i++){
				if($PARTITION_FIELD[$i] eq "BMTPOOL" || $PARTITION_FIELD[$i] eq "OTP"){
					next;
				}
				print NEW "	{\n";
				if($uboot_alias{$PARTITION_FIELD[$i]}){
					print NEW "		.name   = PART_$uboot_alias{$PARTITION_FIELD[$i]},\n"
				}else{
					print NEW "		.name   = PART_$PARTITION_FIELD[$i],\n";
				}
				print NEW "		.blknum = BLK_NUM(PART_SIZE_$PARTITION_FIELD[$i]),\n";
				print NEW "		.flags  = PART_FLAG_NONE,\n";
				
				if($i == 0){
					print NEW "		.startblk = 0x0,\n";
				}
				
				print NEW "	},\n";
			}
			
			print NEW "
    {
        .name   = NULL,
        .flags  = PART_FLAG_END,
    },\n";
		
			print NEW "};\n";
			
			$index=$end_replace;
		}else{
			print NEW $arry[$index];
		}
			
	}

	close SOURCE;
	close NEW;
	rename($new,$UbootC) or &error_handler("CAN NOT rename $new\n",__FILE__, __LINE__);
}
#****************************************************************************************
# subroutine:  ModifyKernelPartitionC
# return:      
#****************************************************************************************
sub ModifyKernelPartitionC {
		if (-e $KernelH)
	{`chmod 777 $KernelH`;}	
	open (SOURCE, "$KernelH") or &error_handler("Ptgen CAN NOT open $KernelH", __FILE__, __LINE__);
	
	my $new=$KernelH.".tmp";
	open (NEW,">$new");

	my $index=0;
	my $start_replace=0;
	my $end_replace=0;
	my $br=0;
	my $matchBr=0;
	my $findbr=0;	
	my @arry=<SOURCE>;
	my $i;

	for($index=0;$index < @arry;$index++){
		#need replacement if match
		if($arry[$index]=~/static.*struct.*mtd_partition.*g_pasStatic_Partition.*\[.*\].*=.*{/)
		{
			$start_replace = $index;
		}
		if($start_replace ne 0){
		#if number of { eq number of },stop the replacement
			my @leftbr;
			my @rightbr;
			@leftbr = $arry[$index] =~/\{/g ;
			$br += @leftbr;
			$matchBr +=@leftbr;
			@rightbr = $arry[$index] =~/\}/g ;
			$br += @rightbr;
			$matchBr -=@rightbr;
			if($br ne 0 && $matchBr eq 0){
				$end_replace = $index;
				last;	
			}
		}
	}
	
#write tmp file
	for($index=0;$index < @arry;$index++){
		if($index == $start_replace){
			print NEW "static struct mtd_partition g_pasStatic_Partition[] = {\n";
			for($i=0;$i < @PARTITION_FIELD;$i++){
				if($PARTITION_FIELD[$i] eq "BMTPOOL" || $PARTITION_FIELD[$i] eq "OTP"){
					next;
				}
				print NEW "	{\n";
				if($kernel_alias{$PARTITION_FIELD[$i]}){
					print NEW "		.name   = \"$kernel_alias{$PARTITION_FIELD[$i]}\",\n"
				}else{
					my $t=lc($PARTITION_FIELD[$i]);
					print NEW "		.name   = \"$t\",\n";
				}
				if($i == 0){
					print NEW "		.offset = 0x0,\n";
				}else{
					print NEW "		.offset = MTDPART_OFS_APPEND,\n"
				}
				if($PARTITION_FIELD[$i] ne "USRDATA"){
					print NEW "		.size = PART_SIZE_$PARTITION_FIELD[$i],\n";
				}else{
					print NEW "		.size = MTDPART_SIZ_FULL,\n";
				}
				if($PARTITION_FIELD[$i] eq "PRELOADER" ||$PARTITION_FIELD[$i] eq "DSP_BL" ||$PARTITION_FIELD[$i] eq "UBOOT"){
					print NEW "		.mask_flags  = MTD_WRITEABLE,\n";
				}
				print NEW "	},\n";
			}
			print NEW "};\n";
			$index=$end_replace;
		}else{
			print NEW $arry[$index];
		}
			
	}

	close SOURCE;
	close NEW;
	rename($new,$KernelH) or &error_handler("CAN NOT rename $new\n",__FILE__, __LINE__);
}
#****************************************************************************************
# subroutine:  get_sheet
# return:      Excel worksheet no matter it's in merge area or not, and in windows or not
# input:       Specified Excel Sheetname
#****************************************************************************************
sub get_sheet {
  my ($sheetName,$Book) = @_;
  return $Book->Worksheet($sheetName);
}


#****************************************************************************************
# subroutine:  xls_cell_value
# return:      Excel cell value no matter it's in merge area or not, and in windows or not
# input:       $Sheet:  Specified Excel Sheet
# input:       $row:    Specified row number
# input:       $col:    Specified column number
#****************************************************************************************
sub xls_cell_value {
	my ($Sheet, $row, $col,$SheetName) = @_;
	my $cell = $Sheet->get_cell($row, $col);
	if(defined $cell){
		return  $cell->Value();
  	}else{
		my $error_msg="ERROR in ptgen.pl: (row=$row,col=$col) undefine in $SheetName!\n";
		print $error_msg;
		die $error_msg;
	}
}
