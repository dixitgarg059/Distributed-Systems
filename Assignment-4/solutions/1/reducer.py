#!/usr/bin/env python3
"""reducer.py"""

from operator import itemgetter
import sys

current_word = None
word = None
lt = []


def PrintOutput():
    if not len(lt):
        return
    print("{}: ".format(current_word), end="")
    for i in range(0, len(lt)-1):
        print("{}, ".format(lt[i]), end="")
    print(lt[-1])


for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()
    sorted_camel_case_word, word = line.split()
    if sorted_camel_case_word == current_word:
        lt.append(word)
    else:
        PrintOutput()
        current_word = sorted_camel_case_word
        lt = [word]
PrintOutput()
