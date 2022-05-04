#!/bin/bash
./allBuild.sh

for file in ./progs/prog*
do
    cp "$file" "./build/"
    if [[ $? -ne 0 ]] ; then
        printf "Error during coping pascal programs!\n"
        exit 1
    fi
done

cd build

mkdir public

for prog_file in prog*
do
    ./graphDraw "$prog_file"
    i=$prog_file
    dot out.dot -Tsvg > "public/output$i.svg"
done

dot out.dot -Tsvg > output.svg
