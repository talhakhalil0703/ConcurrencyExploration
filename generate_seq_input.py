#!/usr/bin/env python3
import os
import sys
try:
    os.mkdir("tests")
except:
    pass

if __name__ == "__main__":
  if len(sys.argv) < 2:
    print("generate_seq_input.py <numeber>")
  number = int(sys.argv[1])
  with open(os.path.join("tests", f"seq_{number}_test.txt"), 'w') as f:
      f.write(f"{number}\n")
      for i in range(number):
          f.write("{}\n".format(i))