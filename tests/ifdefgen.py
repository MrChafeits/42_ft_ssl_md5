#!/usr/bin/env python3
import sys

ifstr = "#ifdef "
undefstr = "# undef "
endifstr = "#endif"
base_macro = "TH"
for i in range(0,60):
    if (i < 10):
        print(ifstr + base_macro + "0" + str(i))
        print(undefstr + base_macro + "0" + str(i))
    else:
        print(ifstr + base_macro + str(i))
        print(undefstr + base_macro + str(i))
    print(endifstr)

