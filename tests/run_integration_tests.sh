#!/bin/bash

for f in scripts/*; do
    ../projects/cmake/rb $f > output_tmp.txt
done

rm output_tmp.txt

failed=0
for f in $(ls output); do
    res=$(diff <(tail -n +3 output/$f) <(tail -n +3 output_expected/$f) | wc -l)
    if [ $res -gt 0 ]; then
        echo "Warning! Test failed: $f"
        (( failed++ ))
    else
        echo "Test passed: $f"
    fi
done
#diff "Expected_results_basic.txt" "Test_basic.txt" | wc -l >> "Test_basic_results.txt"
if [ $failed -gt 0 ]; then
    echo "Warning! $failed tests failed."
    exit 113
else
    echo "All tests passed."
fi
