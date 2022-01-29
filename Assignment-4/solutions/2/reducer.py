#!/home/ds-m21-user9/HW-4/dixit_venv/bin/python
"""reducer.py"""
import sys
dic = {}
for line in sys.stdin:
    if not line:
        continue
    try:
        v,u,flg = line.strip().split(',')
    except:
        sys.stderr.write("Error:"+line)
    flg = int(flg)
    if v not in dic:
        dic[v] = []
    dic[v].append((u,flg))

for v in dic:
    # print(dic)
    # continue
    lt = dic[v]
    if not len(lt):
        continue
    flg_first = 0
    for u,flg in lt:
        if flg == 1:
            flg_first = 1
            break
    if flg_first ==0:
        with open("/home/ds-m21-user9/HW-4/2/N.txt","a") as fp:
                fp.write("\n")
    u_first,_ = lt[0]
    print("{},{},{}".format(v,u_first,flg_first))
    for u,flg in lt[1::]:
        print("{},{},{}".format(v,u,1))
    