#!/bin/sh

#parco specie anno(relativo)

#CONTROLLO ARGOMENTI
if test $# -ne 2
then 
echo "Errore: il numero di parametri deve essere 2"
exit 1
fi

case $2 in
    /*)
    echo "Errore il path di "$2" deve essere relativo"
    exit 2
    ;;
    *);;
esac

if test ! -d "$2"
then
echo "Errore "$2" deve essere una direcotry"
fi

case "$2" in
    *[!0-9]*) 
    echo "La direcorty deve essere composta da solo numeri"
    exit 3 
    ;;
    *);;

esac


#AGGIORNAMENTO PATH
PATH=$PATH:`pwd`
export PATH


#FILE E FILE APPOGGIO
OSSERVAZIONI=$HOME/osservazioni.txt
>"$OSSERVAZIONI"    #sovrascrivo ogni volta
export OSSERVAZIONI

MAX=/tmp/max.tmp
>"$MAX"
echo 0 > "$MAX"
export MAX

MAX_FILE=/tmp/max_file.tmp
>"$MAX_FILE"
echo "" >"$MAX_FILE"
export MAX_FILE



#RICORSIONE
parco_ric.sh "$1" "$2"


#OUTPUT
echo "Il mese con più osservazioni della specie "$1" è: `cat "$MAX_FILE"` con `cat "$MAX"` osservazioni"

#RIMOZIONE FILE
rm -f "$MAX"
rm -f "$MAX_FILE"