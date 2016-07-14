#!/bin/sh

find src -iname "*.cpp" -o -iname "*.h" > files.lst
uncrustify -c superterm.cfg --replace --no-backup -F files.lst
rm -f files.lst

# EOF <stefan@scheler.com>

