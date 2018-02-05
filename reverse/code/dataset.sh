#!/bin/bash

echo $CWD

rm -f empty.txt
touch empty.txt

# abc only
echo -n "abc" > abc.txt
echo -n "cba" > abc.rev

# abcdef
echo -n "abcdef" > abcdef.txt
echo -n "fedcba" > abcdef.rev

# a
echo -n "a" > a.txt

ls -l