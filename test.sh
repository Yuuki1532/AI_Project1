#!/bin/bash

while :
do
    ./bin/main
    echo "$?" >> result.txt
done
