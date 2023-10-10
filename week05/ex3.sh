> ex3.txt 

gcc -pthread ex3.c -o ex3.out -lm

for i in 1 2 4 10 100
do  
    time ./ex3.out 10000000 $i >> ex3.txt 
    echo "$i thread(s)"
done
