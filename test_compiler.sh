#!/bin/bash
 
RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'

touch test
i=1
success=0

echo -e "\nTEST0$i: Empty file \c"
./ifj2018 <test &>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi
rm test

echo -e "\nFunction definition syntax tests\n"
i=$((i+1))
echo -e "TEST0$i: Function without parameters \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function with one parameter \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function with two parameters \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function returns string value \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function returns float value \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Function returns integer value \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Missing closing parentheses \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST0$i: Missing opening parentheses \c"
./ifj2018 <tests_compiler/test0$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing closing parentheses with paramater \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Comma after last parameter \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Double comma after parameter \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: No end after function definition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Function definition inside function definition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: No newline after parameters \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: No newline after end \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing parentheses with parameters \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Multiple functions \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

echo -e "\nWhile loop syntax tests\n"

i=$((i+1))
echo -e "TEST$i: While loop \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Multiple while loops inside each other \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Multiple while loops \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing do token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing EOL after do token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing end token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

echo -e "\nIf condition syntax tests\n"

i=$((i+1))
echo -e "TEST$i: Single if condition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Nested if conditions \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Many if conditions \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing end in if condition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing then token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing expression token in if condition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing EOL after then token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing EOL after else token \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing else and end \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: EOL after expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing first part of if condition \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

echo -e "\nAssignment syntax tests\n"

i=$((i+1))
echo -e "TEST$i: Assignment from integer \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from float \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from string \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from id \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Initialization of id with the same id (id = id) \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Only id on the line \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters, missing comma \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters in parentheses, missing comma\c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by comma in parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by comma \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Assignment from function with parameters separated by multiple commas \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Wrong token after id \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing right side of assignment with defined variable \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function without parameters \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with one parameter, without parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with one parameter, with parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, without parentheses and commas \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, without commas \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with more parameters, with parentheses and commas \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function without parameters, with parentheses\c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing right side of assignment with undefined variable \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

echo -e "\nExpression syntax tests\n"

i=$((i+1))
echo -e "TEST$i: Various add and substract expressions without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Concatenation of expressions without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Concatenation of expressions in assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Various add and substract expressions with assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Nil assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Various multiply and divide expressions without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Various multiply and divide expressions with assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Various RO and CO expressions without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Various RO and CO expressions with assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Various expressions with parantheses, no assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Various expressions with parantheses, and assigned \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Wrong relation operator syntax in expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 4 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Wrong compare operator syntax in expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 4 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Just parentheses in expression \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Wrong open parentheses syntax \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Wrong closed parentheses syntax \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing operator in front of parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing operator behind parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing operator between two values \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Combination of relation and compare operators \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 4 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Missing operator between two parentheses \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
i=$((i+1))
i=$((i+1))
echo -e "TEST$i: Compare operation with nil, without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
i=$((i+1))
echo -e "TEST$i: Nil in parentheses, without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
i=$((i+1))
i=$((i+1))
echo -e "TEST$i: Compare operation with nil, with assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
i=$((i+1))
echo -e "TEST$i: Nil in parentheses, with assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Comparing nil with integer, without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Comparing nil with integer, with assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

echo -e "\nOther various tests\n"

i=$((i+1))
echo -e "TEST$i: Calling undefined function with IDF token type, without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 3 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Trying to assign into IDF variable \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 2 -o $? -eq 3 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with IDF token name, without assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling function with IDF token name, with assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 0 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

i=$((i+1))
echo -e "TEST$i: Calling undefined function with IDF token type, with assignment \c"
./ifj2018 <tests_compiler/test$i &>/dev/null 
if [ $? -eq 3 ]; then
    echo -e "${GREEN}[OK]${NOCOLOR}"
else
    success=1
    echo -e "${RED}[FAILED]${NOCOLOR}"
fi

if [ "$success" == 0 ]; then
    echo -e "\nAll tests were ${GREEN}[OK]${NOCOLOR}.\n"
else
    echo -e "\nSome tests ${RED}[FAILED]${NOCOLOR}.\n"
fi