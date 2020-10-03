#!/bin/sh

find src -iname "*.cpp" -o -iname "*.h" -exec clang-format-6.0 -i -style=file {} \;

