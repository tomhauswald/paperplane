#!/bin/bash
cat ${1}                       \
 | tr -c "[:alnum:]" " "       \
 | tr "[:upper:]" "[:lower:]"  \
 | xargs                       \
 | tr " " "\n"                 \
 | awk 'length($1) >= 2'       \
 | sort -u
