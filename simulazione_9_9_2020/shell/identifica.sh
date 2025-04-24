#!/bin/sh

#Identifica dir messaggio

#CONTROLO PARAMETRI

if test $# -ne 2
    then
    echo "Errore, servono 2 parametri: <dir> <messaggio>"
    exit 1;

fi

case $1 in  #mi richiede che dir abbia path relativo
    /*)
    echo "Errore $1 deve avere un path relativo"
    exit 2;;
    *);;

esac

if test ! -d $1
    then
    echo "Errore $1 deve essere una direcotry"
    exit 3

fi

if test ! -x $1
    then
    echo "Errore permessi d'esecuzione mancanti"
    exit 4

fi



#AGGIORNAMENTO PERCORSO

PATH=$PATH:`pwd`
export PATH

#CREAZIONE FILE D'APPOGGIO

# elimino contenuto dei file
> "$1/Trovati"
> "$1/Max"
# la directory dir e' relativa alla directory corrente

export TROVATI="`pwd`/$1/Trovati"
export MAX="`pwd`/$1/Max"

MAX_HIT=/tmp/max_hit.tmp
echo 0 > "$MAX_HIT"
export MAX_HIT


#RICORSIONE

identifica_ric.sh "$1" "$2"


#OUTPUT
echo "I 5 file con più occorrenze sono: 
`sort -r -n "$TROVATI" | cut -f 2 -d , | head -n 5`"
#RIMOZIONE FILE D'APPOGGIO
rm -f $MAX_HIT