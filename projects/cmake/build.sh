#!/bin/sh
if [ "$1" = "clean" ]
then
	rm -rf build
else
if [ ! -d build ]; then
	mkdir build
fi
	./regenerate.sh $@
	cd build && cmake .
	make -j 4
	cd ..
fi
