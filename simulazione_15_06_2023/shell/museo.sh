#!/bin/sh

#museo esposizione anno

#CONTROLLO ARGOMENTI

if test $# -ne 2
    then 
    echo "Errore:servono 2 parametri"
    exit 1

fi

esposizione=$1
shift
anno=$1
shift

#controllo che dir anno sia path relativo

case $anno in
    /*)
    echo "Errore deve essere un path relativo"
    exit 2
    ;;
    *);;
esac

#controllo anno sia dirrectory

if test ! -d $anno
    then
    echo "Errore <anno> deve essere una directory"
    exit 3
fi

#controllo permessi esecuzione su anno

if test ! -x $anno
    then
    echo "Errore devi avere i permessi di esecuzione sulla directory <anno>"
    exit 4
fi

#AGGIORNO PATH

PATH=$PATH:`pwd`
export PATH

# esporto il file di riepilogo delle opere, facendo così posso usare $OPERE come simbolo di quel file
OPERE=$HOME/esposizione.txt
export OPERE

# creo il file di riepilogo delle opere
echo "" > $OPERE

# creo il file per tener traccia del massimo numero di opere (mettere 0 o "", perchè se devo fare confronto come lo faccio devo avere qualcoaa con cui confrontare)
echo "0" >>/tmp/max.tmp
echo "" >/tmp/file_max.tmp


#LANCIO LA RICORSIONE

museo_ric.sh "$esposizione" "$anno"


#output
echo "Numero massimo di esposizioni nel mese di: `cat /tmp/file_max.tmp`"

#rimuovo file temp

rm -f /tmp/.file_max.tmp
rm -f /tmp/max.tmp
