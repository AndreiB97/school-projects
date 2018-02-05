#!/bin/bash

## CONSTANTS ##
CURRENT_DIRECTORY=`pwd`
RESOURCES_DIRECTORY=$1
CODING_STYLE_BONUS=0
ERROR_BONUS=0
ERRORS_NUMBER=0
PASSED_TESTS_GRADE=0
VALGRIND_ERRORS=0
VALGRIND_LEAKS=0
NR_TESTS=10
EXEC_NAME=tema1

MESSAGE_SIZE=12
ONE=1
TWO=2
## FUNCTIONS ##
function cleanHomework
{
    make clean &> /dev/null
}

function compileHomework
{
    echo ""
    # build here
    make build
}

function checkTests
{
    for i in $(seq 1 $(($NR_TESTS))); do
        cp $EXEC_NAME $i/
        cd $i
        ./$EXEC_NAME
        cd ..

        if [[ -z $(diff --strip-trailing-cr $i"/ram.out" $i"/ram.ref") ]] && [[ -z $(diff --strip-trailing-cr $i"/cache.out" $i"/cache.ref") ]]; then 
            PASSED_TESTS_GRADE=$(($PASSED_TESTS_GRADE+1));
            echo "test"$i".............$(tput setaf 2)1$(tput sgr0)"
        else
            echo "test"$i"............. $(tput setaf 1)0$(tput sgr0)"
            diff --strip-trailing-cr -q $i"/ram.out" $i"/ram.ref"
            diff --strip-trailing-cr -q $i"/cache.out" $i"/cache.ref"
        fi
    done
}

function checkValgrind
{
    for i in $(seq 1 $(($NR_TESTS))); do
        cd $i
        valgrind -q --log-file=errors.out ./$EXEC_NAME > /dev/null;
        if [ -s errors.out ]; then
            VALGRIND_ERRORS=$((VALGRIND_ERRORS+1));
        fi
        rm -rf errors.out
        cd ..
    done

    for i in $(seq 1 $(($NR_TESTS))); do
        cd $i
        valgrind -q --leak-check=full --log-file=leaks.out ./$EXEC_NAME > /dev/null;
        if [ -s leaks.out ]; then
            VALGRIND_LEAKS=$((VALGRIND_LEAKS+1));
        fi
        rm -rf leaks.out
        rm -rf $EXEC_NAME
        cd ..
    done
}

function checkBonus
{
    printf '%*s\n' "${COLUMNS:-$(($(tput cols) - $ONE))}" '' | tr ' ' -
    echo "" > checkstyle.txt
    TargetDir=$RESOURCES_DIRECTORY
    
    echo -ne "Coding style Errors:\n"
    
    for entry in "$RESOURCES_DIRECTORY"/*
    do
        if [[ -f $entry ]]; then
            ./cpplint.py "$entry" > checkstyle.txt 
            YOUR_BONUS=`cat checkstyle.txt | grep "Total errors found: "| tr -dc '0-9'`
            ERRORS_NUMBER=$(( $ERRORS_NUMBER + $YOUR_BONUS ))
        fi
    done

    if [[ $ERRORS_NUMBER != "0" ]]; then
        printf '\nBonus: %*sFAIL' "${COLUMNS:-$(($(tput cols) - $MESSAGE_SIZE))}" '' | tr ' ' -
        echo -ne "\n\t\tYou have a total of $ERRORS_NUMBER coding style errors.\n"

    else
        echo -ne "\n\tNone?\n\t\tWow, next level!"
        printf '\n\nBonus: %*sOK' "${COLUMNS:-$(($(tput cols) - $MESSAGE_SIZE + $TWO ))}" '' | tr ' ' -

        CODING_STYLE_BONUS=$(($PASSED_TESTS_GRADE * 2))
        CODING_STYLE_BONUS=$(($CODING_STYLE_BONUS))
    fi
}

function printBonus
{
    if [[ $ERRORS_NUMBER -gt 0 ]]; then
    printf '\n%*s' "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
        echo -ne  "\n\tAnd you almost did it!\n"
    else
        echo -ne "\n\tDamn!\n"
    fi
    echo -ne "\n\t\tYou got a bonus of 0.$CODING_STYLE_BONUS/0.2.\n\n"
}

## MAIN EXECUTION ##
cleanHomework


## CHECK TESTS
compileHomework && checkTests && checkValgrind

NO_NOTE=0

if [ $VALGRIND_ERRORS -ne 0 ]; then
    echo "Valgrind errors, so I can not give grade. Need human assistance."
    NO_NOTE=1
fi

if [ $VALGRIND_LEAKS -ne 0 ]; then
    echo "Memleaks, I can not give grade. Need human assistance."
    NO_NOTE=1
fi

if [ $NO_NOTE -eq 1 ]; then
    exit -1
fi;

echo ""
if [ $PASSED_TESTS_GRADE -eq 0 ]; then
    echo "GRADE.................................$(tput setaf 1)$PASSED_TESTS_GRADE$(tput sgr0)";
else
    echo "GRADE.................................$PASSED_TESTS_GRADE";
fi;

## BONUS CHECK
checkBonus

## BONUS RESULTS
printBonus

cleanHomework
