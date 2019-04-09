#!/usr/bin/env python
from __future__ import print_function
import sys

if len(sys.argv) == 4:
    base_macro = sys.argv[1]
    range_max = int(sys.argv[2])
    mode = sys.argv[3]
else:
    base_macro = "F"
    range_max = 16
    mode = "undef"

if (mode == "undef"):
    undefstr = "#undef "
    for i in range(0, range_max):
        if (i < 10):
            print(undefstr + base_macro + "0" + str(i))
        else:
            print(undefstr + base_macro + str(i))
elif (mode == "ifdef"):
    ifstr = "#ifdef "
    undefstr = "# undef "
    endifstr = "#endif"
    for i in range(0, range_max):
        if (i < 10):
            print(ifstr + base_macro + "0" + str(i))
            print(undefstr + base_macro + "0" + str(i))
        else:
            print(ifstr + base_macro + str(i))
            print(undefstr + base_macro + str(i))
        print(endifstr)

