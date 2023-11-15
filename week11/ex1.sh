#!/bin/bash
fallocate -l 55M lofs.img
loop_device=$(losetup -f)
losetup $loop_device lofs.img
mkfs.ext4 $loop_device
sudo mkfs.ext4 $loop_device
mkdir ./lofsdisk
sudo mount $loop_device ./lofsdisk
cd ~/lofsdisk
echo "Ilia" > file1
echo "Mitrokhin" > file2

get_libs() {
  binary_file="$1"
  # Use ldd to get shared library information
  libs=$(ldd "$binary_file" | awk '/=>/ {print $3}')
  # Print the paths of shared libraries
  echo "$libs"
}

LOFS_DIR="./lofsdisk"

# Function to copy shared libraries to LOFS directory
copy_libs_to_lofs() {
    binary_name="$1"
    libs=$(get_libs "$(command -v $binary_name)")
    libs_dir="$LOFS_DIR/$binary_name/libs"
    mkdir -p "$libs_dir"
    
    # Copy each shared library to LOFS directory
    for lib in $libs; do
        cp "$lib" "$libs_dir/"
    done
}

# Copy shared libraries for specified commands to LOFS directory
copy_libs_to_lofs "bash"
copy_libs_to_lofs "cat"
copy_libs_to_lofs "echo"
copy_libs_to_lofs "ls"


