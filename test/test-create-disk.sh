#!/bin/bash -e

# create GPT table with EFI System Partition
rm -f test-disk
dd if=/dev/null of=test-disk bs=1M seek=512 count=1
parted --script test-disk "mklabel gpt" "mkpart ESP fat32 1M 512M" "set 1 boot on" "print"

# create FAT32 file system
LOOP=$(losetup --show -f -P test-disk)
mkfs.vfat -F32 $LOOP
mkdir -p mnt
mount $LOOP mnt

# install gummiboot
mkdir -p mnt/EFI/Boot
cp gummibootx64.efi mnt/EFI/Boot/bootx64.efi

[ -e /boot/shellx64.efi ] && cp /boot/shellx64.efi mnt/

# install entries
mkdir -p mnt/loader/entries
echo -e "timeout 3\n" > mnt/loader/loader.conf
echo -e "title Test\nefi /test\n" > mnt/loader/entries/test.conf
echo -e "title Test2\nlinux /test2\noptions option=yes word number=1000 more\n" > mnt/loader/entries/test2.conf
echo -e "title Test3\nlinux /test3\n" > mnt/loader/entries/test3.conf
echo -e "title Test4\nlinux /test4\n" > mnt/loader/entries/test4.conf
echo -e "title Test5\nefi /test5\n" > mnt/loader/entries/test5.conf
echo -e "title Test6\nlinux /test6\n" > mnt/loader/entries/test6.conf

sync
umount mnt
rmdir mnt
losetup -d $LOOP
