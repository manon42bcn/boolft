#!/bin/bash

# Check that input is OK
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <num_elements> <n1> <n2> ... <n..>"
    exit 1
fi

# Save count (param $1) and shift it.
count=$1
shift

# Count and elements are coherent
if [ "$#" -ne "$count" ]; then
    echo "Error: The number of elements does not match the count provided."
    exit 1
fi

# Save elems in array.
numbers=("$@")

# min and max difference
max_difference=-1
min_difference=99999999999999

for ((i=1; i<count; i++)); do
    diff=$((${numbers[i - 1]#-} - ${numbers[i]#-}))
    abs_diff=${diff#-}
    if (( abs_diff > max_difference )); then
        max_difference=$abs_diff
    fi
    if (( abs_diff < min_difference )); then
        min_difference=$abs_diff
    fi
done

# Result
echo $min_difference
echo $max_difference

