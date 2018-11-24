#!/bin/bash
 
RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'

touch test
i=1
echo -e "\nTEST0$i: Empty file \c"
./ifj2018 <test 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi
rm test

echo -e "\nFunction definition syntax tests\n"
i=$((i+1))
echo -e "TEST0$i: Function without parameters \c"
./ifj2018 <tests_compiler/test0$i 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function with one parameter \c"
./ifj2018 <tests_compiler/test0$i 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function with two parameters \c"
./ifj2018 <tests_compiler/test0$i
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function returns string value \c"
./ifj2018 <tests_compiler/test0$i 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function returns float value \c"
./ifj2018 <tests_compiler/test0$i 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function returns integer value \c"
./ifj2018 <tests_compiler/test0$i 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Missing closing parentheses \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Missing opening parentheses \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing closing parentheses with paramater \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Comma after last parameter \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Double comma after parameter \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: No end after function definition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Function definition inside function definition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: No newline after parameters \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: No newline after end \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing parentheses with parameters \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Multiple functions \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

echo -e "\nWhile loop syntax tests\n"

i=$((i+1))
echo -e "TEST$i: While loop \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Multiple while loops inside each other \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Multiple while loops \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing do token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing EOL after do token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing end token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

echo -e "\nIf condition syntax tests\n"

i=$((i+1))
echo -e "TEST$i: Single if condition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Nested if conditions \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Many if conditions \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing end in if condition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing then token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing expression token in if condition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing EOL after then token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing EOL after else token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing else and end \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: EOL after expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing first part of if condition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

echo -e "\nAssignment syntax tests\n"

i=$((i+1))
echo -e "TEST$i: Assignment from integer \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from float \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from string \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from id \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Initialization of id with the same id (id = id) \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Only id on the line \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters, missing comma \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters in parentheses, missing comma\c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by comma in parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by comma \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by multiple commas \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Wrong token after id \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing right side of assignment with defined variable \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function without parameters \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with one parameter, without parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with one parameter, with parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, without parentheses and commas \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, without commas \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, with parentheses and commas \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function without parameters, with parentheses\c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing right side of assignment with undefined variable \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi