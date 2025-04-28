#!/bin/sh

#ristorante piatto anno

cd "$2"
piatto="$1" #piatto che mi passa l'utente da ricercare

for file in *.log
do 
    if test -f "$file" -a -r "$file"
    then
   #echo "Eseguo grep"
    echo `grep -w "$piatto" "$file" | cut -d , -f3` >> "$LOG"

    if test `cat "$MAX"` -lt `grep -w "$piatto" "$file" | wc -l`
        then
        echo `grep -w "$piatto" "$file" | wc -l` > "$MAX"
        echo "$file" > "$MAX_FILE"  #metto nome del file MMDD.log nel file
        echo `cat "$MAX_FILE" | cut -d. -f1` > "$MAX_FILE"  #indico il . come carattere separataro e prendo il campo 1, ovvero solo MMDD e lo metto dentro al file MAX_FILE (sovrascrivendo eliminando ciò che c'era prima)

    fi
    
    fi


done

for d in *
do
    if test -d "$d" -a -x "$d"
    then
    ristorante_ric.sh  "$1" "$d"

    fi


done

