#! /bin/sh

dir_build="build"

cmake -S . -B${dir_build}
cd ${dir_build}
make
mv 42sh ..
