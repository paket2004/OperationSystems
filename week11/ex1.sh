#!/bin/bash

# Step 1: Create a file lofs.img not less than 50 MiB
fallocate -l 55M lofs.img

# Step 2: Setup a loop device on the created file
loop_device=$(sudo losetup -f)
sudo losetup $loop_device lofs.img

# Step 3: Create a Loop File System (LOFS) ext4 on the created file
sudo mkfs.ext4 $loop_device

# Step 4: Create a new empty directory ./lofsdisk. Mount the created filesystem
mkdir -p ./lofsdisk
sudo mount $loop_device ./lofsdisk

# Step 5: Add two files file1, file2 to the LOFS
echo "Ilia" | sudo tee ./lofsdisk/file1 > /dev/null
echo "Mitrokhin" | sudo tee ./lofsdisk/file2 > /dev/null

# Step 6: Define a function get_libs() to return shared libraries
get_libs() {
    binary_file="$1"
    libs=$(ldd "$binary_file" | awk '/=>/ {print $3}')
    echo "$libs"
}

# Step 7: Get shared libraries of commands and add them to LOFS
copy_libs_to_lofs() {
    binary_name="$1"
    libs=$(get_libs "$(command -v $binary_name)")
    libs_dir="./lofsdisk/libs/$binary_name"
    sudo mkdir -p "$libs_dir"
    
    # Copy each shared library to LOFS directory
    for lib in $libs; do
        sudo cp --parents "$lib" "./lofsdisk"
    done
}

sudo mkdir -p ./lofsdisk/bin
sudo mkdir -p ./lofsdisk/libs

# Copy executables
sudo cp --parents /usr/bin/{bash,cat,echo,ls} ./

# Copy dependencies

sudo cp /lib/x86_64-linux-gnu/{libtinfo.so.6,libc.so.6,libpcre2-8.so.0,libpthread.so.0} ./lofsdisk/libs/

sudo cp /lib64/ld-linux-x86-64.so.2 ./lofsdisk/a

gcc ex1.c -o ./lofsdisk/ex1 -static -g -Wall -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow
sudo chroot ./lofsdisk /ex1 > ex1.txt

gcc ex1.c -o ./lofsdisk/ex1 -static -g -Wall -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow
./lofsdisk/ex1 >> ex1.txt

# Cleanup: Unmount LOFS and detach loop device
sudo umount ./lofsdisk
sudo losetup -d $loop_device
