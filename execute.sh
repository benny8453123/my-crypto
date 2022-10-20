#!/bin/bash

# ./execute.sh 0
if [ $# -eq 1 ] && [ $1 -eq 0 ]
then
	sudo rmmod encrypt-test-module.ko
	exit
fi

# ./execute.sh 1 [password you guess]
if [ $# -eq 2 ] && [ $1 -eq 1 ] && [ -n "$2" ]
then
    sudo insmod encrypt-test-module.ko mod_password="$2"
	sudo chown ben /proc/guess_password
	exit
fi

echo "Usage:"
echo "insmod + passing password to module: ./execute.sh 1 [password you guess]"
echo "rmmod: ./execute.sh 0"
