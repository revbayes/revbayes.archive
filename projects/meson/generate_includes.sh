#!/bin/sh

SRC=../../src

cd $SRC

echo -n "Generating includes ... "

echo "root_inc = include_directories([" > meson.build
find libs revlanguage core cmd -name '*.h' |
    xargs dirname |
    sort |
    uniq |
    sed "s|^|'|" |
    sed "s|$|',|" >> meson.build
truncate -s-2 meson.build
echo "])" >> meson.build
echo >> meson.build

echo "done."
