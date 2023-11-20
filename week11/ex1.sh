#!/bin/bash

# Create a file lofs.img not less than 50 MiB (I create 55 Mib)
fallocate -l 55M lofs.img

# Setup a loop device on the created file
loop_device=$(sudo losetup -f)
sudo losetup $loop_device lofs.img

# Create a Loop File System (LOFS) ext4 on the created file
sudo mkfs.ext4 $loop_device

# Create a new empty directory ./lofsdisk. Mount the created filesystem
mkdir -p ./lofsdisk
sudo mount $loop_device ./lofsdisk

# Add two files file1 and file2 with my name and surname to the LOFS
echo "Ilia" | sudo tee ./lofsdisk/file1 > /dev/null
echo "Mitrokhin" | sudo tee ./lofsdisk/file2 > /dev/null
# make a directories bin and libs inside lofsdisk (if they doesn't exist)
sudo mkdir -p ./lofsdisk/bin
sudo mkdir -p ./lofsdisk/libs

# Copying certain executable files from the /usr/bin/ directory to the current working directory. The executables being copied are bash, cat, echo, and ls. The --parents flag ensures that the directory structure leading to the destination is created if it doesn't exist.
sudo cp --parents /usr/bin/{bash,cat,echo,ls} ./

# Copying specific shared libraries from the /lib/x86_64-linux-gnu/ directory to the ./lofsdisk/libs/ directory. The libraries being copied are libtinfo.so.6, libc.so.6, libpcre2-8.so.0, libpthread.so.0. These libraries are necessary for the proper functioning of the executables (bash, cat, echo, ls) that were copied in the previous line.

sudo cp /lib/x86_64-linux-gnu/{libtinfo.so.6,libc.so.6,libpcre2-8.so.0,libpthread.so.0} ./lofsdisk/libs/

sudo cp /lib64/ld-linux-x86-64.so.2 ./lofsdisk/a
# gcc command compiles the ex1.c program, statically links it, includes debugging information and several sanitizers to catch potential issues during runtime. The resulting binary is named ex1 and is placed in the lofsdisk directory.
gcc ex1.c -o ./lofsdisk/ex1 -static -g -Wall -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow
# Change root directory of the process to mount point of the created LOFS, run ex1 and save output to ex1.txt
sudo chroot ./lofsdisk /ex1 > ex1.txt
# Again, compile a program and run it without changing root directory, append output to the ex1.txt
gcc ex1.c -o ./lofsdisk/ex1 -static -g -Wall -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow
./lofsdisk/ex1 >> ex1.txt

# Cleanup: Unmount LOFS and detach loop device
sudo umount ./lofsdisk
sudo losetup -d $loop_device
