#!/bin/sh

#azienda veicolo anno

#CONTROLLO ARGOMENTI

if test $# -ne 2
    then 
    echo "Errore: numero parametri deve essere 2"
    exit 1
fi

case $2 in
    /*)
    echo "Errore: <anno> deve essere un percorso relativo"
    exit 2;;
    *);;
esac

if test ! -d "$2"
    then 
    echo "Errore: $2 deve essere una directroy"
    exit 3
fi

case $2 in
    *[!0-9]*)
    echo "Errore: <anno> deve essere composta solo da caratteri numerici"
    exit 4
    ;;
    *);;
esac



#AGGIORNAMENTO PERCORSO
PATH=$PATH:`pwd`
export PATH

#FILE E FILE APPOGGIO

>$HOME/output.txt
OUTPUT=$HOME/output.txt
export OUTPUT

>/tmp/max.tmp
MAX=/tmp/max.tmp
echo 0 >"$MAX"
export MAX

>/tmp/max_file.tmp
MAX_FILE=/tmp/max_file.tmp
echo "" > "$MAX_FILE"
export MAX_FILE

#RICORSIONE
azienda_ric.sh "$1" "$2"


#OUTPUT
echo "Il file con più viaggi è: `cat $MAX_FILE`"

#RIMOZIONE FILE APPOGGIO
rm -f "$MAX"
rm -f "$MAX_FILE"
