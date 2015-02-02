echo "Number of wrong results in basic test:" > "Test_basic_results.txt"
diff "Expected_results_basic.txt" "Test_basic.txt" | wc -l >> "Test_basic_results.txt"
