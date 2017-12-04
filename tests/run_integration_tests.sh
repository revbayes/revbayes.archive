#!/bin/bash

tests=()
status=()

for t in test_*; do
    if [ -d $t ]; then
        tests+=($t)
    else
        continue
    fi
    
    printf "\n\n#### Running test: $t\n\n"
    cd $t
    
    rm -rf output data
    ln -s ../data data

    res=0
    # run the test scripts
    for f in scripts/*; do
        ../../projects/cmake/rb -b $f # print output so we can see any error messages
        res="$?"
        if [ $res = 1 ]; then
            res="error: $f"
        elif [ $res = 139 ]; then
            res="segfault: $f"
        elif [ $res != 0 ]; then
            res="error $res: $f"
            break
        fi
    done

    # store the exit status
    status+=("$res")

    rm data

    cd ..
done

printf "\n\n#### Checking output from tests... \n"
failed=0
i=0
while [  $i -lt ${#tests[@]} ]; do
    t=${tests[$i]}
    cd $t

    # check if output matches expected output
    errs=()
    for f in $(ls output_expected); do
        if [ ! -e output/$f ]; then
            errs+=("missing:  $f")
        elif ! diff <(tail -n +3 output/$f) <(tail -n +3 output_expected/$f) > /dev/null; then
            errs+=("mismatch: $f")
        fi
    done

    if [ "${status[$i]}" != 0 ]; then
        errs=("${status[$i]}")
    fi

    # failure if the output doesn't match, or the script aborted
    if [ ${#errs[@]} -gt 0 ]; then
        ((failed++))
        printf ">>>> Test failed: $t\n"
        for errmsg in "${errs[@]}"; do
            printf "\t$errmsg\n"
        done
    else
        printf "#### Test passed: $t\n"
    fi

    cd ..

    ((i++))
done


if [ $failed -gt 0 ]; then
    printf "\n\n#### Warning! $failed tests failed.\n\n"
    exit 113
else
    printf "\n\n#### All tests passed.\n\n"
fi
