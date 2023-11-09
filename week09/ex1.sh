#!/bin/bash
gcc pager.c -o pager
./pager 5 3 & random
pager_pid=$!

gcc mmu.c -o mmu
./mmu 5 R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2
R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4 $pager_pid

gcc pager.c -o pager
./pager 5 3 & nfu
pager_pid=$!

gcc mmu.c -o mmu
./mmu 5 R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2
R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4 $pager_pid

gcc pager.c -o pager
./pager 5 3 & aging
pager_pid=$!

gcc mmu.c -o mmu
./mmu 5 R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2
R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4 $pager_pid
