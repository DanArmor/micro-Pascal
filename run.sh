cd build &&
cmake .. &&
make Main &&
#./TEST &&
#valgrind ./testList
cd ..

for file in ./progs/prog*
do
    cp $file ./build/
    if [[ $? -ne 0 ]] ; then
        printf "Error during coping pascal programs!\n"
        exit 1
    fi
done

cd build
valgrind --leak-check=full ./Main prog1.txt
dot out.dot -Tsvg > output.svg
