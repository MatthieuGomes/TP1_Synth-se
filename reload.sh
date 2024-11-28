#!/bin/bash

gcc -o enseash src/*.c -Werror -Wall -Wextra -g 
if [ $? -eq 0 ] 
    then ./enseash
else
    exit 1
fi
    