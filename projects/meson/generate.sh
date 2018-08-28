#!/bin/sh

SRC=../../src

./generate_includes.sh

echo "
subdir('core')
subdir('revlanguage')
subdir('libs')
subdir('cmd')
" >> $SRC/meson.build

./generate_sources.sh core

./generate_sources.sh revlanguage

./generate_sources.sh libs

./generate_sources.sh cmd
