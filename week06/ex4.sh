#!/bin/bash
gcc worker.c -o worker
gcc scheduler_rr.c -o scheduler
./scheduler data.txt
