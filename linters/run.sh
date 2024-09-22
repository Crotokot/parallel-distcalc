#!/usr/bin/env bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}

export LIST_CPP_FILES=$(find . -name "*.cpp" -not -path "*build*" -not -path "*boost*")
export LIST_HPP_FILES=$(find . -name "*.h" -not -path "*build*" -not -path "*boost*")

print_header "RUN cppcheck"
check_log "cppcheck ${LIST_CPP_FILES} ${LIST_HPP_FILES} --std=c++17 --includes-file=linters/include_paths --enable=all --inconclusive --check-config --error-exitcode=1 --suppressions-list=linters/suppress.txt" "\(information\)"

print_header "RUN cpplint"
check_log "cpplint --extensions=cpp,h ${LIST_CPP_FILES} ${LIST_HPP_FILES}" "Can't open for reading"

print_header "SUCCESS"