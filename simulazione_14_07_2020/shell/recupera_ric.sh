#!/bin/sh

#recupera dir recuperati titolo

cd "$1" #mi sposto in dir per poter cercare i file con i vincoli richiesti
titolo=$3

COUNTER=0 #variabile contatore per contare i file pongo inizialmente =0


for file in *.bak
do
    if test -f $file -a -r $file 
        then
        if test `head -n 1 $file | grep -c $titolo` -gt 0 #se nella prima riga ho una corrispondenza con titolo, #usando pipe, non devo mettere nella grep dove cercare perchè cerca sulla pipe, l'output che ha restituito la head
            then
            cp $file "$2"
            COUNTER=`expr "$COUNTER" + 1`   #incremneto numero file contenuti da quella sub-directory
            
        fi


    fi

done

#faccio confronto per sapere quale dir ha più file da copiare
if test `cat $MAX` -lt "$COUNTER"
    then
    echo "$COUNTER" > $MAX
    echo `pwd`> $MAX_DIR

fi



#DEVO espolicitare il totale dei file copiati (tra tutte le dir)
TOT=`cat $TOTALE`   #per non creare casino mi appoggio ad una variabile (farò una sorta di swap di variabile) 
echo `expr $TOT + $COUNTER` > $TOTALE #sommo quello che ho fin'ora con Conuter che poi si azzereà e lo mando al file collegato a $TOTALE




#ricorsione

for i in *
do
    if test -d "$i" -a -x "$i"
        then
        recupera_ric.sh `pwd`/$i "$2" "$3"  #mettere `pwd`/$i o solo $i NON cambia in questo caso, perche mi sposto sempre dentro la directory dir, se fossi in una directory esterna dovrei mettere path assoluto con pwd

    fi



done