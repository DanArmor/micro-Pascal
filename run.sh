#!/bin/bash
cd build &&
cmake .. &&
make Main &&
#./TEST &&
#valgrind ./testList
cd ..

for file in ./progs/prog*
do
    cp "$file" "./build/"
    if [[ $? -ne 0 ]] ; then
        printf "Error during coping pascal programs!\n"
        exit 1
    fi
done

cd build
#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=val-ouput.txt 
./Main prog4.txt
dot out.dot -Tsvg > output.svg
