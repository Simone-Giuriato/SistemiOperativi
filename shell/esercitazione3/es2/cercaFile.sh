#!/bin/sh

#cercaFile stringa dir num

#controllo Argomenti

if test $# -ne 3

    then
    echo "Errore: Devono essere 3 parametri: #cercaFile stringa dir num"
    exit 1
fi

stringa=$1
dir=$2
num=$3

if test ! -d $dir

    then
    echo "Errore: $dir Non è una directory"
    exit 2

fi

if test ! -x $dir
    then
    echo "Errore: Non ho i diritti di accesso a $dir"
    exit 3
fi

case $dir in
    /*) ;;
    *)
    echo "Errore $dir non è un nome assoluto"
    exit 4
    ;;
esac

case $num in 
    *[!0-9]*) 
    echo "Errore $3 non è un numero positivo"
    exit 5;;
esac

# aggiunta della directory script corrente alla variabile $PATH
PATH=$PATH:`pwd`
export PATH

#creazione dei file max_counter.tmp e max_dirname.tmp--> il .file Rende il file "nascosto" (hidden) nel sistema Unix/Linux.
>/tmp/.max_counter.tmp
>/tmp/.max_dirname.tmp

#chiamata a cercastringa.sh <parametri>
cercaStringa.sh "$1" "$2" "$3"

#stampa risultato-->Al termine delle operazioni, il file comandi deve scrivere a video il nome della
#directory che contiene il maggior numero di file che soddisfano le condizioni richieste

#cat (visualizzo contenuto) nome file che contiente il nome della dir, e con wc conto le crice del file counter
echo "La directory con il maggior numero di file è: `cat /tmp/.max_dirname.tmp` con `wc -l < /tmp/.max_counter.tmp` file" #la wc cosi mi restituisce solo il numero e non anche il nome file

#rimuovo file tmp
rm -f /tmp/.max_counter.tmp
rm -f /tmp/.max_dirname.tmp