#!/usr/bin/env python3
from asyncio.format_helpers import _format_callback_source
import os
from subprocess import check_output
import re
from time import sleep
from typing import List

THREADS = [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 ,26 ,28 ,30 ,32]
LOOPS = [100000]
RUNS_PER_COMBINATION = 5
INPUT_FILES = ["seq_64_test.txt", "1k.txt", "8k.txt", "16k.txt"]

class Data:
    input_file = ""
    thread_count = 0
    loop = 0
    time_sum = 0
    custom_barrier = False

data_set:List[Data] = []

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
    all_matched = True
    csvs = []
    for input_file in INPUT_FILES:
        for loop in LOOPS:
            csv = [f"{input_file}/{loop}"]
            for thread in THREADS:
                for barrier in ["", "-s"]:
                    d = Data()
                    d.input_file = input_file
                    d.thread_count = thread
                    d.loop = loop
                    if barrier == "":
                        d.custom_barrier = False
                    else:
                        d.custom_barrier = True
                    for run in range(RUNS_PER_COMBINATION):
                        cmd = f"./bin/prefix_scan -o temp.txt -n {thread} -i tests/{input_file} -l {loop} {barrier}"
                        out = check_output(cmd, shell=True).decode("ascii")
                        m = re.search("time: (.*)", out)
                        if m is not None:
                            time = m.group(1)
                            csv.append(time)
                        if not files_match(f"out/out_{input_file}", f"temp.txt"):
                            print(f"{input_file} with thread: {thread} loop: {loop} does not match.")
                            all_matched = False
                        d.time_sum += int(time)
                    data_set.append(d)
            csvs.append(csv)


    csv_string = f"input, loop count, thread count, time averaged over {RUNS_PER_COMBINATION} runs, custom barrier\n"

    for data in data_set:
        csv_string += f"{data.input_file}, {data.loop}, {data.thread_count}, {data.time_sum/RUNS_PER_COMBINATION}, {data.custom_barrier}\n"

    with open("data.csv", "w") as csv_file:
        csv_file.write(csv_string)
    if all_matched:
        print("ALL TESTS PASSED")

if __name__ == "__main__":
    main()