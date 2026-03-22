#!/bin/bash
# Elijah Greig
# 3128908
# Submitted on: Mar 22 2026
# File: check_output.sh | Bash script for testing vmsim application output

PROGRAM_CMD="$1"
EXPECTED_FILE="$2"

TMP_OUTPUT=$(mktemp)

# Run program
eval "$PROGRAM_CMD" > "$TMP_OUTPUT"

# Compare with diff
if diff -uw "$EXPECTED_FILE" "$TMP_OUTPUT"; then
    echo "Test Passed, output matches."
    rm "$TMP_OUTPUT"
    exit 0
else
    echo "Test failed, output does not match."
    rm "$TMP_OUTPUT"
    exit 1
fi