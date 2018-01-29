#!/bin/bash

cd "$(dirname "$0")"
DIRS=`ls -d */`

git clone https://github.com/Zashas/CTester.git

for dir in $DIRS; do
    cp CTester/run $dir"run"
    cp -r CTester/student/CTester/ $dir"student/"
done

rm -rf CTester/
