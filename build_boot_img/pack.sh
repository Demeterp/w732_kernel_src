cp ../kernel/arch/arm/boot/zImage .
../mkimage zImage KERNEL > boot.img-kernel.img
../repack-MT65xx.pl -boot boot.img-kernel.img boot.img-ramdisk boot.img
