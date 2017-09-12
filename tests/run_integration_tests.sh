#!/bin/bash

for f in scripts/*; do
    printf "\n\n#### Running test script: $f\n\n"
    ../projects/cmake/rb $f # print output so we can see any error messages
done

failed=0
printf "\n\n#### Checking output from test scripts... \n"
for f in $(ls output); do
    res=$(diff <(tail -n +3 output/$f) <(tail -n +3 output_expected/$f) | wc -l)
    if [ $res -gt 0 ]; then
        printf "#### Warning! Test failed: $f\n"
        (( failed++ ))
    else
        printf "#### Test passed: $f\n"
    fi
done
#diff "Expected_results_basic.txt" "Test_basic.txt" | wc -l >> "Test_basic_results.txt"
if [ $failed -gt 0 ]; then
    printf "\n\n#### Warning! $failed tests failed.\n\n"
    exit 113
else
    printf "\n\n#### All tests passed.\n\n"
fi
