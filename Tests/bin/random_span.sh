#!/bin/bash

# Easy check: we need 3 args to build the list
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <min_value> <max_value> <count>"
    exit 1
fi

# Needed vars
min_value=$1
max_value=$2
count=$3
numbers=()

# Verify if the list is possible. Enough range to the needed list.
if [ "$((max_value - min_value + 1))" -lt "$count" ]; then
    echo "Error: The range of numbers is too small to generate $count unique numbers."
    exit 1
fi

# Echo number of elements followed by the random span
echo $count $(seq $min_value $max_value | shuf -n $count)
