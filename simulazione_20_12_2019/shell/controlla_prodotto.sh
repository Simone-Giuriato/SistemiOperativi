#!/bin/sh

#controlla_prodotto dir nome_prodotto

#CONTROLLO ARGOMENTI

if test $# -ne 2
    then
    echo "ERRORE: il numero dei parametri deve essere 2: Usage <dir> <nome_prodotto>"
    exit 1

fi

case $1 in
    /*) ;;
    *)
    echo "Errore: la directory deve avere un path assoluto"
    exit 2
    ;;
esac

if test ! -d $1
    then
    echo "Errore $1 deve essere una directory"
    exit 3

fi


#AGGIORNAMENTO PERCORSO

PATH=$PATH:`pwd`
export PATH


#CREAZIONE FILE E FILE D'APPOGGIO
INVENTARIO_2019=$HOME/inventario_2019.txt
export INVENTARIO_2019

#file dove memorizzo la quantità inventariata
>/tmp/qta_inventariata.tmp      #Crea un file vuoto (o svuota se esiste già) 
QTA_INVENTARIATA=/tmp/qta_inventariata.tmp  # Assegna il path del file alla variabile QTA_INVENTARIATA
echo 0 > $QTA_INVENTARIATA  #scrivoil numero 0 nel file, che userò come valore iniziale (es. per confornti o somme).
export QTA_INVENTARIATA #Rende la variabile visibile ai sottoprocessi o script chiamati da questo. Nel file controlla_prodotto_ric..sh posso richiamrlo come variabile

>/tmp/max.tmp
MAX=/tmp/max.tmp
echo 0 > "$MAX"
export MAX

>/tmp/max_magazzino.tmp
MAX_MAGAZZINO=/tmp/max_magazzino.tmp
export MAX_MAGAZZINO


#RICORSIONE

controlla_prodotto_ric.sh "$1" "$2"


#OUTPUT
echo "La quantità totale immaggazzinata è: `cat "$QTA_INVENTARIATA"`"
echo "$2",`cat "$QTA_INVENTARIATA"` >> "$INVENTARIO_2019"
echo "Magazzino con maggior quantità di $2: `cat "$MAX_MAGAZZINO"`"


#RIMOZIONE FILE D'APPOGGIO

rm -f "$QTA_INVENTARIATA"
rm -f "$MAX"
rm -f "$MAX_MAGAZZINO"