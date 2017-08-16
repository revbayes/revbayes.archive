#!/bin/bash

for f in scripts/*; do
    projects/cmake/rb $f > output_tmp.txt
done

rm output_tmp.txt

for f in $(ls output); do
    echo "$f"
    res=$(diff <(tail -n +3 output/$f) <(tail -n +3 output_expected/$f) | wc -l)
    if [ $res -gt 0 ]; then
        exit 113
    fi
done
#diff "Expected_results_basic.txt" "Test_basic.txt" | wc -l >> "Test_basic_results.txt"
