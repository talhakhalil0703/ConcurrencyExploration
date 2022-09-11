#!/usr/bin/env python3
import os
from subprocess import check_output
import re
from time import sleep

THREADS = [1]
LOOPS = [1]
INPUT_FILES = ["seq_64_test.txt", "1k.txt", "8k.txt", "16k.txt"]

def files_match(file_one, file_two) -> bool:
    first = []
    second = []
    with open(file_one, "r") as file:
        first  = file.readlines()
    with open(file_two, "r") as file:
        second = file.readlines()

    for index, value in enumerate(first):
        if value != second[index]:
            return False

    return True

def main() -> None:
    csvs = []
    for input_file in INPUT_FILES:
        for loop in LOOPS:
            csv = [f"{input_file}/{loop}"]
            for thread in THREADS:
                cmd = "./bin/prefix_scan -o temp.txt -n {} -i tests/{} -l {}".format(
                    thread, input_file, loop)
                out = check_output(cmd, shell=True).decode("ascii")
                m = re.search("time: (.*)", out)
                if m is not None:
                    time = m.group(1)
                    csv.append(time)
                if not files_match(f"out/out_{input_file}", f"temp.txt"):
                    print(f"{input_file} with thread: {thread} loop: {loop} does not match.")

            csvs.append(csv)

    header = ["microseconds"] + [str(x) for x in THREADS]

    print("\n")
    print(", ".join(header))
    for csv in csvs:
        print (", ".join(csv))


if __name__ == "__main__":
    main()