#!bin/sh

#ristorante piatto anno

#CONTROLLO ARGOMENTI
if test $# -ne 2
then
echo "Errore devono essere passati 2 parametri"
exit 1
fi

case $2 in
    /*);;
    *) 
    echo "Errore deve essere un path assoluto di direcotry"
    exit 2
    ;;

esac

if test  ! -d "$2"
then
echo "Errore: $2 deve essere una direcotry"
fi

#AGGIORNAMENTO PATH
PATH=$PATH:`pwd`
export PATH

#FILE E FILE D'APPOGGIO

LOG=$HOME/piatto.log    #creo file nella dir utente
>$LOG   #lo svuoto ogni volta (sovrascrivo sempre)
export LOG

#file d'appoggio
>/tmp/max.tmp
MAX=/tmp/max.tmp
echo 0 > "$MAX"
export MAX

>/tmp/max_file.tmp
MAX_FILE=/tmp/max_file.tmp
echo ^^^non ordinato^^^ > "$MAX_FILE"
export MAX_FILE

#RICORSIONE

ristorante_ric.sh "$1" "$2"


#OUTPUT
echo "Contenuto file di riepilogo: `cat "$LOG"`"

#il giorno (ovverosia il nome del file corrispondente) in cui il piatto di interesse è stato ordinato più volte.
echo "Il giorno in cui $1 è stato ordinato più volte è: `cat "$MAX_FILE"` con `cat "$MAX"` VOLTE"

#RIMOZIONE FILE
rm -f "$MAX"
rm -f "$MAX_FILE"