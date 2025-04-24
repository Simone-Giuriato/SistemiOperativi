#!/bin/sh

#trova_script dir

#CONTROLLO ARGOMENTI

if test $# -ne 1
    then
    echo "Errore: devi inserire un parametro, USO: $0 <dir>"
    exit 1

fi

case $1 in
    /*);;
    *)
    echo "Errore:: devi dare un path assoluto di <dir>"
    exit 2;;
esac

if test ! -d $1 
    then 
    echo "Errore devi dare una diretory"
    exit 3

fi



#AGGIORNO PATH
PATH=$PATH:`pwd`
export PATH


#FILE

#script.txt nella home utente (se non c'è andrà creato)
LIST_FILE=$HOME/script.txt
export LIST_FILE

rm -f $LIST_FILE    #se c'è lo rimuovo perchè se no mi darebbe in append i risultati dei debug precedenti, rimuovendolo ogni volta, ho i risultati senza ripetizioni di esami precedenti
#file appoggio
MAX_DIR=/tmp/.max_dir
export MAX_DIR
>$MAX_DIR            #creo effettivamente il file che contiene il nome della sub-directory con più file

MAX=/tmp/.max
export MAX
echo 0 > $MAX   #metto 0 perchè mi serve per il confronto 


#LANCIO LA RICORSIONE
trova_script_ric.sh "$1"

#output
echo "La directory con più file è: `cat $MAX_DIR` con: `cat $MAX` file"

#RIMUOVO FILE

rm -f $MAX_DIR
rm -f $MAX
