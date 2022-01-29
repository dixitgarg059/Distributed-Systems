#!/home/ds-m21-user9/HW-4/dixit_venv/bin/python
"""mapper2.py"""
import sys
for line in sys.stdin:
    if not line:
        continue
    words = list(map(int,line.strip().split()))
    print("{} {}".format(words[2],words[0]))