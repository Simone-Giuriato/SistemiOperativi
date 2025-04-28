#!/bin/sh

#cinema genere anno

#CONTROLLO ARGOMENTI
if test $# -ne 2
    then
    echo "Errore il numero dei parametri deve essere 2"
    exit 1
fi

case "$2" in
    /*)
    echo "Errore $2 deve avere path relativo"
    exit 2 ;;
    *) ;;
esac

if test ! -d "$2"
    then 
    echo "Errore "$2" deve essere una directory"
    exit 3;
fi

case "$2" in
    *[!0-9]*)
    echo "Errore "$2" deve contenere solo numeri"
    exit 4;;
    *) ;;

esac

#AGGIORNAMENTO PATH
PATH=$PATH:`pwd`
export PATH

#CREAZIONE FILE E FILE APPOGGIO
>$HOME/riepilogo.log
RIEPILOGO=$HOME/riepilogo.log
export RIEPILOGO

#file appoggio
>/tmp/max.tmp
MAX=/tmp/max.tmp
echo 0 > "$MAX"
export MAX

>/tmp/max_titolo.tmp
TITOLO=/tmp/max_titolo.tmp
echo "Non sono presenti film" > "$TITOLO"
export TITOLO

#RICORSIONE
cinema_ric.sh "$1" "$2"


#OUTPUT (da completare questa logica qua per trovare il film con più biglietti venduti, in cinema_ric devi tirarli fuori come? mi dava problemi con un secondo for dentro)
#echo "Il titolo con più biglietti venduti è: `cat "$TITOLO"` con `cat "$MAX"` biglietti"

#RIMOZIONE FILE
rm -f "$TITOLO"
rm -f "$MAX"