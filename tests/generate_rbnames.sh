#!/usr/bin/env bash

grep -orE "const std::string .*_name.*=.*\".*\";" ../src/* | cut -d ":" -f 2,3,4 | sort | uniq