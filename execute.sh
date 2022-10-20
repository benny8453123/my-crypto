#!/bin/bash

if [ $# -eq 1 ] && [ $1 -eq 0 ]
then
	sudo rmmod encrypt-test-module.ko
fi

if [ $# -eq 2 ] && [ $1 -eq 1 ] && [ -n "$2" ]
then
    sudo insmod encrypt-test-module.ko mod_password="$2"
	sudo chown ben /proc/guess_password
fi
