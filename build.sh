#!/bin/bash

if [[ $1 = "debug" ]]; then
	echo "Compiling for debug"
	gcc -g -Wall -D "EXECUTABLE" Basilea.c -lm -o Basilea && ./Basilea $2
	exit
fi

echo "Generating executable..."
gcc -s -O3 -Wall -D "EXECUTABLE" Basilea.c -lm -o Basilea && echo "Success"


echo "Generating shared and static libraries"
gcc -fPIC -c -O3 -Wall Basilea.c -o basilea.o

if [[ $? = 0 ]]; then
	gcc -shared basilea.o -o libbasilea.so
	shared_result=$?
	
	ar rvs libbasilea.a basilea.o
	static_result=$?
	
	rm basilea.o
	
	# Echoes errors, if any
	if [[ $shared_result -ne 0 ]]; then
		echo "Shared library creation failed"
		exit
	fi
	if [[ $static_result -ne 0 ]]; then
		echo "Static library creation failed"
		exit
	fi
	
	echo "Success"
else
	echo "Failure"
fi
