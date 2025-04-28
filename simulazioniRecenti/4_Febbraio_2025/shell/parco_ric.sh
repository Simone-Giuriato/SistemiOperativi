#!/bin/sh

#parco specie anno(relativo)

#entro nella dir
cd "$2"

for file in *.txt
do 
    if test -f $file -a -r $file
    then
   # echo "eseuo grep "$file""
    echo `grep -w "$1" "$file" | cut -d, -f2,4,5` >> "$OSSERVAZIONI"

    if test `cat "$MAX"` -lt `grep -w "$1" "$file" | wc -l`
    then
    echo `grep -w "$1" "$file" | wc -l` > "$MAX"
    echo "$file" > "$MAX_FILE"  #mettto mese.txt in MAX_FILE
    echo `cat "$MAX_FILE" | cut -d. -f1` > "$MAX_FILE"  #uso il '.' come separatori di fields, e prendo il primo campo ovvero il nome del mese
    fi


    fi

done

for d in *
do 

if test -d "$d" -a -x "$d"
then
    "$0" "$1" "$d"


fi

done

