#!/bin/bash
sudo apt install gcc-11 g++-11
sudo apt install cmake
sudo apt install libfmt-dev
sudo apt install googletest
sudo apt install libgtest-dev
sudo apt install valgrind
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 --slave /usr/bin/g++ g++ /usr/bin/g++-11 --slave /usr/bin/gcov gcov /usr/bin/gcov-11
sudo update-alternatives --set gcc /usr/bin/gcc-11