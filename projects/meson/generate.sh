#!/bin/sh

SRC=../../src

cd $SRC

echo "root_inc = include_directories([" > meson.build
find libs revlanguage core -name '*.h' |
    xargs dirname |
    sort |
    uniq |
    sed "s|^|'|" |
    sed "s|$|',|" >> meson.build
truncate -s-2 meson.build
echo "])" >> meson.build
echo >> meson.build

echo "rb_sources = files([" >> meson.build
find libs core revlanguage -name '*.cpp' |
    sed "s|^|'|" |
    sed "s|$|',|" >> meson.build
find libs core revlanguage -name '*.c' |
    sed "s|^|'|" |
    sed "s|$|',|" >> meson.build
truncate -s-2 meson.build
echo "])" >> meson.build
echo >> meson.build


