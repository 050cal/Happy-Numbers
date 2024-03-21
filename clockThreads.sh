#!/bin/bash

gcc -Wall -pedantic -g -pthread -o happyNums happyNums.c
echo "Executing program with one thread from 1 to 10,000,000"
echo
time ./happyNums -t 1 -l 10000000
echo
echo "Executing program with two threads from 1 to 10,000,000"
echo
time ./happyNums -t 2 -l 10000000
echo
echo "Executing program with three threads from 1 to 10,000,000"
echo
time ./happyNums -t 3 -l 10000000
echo
echo "Executing program with four threads from 1 to 10,000,000"
echo
time ./happyNums -t 4 -l 10000000
echo