#! /bin/sh

#RICORDO DI AVERE I DIRITTI DI ESECUZIONE chmod 777 cancella_versioni_preliminari_aux

cd "$1"

counter=0

for i in *.txt
do   

    if test -f $i -a -w $i 
        then
        if test `cut -d , -f 3 "$i"  | grep -c "preliminare"` -gt 0 #se è più grende di 0, ha trovato almeno 1 preliminare vuol dire che è da eliminare
            then
            echo "`pwd`/$i è preliminare" #è preliminare allora rimuovo
            rm -f "$i"
            counter=`expr $counter + 1` #incremento il valore di 1 con comando expr faccio operazioni arimetiche
            else
            echo "`pwd`/$i è definitivo"
        fi

    fi

done 

# controllo del massimo utilizzando la variabile counter (numero)
#se il valore di counter > del valore contenuto in max_counter.tmp aggiornamento di max_counter.tmp e max_dirname.tmp

if test "$counter" -gt `cat /tmp/max_counter`
    then
    echo $counter > /tmp/max_counter
    echo `pwd` > /tmp/max_dirname
fi


# lancio la ricorsione

for i in *
do
    if test  -d $i -a -x $i

    then
    cancella_versioni_preliminari_aux.sh "$i"   #accetta nomi relativi

    fi
done