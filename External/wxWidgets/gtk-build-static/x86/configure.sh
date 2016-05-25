#!/bin/bash

../configure CC='gcc -m32' CXX='g++ -m32' -enable-unicode --disable-shared --with-libpng --with-zlib --with-expat --prefix=$(pwd)
