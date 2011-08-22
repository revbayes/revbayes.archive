#!/usr/bin/env python
#
# Generates Rev.plist language module file for
# syntax highlighting in TextWrangler and BBEdit
#
# Daniel Ayres

import sys
import commands
import re

def main():
    names_cmd = "grep -oE '\( *\".*?\",' ../src/core/workspace/RbRegister.cpp | cut -d '\"' -f 2 | sort | uniq | sed -Ee 's/(.*)/		<string>\\1<\\/string>/'"
    names = commands.getoutput(names_cmd)

    plist_template_file = open('Rev-template.plist', "r")
    plist_file = open('Rev.plist', "w")
    
    for line in plist_template_file:
        if re.search("RevNames", line):
            plist_file.write(names)
        else:
            plist_file.write(line)
    
    
    return 0

if __name__ == '__main__':
    sys.exit(main())


