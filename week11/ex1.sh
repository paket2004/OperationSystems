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

# Copy shared libraries for specified commands to LOFS directory
copy_libs_to_lofs "bash"
copy_libs_to_lofs "cat"
#copy_libs_to_lofs "echo"
sudo cp --parents /lib/x86_64-linux-gnu/libc.so.6 "./lofsdisk"
copy_libs_to_lofs "ls"

# Step 8: Change the root directory of the process to the mount point of the created LOFS
sudo cp /bin/bash ./lofsdisk/bin/
sudo chroot ./lofsdisk /bin/bash -c "gcc -o /bin/ex1 /lofsdisk/ex1.c"
sudo chroot ./lofsdisk /bin/bash -c "/bin/ex1" > ex1.txt.old

# Step 9: Run the same program again and append the output to the file ex1.txt
sudo chroot ./lofsdisk /bin/bash -c "/bin/ex1" >> ex1.txt

# Step 10: Display the differences between the outputs and append findings to ex1.txt
sudo diff ex1.txt ex1.txt.old >> ex1.txt

# Cleanup: Unmount LOFS and detach loop device
sudo umount ./lofsdisk
sudo losetup -d $loop_device
