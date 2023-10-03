gcc ex3.c -o ex3

./ex3 3 &
sleep 1
pstree -p -a > ex3_1.txt
sleep 1
pstree -p -a >> ex3_1.txt


./ex3 5 &
sleep 1
pstree -p -a > ex3_2.txt
sleep 1
pstree -p -a >> ex3_2.txt
sleep 1
pstree -p -a >> ex3_2.txt
# writing to file just to check does my code output process correctly

rm ex3
