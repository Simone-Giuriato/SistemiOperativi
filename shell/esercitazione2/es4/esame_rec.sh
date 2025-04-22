#!/bin/sh

#esame_rec.sh "$estensione" "$dir" "$dir2" 

estensione=$1
dir1=$2
dir2=$3

#devo cercare in dir1
cd $dir1

#per ogni file con etsensione $estensione

for i in *$estensione   #cerco tutti i file con quella estensione
do 
    if test -f $i -a ! -f "dir2/$i"         #se è un file AND questo file non c'è nella dir2--> allora copio
    then
    cp $i $dir2/$i    #bastava anche solo mettere $dir2 ma cos' funziona anche se non esiste cartella
    #inserisco info avvenuta copia nel file temporaneo
    echo $i >> /tmp/copiati     #aggiunto nome del file da copiare in dir2 in file temporaneo cosi da poter contare successivamente le righe, ogni riga è un file, ridirezionando in append l output dell echo da terminale al temporaneo

    fi

done 


#ricerca ricorsiva

for i in *
do

    if test -d $i -a -x $i          #controllo che se è direcorty (gli argomenti) io abbia permesso esecuzione
        then
        eseme_rec.sh "$estensione" "$dir1" "$dir2"
    fi

done


# cp non ha bisgno delle bacquotes qui perchè non utilizzo il suo output
#METTERE SEMPRE I PERMESSI DI ESECUZIONE DEI FILE CON chmod 777 su entrmabi se no la ricorsione potrebbe dare problemi