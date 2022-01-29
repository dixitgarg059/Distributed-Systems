#!/home/ds-m21-user9/HW-4/dixit_venv/bin/python
"""mapper.py"""
import sys
for line in sys.stdin:
    if not line:
        continue
    words = line.strip().split(',')
    v,u,flg = words
    if v!=u:
        print("{} {}".format(u,v))