#!/bin/bash
./allBuild.sh

if [[ $? -ne 0 ]] ; then
    printf "Error during execution of allBuild.sh!\n"
    exit 1
fi

for file in ./tests/lexer/text*
do
    cp "./$file" "./build/tests/"
    if [[ $? -ne 0 ]] ; then
        printf "Error during coping test data for lexer!\n"
        exit 1
    fi
done

for file in ./tests/syntax/text*
do
    cp "./$file" "./build/tests/"
    if [[ $? -ne 0 ]] ; then
        printf "Error during coping test data for syntax!\n"
        exit 1
    fi
done

for file in ./tests/semantic/text*
do
    cp "./$file" "./build/tests/"
    if [[ $? -ne 0 ]] ; then
        printf "Error during coping test data for syntax!\n"
        exit 1
    fi
done

for file in ./build/tests/test*
do
    "./$file"
    if [[ $? -ne 0 ]] ; then
        printf "Error during execution of tests!\n"
        exit 1
    fi
done