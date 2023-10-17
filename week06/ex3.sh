#!/bin/bash
gcc worker.c -o worker
gcc scheduler_sjf.c -o scheduler
./scheduler data.txt
