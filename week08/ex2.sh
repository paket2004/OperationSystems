#!/bin/bash
gcc pager.c -o pager
/.pager 4 2
gcc mmu.c -o mmu
pid_pager = $(pgreep pager)
gnome-terminal -- bash -c "./pager 4 2; exec bash"
pid_pager=$(pgrep pager)
gnome-terminal -- bash -c "./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pid_pager; exec bash"
