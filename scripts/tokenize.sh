cat ${1}                                       \
 | sed s/Ä/Ae/g                                \
 | sed s/ä/ae/g                                \
 | sed s/Ö/Oe/g                                \
 | sed s/ö/oe/g                                \
 | sed s/Ü/Ue/g                                \
 | sed s/ü/ue/g                                \
 | sed s/ß/ss/g                                \
 | tr "[:upper:]" "[:lower:]"                  \
 | tr -c "[:alnum:]" " "                       \
 | xargs                                       \
 | tr " " "\n"                                 \
 | awk 'length($1) >= 3'                       \
 | sort                                        \
 | grep -vxFf "$(dirname ${0})/blacklist.txt"  \
 | uniq -c                                     \
 | sort -srn
