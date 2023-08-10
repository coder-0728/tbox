#! /bin/sh
SRC_DIR=`pwd`

if [ "$1" = "test" ];then
    rm CMakeLists.txt
    cp CMakeLists_test.txt CMakeLists.txt
elif [ "$1" = "arm" ];then
    rm CMakeLists.txt
    cp CMakeLists_arm.txt CMakeLists.txt
fi

if [ -z "$2" -o "$2" = "cmake" ];then 
    rm ${SRC_DIR}/build/ -rf 
    rm ${SRC_DIR}/out -rf 
    mkdir ${SRC_DIR}/build

    cd ${SRC_DIR}/build/

    cmake ..
    if [ ! $? -eq 0 ];then 
        echo "[sirun error] cmake error"
        exit
    fi
fi

cd ${SRC_DIR}/build/
if [ -z "$2" -o  "$2" = "make" ];then
    make
    if [ ! $? -eq 0 ];then 
        echo "[sirun error] make error"
        exit
    fi
fi


if [ "$1" = "clean" ];then
    make clean    
fi

if [ "$1" != "clean" -a "$1" != "cmake" ];then
    make install
fi
