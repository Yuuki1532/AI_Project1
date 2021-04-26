#!/bin/bash
num=0
while :
do
    num=$(($num+1))
    ./bin/main
    echo "$?" >> result.txt
    echo "$num $?"
done
