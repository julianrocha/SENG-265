#!/usr/bin/env python3

import sys
import classykwic

def main():
    count = 0
    mode = 0   # 0 = version; 1 = exclusion; 2 = actual lines
    exclusion = 0

    excluded = []
    input_lines = []

    for line in sys.stdin:
        line = line.rstrip()
        if (mode == 0 and line != "::"):
            kwic_version = int(line)
            if (kwic_version != 2):
                print("File has version ", kwic_version,
                    ", expected 2")
                sys.exit(1)

            continue
        elif (line == "::" and mode == 0):
            mode = 1
            continue
        elif (line == "::" and mode == 1):
            mode = 2
            continue

        if (mode == 1):
            excluded.append(line.lower())
        elif (mode == 2):
            if (line != ""):
                input_lines.append(line)

    k = classykwic.Kwic(excluded, input_lines)


if __name__ == "__main__":
    main()
