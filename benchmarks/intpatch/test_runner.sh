#!/bin/bash

count_before=0
count_after=0

for file in ./*.c; do
    # Remove the .c extension
    filename=$(basename -- "$file")
    filename="${filename%.*}"
    echo "phx: $filename"
    # Run the script on the file
    sh run.sh "$filename" >/dev/null 2>&1 || true
    #Count the number of lines in the generated files
    if [ -f "${filename}_before.ll" ]; then
        count_before=$(($count_before + $(wc -l < "${filename}_before.ll")))
    fi
    if [ -f "${filename}_after.ll" ]; then
        count_after=$(($count_after + $(wc -l < "${filename}_after.ll")))
    fi
done

echo "Total lines in *_before.ll files: $count_before"
echo "Total lines in *_after.ll files: $count_after"
