#!/bin/bash

# From https://askubuntu.com/questions/528928/how-to-do-underline-bold-italic-strikethrough-color-background-and-size-i
ansi()          { echo -e "\e[${1}m${*:2}\e[0m"; }
bold()          { ansi 1 "$@"; }
italic()        { ansi 3 "$@"; }
underline()     { ansi 4 "$@"; }
strikethrough() { ansi 9 "$@"; }
red()           { ansi 31 "$@"; }

stop() {
    kill $(jobs -p)
}

trap 'stop' SIGINT

test() {
    if [ -f "./$1" ]; then
        echo $(underline "Running tests for $3!")
        for filename in tests/input_*; do   
            (
                out_diff=`$2 7 ${filename} /dev/stdout | diff - ${filename/in/out}`
                if [ $? != 0 ]; then
                    echo -e "There was a diffrence in $filename\n$out_diff"
                else
                    echo "Finished test $filename"
                fi
            ) &
        done

        wait
    else
        echo "Cannot find file '$1', to run the $3 tests please include the executable."
        return -1;
    fi

    echo ""
    return 0;
}

test "hw1" "./hw1" "C"
if [ -f "./kmeans.py" ]; then echo $(red "WARNING: Python tests might take a long time!"); fi
test "kmeans.py" "python3 kmeans.py" "Python"
