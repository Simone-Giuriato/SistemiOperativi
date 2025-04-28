#!/bin/sh

#cinema genere anno


cd "$2"     #sposto in dri

genere="$1"
for file in *.txt
do 
    if test -f "$file" -a -r "$file"
    then
    #echo "Eeguo grep in $file"
    echo `grep -w "$genere" "$file" | cut -d , -f1,4` >> "$RIEPILOGO"

   
 
    fi


done


for d in *
do 
    if test -d "$d" -a -x "$d"

    then
    "$0" "$1" "$d"

    fi


done