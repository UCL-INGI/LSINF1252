#!/bin/bash

EXEC=./test

function expect_test {
    ${EXEC} ${1}
    let ret=$?
    
    if [ $ret -ne ${2} ]; then
        echo "L'expression: <code>$1</code> retourne <code>$ret<code> mais devrait retourner <code>${2}</code>"
        exit 1
    fi
}

# test si les arguments ne sont pas bon
#expect_test "" 1

# test de l'implem
expect_test "1 -ne 1" 1
expect_test "0 -ne 1" 0
expect_test "-1 -ne 1" 0

expect_test "1 -eq 1" 0
expect_test "-456 -eq 456" 1
expect_test "4294967297 -eq 4294967297" 0

expect_test "1 -lt 1" 1
expect_test "1 -le 1" 0

expect_test "1 -gt 1" 1
expect_test "1 -ge 1" 0

expect_test "0 -ge 4294967297" 1
expect_test "-42 -le 43" 0
expect_test "42 -lt 41" 1

expect_test "+23 -eq 23" 0

echo "OK"

exit 0
