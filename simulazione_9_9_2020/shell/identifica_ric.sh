#!/bin/sh

#identifica dir messaggio
cd $1
messaggio=$2

for file in *
do 
    if test -f $file -a -r $file
        then
        occorrenze=`grep -w $messaggio "$file" | wc -l`   #cerco se compare messaggio nel fle, mando a wc che conta le righe
        if test "$occorrenze" -gt 0
            then
            echo "$occorrenze", "`pwd`/$file" >> $TROVATI
            fi
    
        if test `cat "$MAX_HIT"` -lt $occorrenze    # controllo del massimo

            then 
            echo `pwd`/"$file" > "$MAX" #scrivo nela file MAX il nome assoluto del file
            echo "$occorrenze" > "$MAX_HIT" #aggiorno il file con le riccorrenze massime trovate

        fi
        
    fi
  
done

#ricorsione


for i in *

do
    if test -d "$i" -a -x "$i"

        then 
        identifica_ric.sh "$i" "$2"

    fi

done

