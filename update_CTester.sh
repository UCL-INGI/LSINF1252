#!/bin/bash

cd "$(dirname "$0")"
DIRS=`ls -d */`

git clone https://github.com/UCL-INGI/CTester.git

for dir in $DIRS; do
    ls $dir".no_ctester" &> /dev/null
    if [[ $? -ne 0 ]]; then
        cp CTester/run $dir"run"
        cp CTester/student/Makefile $dir"student/Makefile"
        mkdir -p $dir"student/"
        cp -r CTester/student/CTester/ $dir"student/"
    fi
done

rm -rf CTester/
