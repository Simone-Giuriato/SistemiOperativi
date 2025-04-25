#!/bin/sh

#controlla_prodotto dir nome_prodotto
cd "$1" #mi sposto nella dir 

for file in *.txt
do 
    if test -f "$file" -a -r "$file"
    then
    info_prod=`grep -w "$2" "$file"` #estraggo l'info prodotto
        #se è presente  aggiorno
        if test -n $info_prod   #verifico se non è vuota
            then 
            qta=`echo $info_prod | cut -d , -f4`  #estraggo la quantità e la metto i  qta

            qta_magazzinata=`cat "$QTA_INVENTARIATA"`   #metto valore del file fin'ora in una variabile

            #aggiorno la quantità totale
            echo `expr "$qta" + "$qta_magazzinata"` > "$QTA_INVENTARIATA"   #faccio la somma delle 2 variabili, e la butto nel file

            #controllo del massimo
            if test `cat "$MAX"` -lt "$qta" #confronto file max con la quantità attuale
                then
                echo "$qta" > "$MAX"    #aggiorno il file quantità massima inserendoci il valore qta
                echo `pwd`/"$file" > "$MAX_MAGAZZINO"   #aggiorno file MaxMagazzino inserendoci nome del file

            fi


        fi


    fi

   


done


#ricorsione

for i in *
do
if test -d $i -a -x $i
then
controlla_prodotto_ric.sh "$i" "$2"


fi

done