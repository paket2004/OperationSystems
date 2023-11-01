#!/bin/bash
gcc ex1.c -o ex1
./ex1 &


program_pid=$(pgrep -f "./ex1")

# Extract the password from the correct program's memory
heap_line=$(grep "heap" /proc/$program_pid/maps)
start="0x$(echo "$heap_line" | awk '{print $1}' | cut -d'-' -f1)"
end="0x$(echo "$heap_line" | awk '{print $1}' | cut-d'-' -f2)"
xxd -s $start -l $(($end - $start)) /proc/$program_pid/mem | less | grep "pass:"

kill -9 $program_pid
rm -rf ex1
