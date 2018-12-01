#!/bin/bash
 
RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'

touch testik
i=1
leaks=0
errors=0
returnval=0

echo -e "\nTEST0$i: Empty file \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <testik &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm testik
rm output

echo -e "\nFunction definition syntax tests\n"
i=$((i+1))
echo -e "TEST0$i: Function without parameters \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test0$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST0$i: Function with one parameter \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test0$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST0$i: Function with two parameters \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test0$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST0$i: Function returns string value \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test0$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST0$i: Function returns float value \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test0$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST0$i: Function returns integer value \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test0$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST0$i: Missing closing parentheses \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test0$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST0$i: Missing opening parentheses \c" 
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test0$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing closing parentheses with paramater \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Comma after last parameter \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Double comma after parameter \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: No end after function definition \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Function definition inside function definition \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: No newline after parameters \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: No newline after end \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing parentheses with parameters \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Multiple functions \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

echo -e "\nWhile loop syntax tests\n"

i=$((i+1))
echo -e "TEST$i: While loop \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Multiple while loops inside each other \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Multiple while loops \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing do token \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing EOL after do token \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing end token \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing expression \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

echo -e "\nIf condition syntax tests\n"

i=$((i+1))
echo -e "TEST$i: Single if condition \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Nested if conditions \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Many if conditions \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing end in if condition \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing then token \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing expression token in if condition \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing EOL after then token \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing EOL after else token \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing else and end \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: EOL after expression \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing first part of if condition \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

echo -e "\nAssignment syntax tests\n"

i=$((i+1))
echo -e "TEST$i: Assignment from integer \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from float \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from string \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from function \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from id \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Initialization of id with the same id (id = id) \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Only id on the line \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from expression \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters, missing comma \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters in parentheses, missing comma\c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by comma in parentheses \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by comma \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by multiple commas \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Wrong token after id \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing right side of assignment with defined variable \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function without parameters \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function with one parameter, without parentheses \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function with one parameter, with parentheses \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, without parentheses and commas \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, without commas \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, with parentheses and commas \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function without parameters, with parentheses\c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing right side of assignment with undefined variable \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

echo -e "\nExpression syntax tests\n"

i=$((i+1))
echo -e "TEST$i: Various add and substract expressions without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Concatenation of expressions without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Concatenation of expressions in assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Various add and substract expressions with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Nil assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Various multiply and divide expressions without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Various multiply and divide expressions with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Various RO and CO expressions without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Various RO and CO expressions with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Various expressions with parantheses, no assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Various expressions with parantheses, and assigned \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Wrong relation operator syntax in expression \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Wrong compare operator syntax in expression \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Just parentheses in expression \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Wrong open parentheses syntax \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Wrong closed parentheses syntax \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing operator in front of parentheses \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing operator behind parentheses \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing operator between two values \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Combination of relation and compare operators \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Missing operator between two parentheses \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
i=$((i+1))
i=$((i+1))
echo -e "TEST$i: Compare operation with nil, without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
i=$((i+1))
echo -e "TEST$i: Nil in parentheses, without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
i=$((i+1))
i=$((i+1))
echo -e "TEST$i: Compare operation with nil, with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
i=$((i+1))
echo -e "TEST$i: Nil in parentheses, with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Comparing nil with integer, without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Comparing nil with integer, with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

echo -e "\nOther various tests\n"

i=$((i+1))
echo -e "TEST$i: Calling undefined function with IDF token type, without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Trying to assign into IDF variable \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function with IDF token name, without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling function with IDF token name, with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling undefined function with IDF token type, with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling builtin functions, without assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

i=$((i+1))
echo -e "TEST$i: Calling builtin functions, with assignment \c"
valgrind --leak-check=full --error-exitcode=23 --log-file="output" ./ifj2018 <tests/tests_compiler/test$i &>/dev/null
if grep -q -e "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" output ; then
    echo -e "## Error ${GREEN}[OK]${NOCOLOR} \c"
else
    errors=$((errors+1))
    echo -e "## Error ${RED}[FAILED]${NOCOLOR} \c"
fi
if grep -q -e "All heap blocks were freed -- no leaks are possible" output ; then
    echo -e "Leak ${GREEN}[OK]${NOCOLOR}"
else
    leaks=$((leaks+1))
    echo -e "Leak ${RED}[FAILED]${NOCOLOR}"
fi
rm output

if [ "$errors" == 0 ]; then
    echo -e "\nAll error tests were ${GREEN}[OK]${NOCOLOR}.\n"
else
    echo -e "\n Sorry, but" $errors " error tests ${RED}[FAILED]${NOCOLOR}.\n"
fi

if [ "$leaks" == 0 ]; then
    echo -e "\nAll leak tests were ${GREEN}[OK]${NOCOLOR}.\n"
else
    echo -e "\n Sorry, but" $leaks " leak tests ${RED}[FAILED]${NOCOLOR}.\n"
fi