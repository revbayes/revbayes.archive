#!/bin/sh 
 # Shell script to find out all the files under a directory and 
 #its subdirectories. This also takes into consideration those files
 #or directories which have spaces or newlines in their names 

DIR=""

function list_files()
{
    if !(test -d "$1")  
    then echo $1; return;
    fi

    cd "$1"
#    echo; echo `pwd`:; #Display Directory name

    for i in *
    do
               if test -d "$i"  #if dictionary
                then  
                          list_files "$i" "$2" "$3/$i"  #recursively list files
                  cd ..
                else
            echo "\${PROJECT_SOURCE_DIR}/$2$3/$i"; #Display File name
                fi

    done
}

echo "set(CORE_FILES"

for i in $*
do
    DIR="$1"
    list_files "$DIR" "$2" ""
    shift 1 #To read next directory/file name
done

echo ")";
echo "add_library(rb-core \${CORE_FILES})"