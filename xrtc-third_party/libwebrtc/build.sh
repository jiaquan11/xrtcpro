#!/bin/bash
if [ ! -d "./out" ];then
    mkdir out
fi

cd out && cmake ../

if test $# -gt 0 && test $1 = "clean"
then
    echo "make clean"
    make clean
elif test $# -gt 0 && test $1 = "install"
then
    echo "make install"
    make install
else
    echo "make"
    make
fi

