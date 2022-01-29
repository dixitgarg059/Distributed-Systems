#!/home/ds-m21-user9/HW-4/dixit_venv/bin/python
"""reducer2.py"""
import sys
component=[]
current_color = -1
for line in sys.stdin:
    if not line:
        continue
    color_u,u = list(map(int,line.strip().split()))
    if color_u == current_color:
        component.append(u)
    else:
        component = list(set(component))
        for v in component:
            print(v,end=' ')
        if current_color !=-1:
            print()
        component = [u]
        current_color = color_u
component = list(set(component))
for v in component:
    print(v,end=' ')