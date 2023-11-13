#!/bin/bash
#chmod +x gen.sh
# ./gen.sh 10 ex1.txt
#ln -s ex1.txt ex11.txt
#ln -s ex1.txt ex12.txt
#diff ex1.txt ex11.txt
#diff ex1.txt ex12.txt
#diff command doesn't provide any input, because files ex11.txt and ex12.txt should be identical to ex1.txt as they point to the same underlying file.
#ls -i ex1.txt
#ls -i ex11.txt
#ls -i ex12.txt
#They have same i-numbers, becaues we link it.
# du ex1.txt output: 4 kilobytes.
# current directory:
#find ./ -inum 56021
#./ex1.txt
#./ex11.txt
#./test.txt
#./ex12.txt
#root directory:
#/root/week10bonus/ex1.txt
#/root/week10bonus/ex11.txt
#/root/week10bonus/test.txt
#/root/week10bonus/ex12.txt
#/tmp/ex13.txt
#I just created test file to check some info. In the current directory we can't find ex13.txt because we move it to another directory and we use search in root to find that file.
#stat -c "%h" ex1.txt ANS: 5 (4 + test.txt)
# find . -inum $(stat -c %i ex1.txt) -exec rm {} \;
