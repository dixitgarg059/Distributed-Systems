#!/home/ds-m21-user9/HW-4/dixit_venv/bin/python
"""mapper.py"""
import sys
for line in sys.stdin:
    if not line:
        continue
    words = list(map(int,line.strip().split()))
    u,v,col_u,col_v =[-1,-1,-1,-1]
    if len(words) == 2:
        u,v = words
        col_u,col_v = words
    else:
        u,v,col_u,col_v = words
    col_u,col_v = [min(col_u,col_v),min(col_u,col_v)]
    print("{} {} {} {}".format(u,v,col_u,col_v))
    print("{} {} {} {}".format(v,u,col_v,col_u))
