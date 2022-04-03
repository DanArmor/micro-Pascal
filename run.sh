cd build &&
cmake .. &&
make Main &&
#./TEST &&
#valgrind ./testList
cd ..

for file in ./prog*
do
    cp $file ./build/
    if [[ $? -ne 0 ]] ; then
        printf "Error during coping pascal programs!\n"
        exit 1
    fi
done

cd build
./Main prog2.txt
dot out.dot -Tsvg > output.svg