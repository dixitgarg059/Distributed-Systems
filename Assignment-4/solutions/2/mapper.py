#!/home/ds-m21-user9/HW-4/dixit_venv/bin/python
"""mapper.py"""
import sys


from urllib.parse import urljoin

from bs4 import BeautifulSoup
import requests
import validators
import re
# from django.core.validators import URLValidator
# from django.core.exceptions import ValidationError
# import functools
from itertools import repeat

def formaturl(parent,child):
    url = requests.compat.urljoin(parent,child)
    return url
    # if not re.match('(?:http|ftp|https)://', url):
    #     return 'http://{}'.format(url)
    # return url

# def check(url):
#     return True
#     val = URLValidator(verify_exists=True)
#     try:
#         val(url)
#         return True
#     except:
#         return False
def find_new_neighbour(url):
    page = requests.get(url)
    soup = BeautifulSoup(page.content, 'html.parser')
    urls = list(map(formaturl,repeat(url),list(filter(lambda x:x!='#',[link.get('href') for link in soup.find_all('a')]))))
    urls = list(set(filter(lambda x:x!=url,urls)))
    return urls
    
for line in sys.stdin:
    if not line:
        continue
    words = line.strip().split(',')
    v,u,flg = words
    flg = int(flg)
    print("{},{},{}".format(v,u,1))
    if flg == 1:
        continue
    else:
        v1_list = find_new_neighbour(v)
        for v1 in v1_list:
            with open("/home/ds-m21-user9/HW-4/2/M.txt","a") as fp:
                fp.write("\n")
            print("{},{},{}".format(v1,v,0))