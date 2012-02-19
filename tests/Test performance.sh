echo "Number of wrong results in arithmetic test:" > "Test perfomance results.txt"
diff "Expected results arithmetics.txt" "Test results arithmetics.txt" | wc -l >> "Test perfomance results.txt"

echo "Number of wrong results in math-function test:" >> "Test perfomance results.txt"
diff "Expected results math-functions.txt" "Test results math-functions.txt" | wc -l >> "Test perfomance results.txt"

echo "Number of wrong results in user-function test:" >> "Test perfomance results.txt"
diff "Expected results user-functions.txt" "Test results user-functions.txt" | wc -l >> "Test perfomance results.txt"