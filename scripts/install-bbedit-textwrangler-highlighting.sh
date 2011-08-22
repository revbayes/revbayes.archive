#!/usr/bin/env bash

./generate-rev-plist.py

mkdir ~/Library/Application\ Support/TextWrangler/Language\ Modules > /dev/null 2>&1
cp Rev.plist ~/Library/Application\ Support/TextWrangler/Language\ Modules/Rev.plist > /dev/null 2>&1

mkdir ~/Library/Application\ Support/BBEdit/Language\ Modules > /dev/null 2>&1
cp Rev.plist ~/Library/Application\ Support/BBEdit/Language\ Modules/Rev.plist > /dev/null 2>&1

