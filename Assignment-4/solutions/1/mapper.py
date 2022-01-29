#!/usr/bin/env python3
"""mapper.py"""

import sys

# input comes from STDIN (standard input)
for line in sys.stdin:
    # remove leading and trailing whitespace
    word = line.strip()
    sorted_word = "".join(sorted(word))
    sorted_camel_case_word = sorted_word[:1].upper() + sorted_word[1:]
    print(sorted_camel_case_word, "\t", word)
