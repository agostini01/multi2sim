#!/usr/bin/env python2.7

ACCESS_TYPE = 2
MODULES = [1, 2]
MIN_ADDR = 0x0000
MAX_ADDR = 0x4000
SEPARATED_BY = 32
DELAY = 1

TRACE_FILE = "sample-trace-2.txt"


def main ():
    with open(TRACE_FILE, "w+") as trace_file:
        for addr in xrange(MIN_ADDR, MAX_ADDR, SEPARATED_BY):
            for mod in MODULES:
                inp = [ format(DELAY, "+"), str(mod), str(ACCESS_TYPE), format(addr, "X") ]
                trace_file.write(" ".join(inp) + "\n")

if __name__ == "__main__":
    main()
