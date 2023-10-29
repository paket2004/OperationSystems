#!/bin/bash

# Find the process ID (PID) of the running "ex1.c" program.
PID=$(cat /tmp/ex1.pid)

# Load the "ex1.c" program into GDB.
gdb -q -ex "attach $PID" -ex 'call (void)printf("Password: %s\n",  (char *) $mmap_pas)' -ex "detach" -ex "quit"

# Send a SIGKILL signal to terminate the "ex1.c" program.
kill -9 $PID
