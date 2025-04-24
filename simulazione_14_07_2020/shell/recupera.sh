#!/bin/sh

#recupera dir recuperati titolo


#CONTROLLO ARGOMENTI

if test $# -ne 3
    then
    echo "Errore inserire 3 parametri: <dir> <dir> <titolo>"
    exit 1

fi

case $1 in
    /*);;
    *)
    echo "Errore: <dir> deve essere un path assoluto"
    exit 2
    ;;
esac


case $2 in
    /*);;
    *)
    echo "Errore:  <recuperati> deve essere un path assoluto"
    exit 2
    ;;
esac

if test ! -d $1
    then
    echo "Errore $1 deve essere una directory"
    exit 3

fi

if test ! -d $2 #mi dice che se directory recuperati NON esistesse, devo crearla:
    then
    mkdir $2

fi

#AGGIORNAMENTO PERCORSO
PATH=$PATH:`pwd`
export PATH


#CREAZIONE FILE
MAX=/tmp/max.tmp
export MAX
echo 0 > $MAX   #mi serve un primo numero per fare i confronti è come inizializzare una variabile/file


MAX_DIR=/tmp/max_dir.tmp
export MAX_DIR
>$MAX_DIR

TOTALE=/tmp/.totale #collegamento file a variabile TOTALE
export TOTALE   #esporto come #TOTALE, posso usarlo nell'altro file con il nome di variabile anziche sempre /tmp/file.tmp..
echo 0  > $TOTALE


#RICORSIONE
recupera_ric.sh "$1" "$2" "$3"

#OUTPUT

echo "La direcotry con più file è `cat $MAX_DIR` con `cat $MAX` file"
echo "Il numero totale di file copiati è: `cat $TOTALE`"

#RIMOZIONE FILE APPOGGIO

rm -f $MAX
rm -f $MAX_DIR