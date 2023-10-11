> ex4.txt 

gcc -pthread ex4.c -o ex4.out -lm

for i in 1 2 4 10 100
do  
    time ./ex4.out 10000000 $i >> ex4.txt 
    echo "$i thread(s)"
done
