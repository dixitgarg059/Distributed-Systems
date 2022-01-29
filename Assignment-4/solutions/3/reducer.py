#!/home/ds-m21-user9/HW-4/dixit_venv/bin/python
"""reducer.py"""
from operator import itemgetter
import sys
edges=[]
colors={}
for line in sys.stdin:
    if not line:
        continue
    u,v,color_u,color_v = list(map(int,line.strip().split()))
    if not u in colors or colors[u] > color_u:
        colors[u] = color_u
    if not v in colors or colors[v] > color_v:
        colors[v] = color_v
    edges.append((u,v))
edges = list(set(edges))
for u,v in edges:
    print("{} {} {} {}".format(u,v,colors[u],colors[v]))