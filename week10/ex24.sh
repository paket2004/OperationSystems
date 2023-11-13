#!bin/bash
#rm -rf ./tmp
#ln -s ./tmp tmp1
#ls -li
#mkdir ./tmp
#ls -li
# We  see the tmp1 symbolic link pointing to ./tmp, and now there's a new ./tmp directory.
#bash gen.sh 5 ex1.txt
#mv ex1.txt ./tmp
#bash gen.sh 5 ex1.txt
#mv ex1.txt ./tmp1/tmp2
#I  obtain an infinite loop, so this is not the best practice to do)
#rm -rf ./tmp
#find -type l -exec rm {} \;
