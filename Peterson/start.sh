#!/bin/sh
/usr/lib64/openmpi/bin/mpirun -np 30 ./peterson --batch --input /home/user/Peterson_the_last/Input/input.txt --output /home/user/Peterson_the_last/Output/out.txt &

