#!/bin/sh

#memorizzo i parametri passati in variabili

num="$1"
shift
dirSor="$1"
shift
dirDest="$1"
shift
#ora avrei tutte le estensioni

#entro nella cartella sorgente
cd "$dirSor"

#per ogni estensione
for est in $*
do
    #per ogni file con quell'estensione
    for file in *"$est"
    do
        if test -f "$file" -a -r "$file"   #se è un file e se ha diritti lettura
        then

        #se numero righe >= num  allora: scrittura di una riga nel file counter.tmp  (sarebbe # wc -l < "$file")
            if test `cat "$file" | wc -l` -ge $num   #andava bene anche # wc -l < "$file" voglio solo il numero, -ge è >=
            then

            cp "$file" "$dirDest/$file"
            echo `pwd`/"$file" >> /tmp/counter.tmp  #scrivo percorso sul file tmp
            

            fi

        fi

    

    done

done


#ricorsione: per ogni directory
    # se si tratta di una directory e l'utente ha i diritti di esecuzione
    #chiamata a ricerca.sh <parametri> passando la directory corrente

for d in *

do

    if test -d "$d" -a -x "$d"
    then
    ricerca.sh "$num" `pwd`/"$d" "$dirDest" $*
    fi


done


#La ricorsione serve a:

    #Entrare dentro ogni sottocartella

    #Ripetere lo stesso processo di controllo su tutti i file dentro quelle sottocartelle

   # Copiare i file validi nella destinazione

    #Salvare il percorso nel file /tmp/counter.tmp


#Se non fai ricorsione, il tuo script vedrà solo i file nella cartella principale (main.c, utils.c) e ignorerà tutto ciò che è dentro subdir1, subdir2, ecc.