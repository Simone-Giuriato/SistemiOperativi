#!/bin/sh

#controllo parametri
#muoviFile num dirSorgente dirDestinazione ext1 ext2...extN

#RICORDO CHE I 2 FILE DEVONO ESSERE ESEGUIBILI, A ME LI CREA SENZA IL DIRITTO DI ESECUZIONE QUINDI FACCIO chmod 777 muoviFile.sh
if test $# -lt 4
    then
    echo "Errore: inserire muoviFile num dirSorgente dirDestinazione ext1 ext2...extN"
    exit 1

fi


num="$1"
shift
dirSor="$1"
shift
dirDest="$1"
shift
#ora avrei tutte le estensioni

case $dirSor in
    /*) ;;
    *)
    echo "Errore: Dir sorgente deve essere nome assoluto di directory"
    exit 2
    ;;
esac

case $dirDest in
    /*) ;;
    *)
    echo "Errore: Dir destinatario deve essere nome assoluto di directory"
    exit 2
    ;;
esac

if test ! -d "$dirSor"

    then  
    echo "Errore: $dirSor e non è una directory"
    exit 3

fi

if test ! -d "$dirDest"

    then  
    echo "Errore: $dirDest e non è una directory"
    exit 3

fi

if test ! -x "$dirSor"

    then
    echo "Errore non hai i permessi di esecuzione per $dirSor"
    exit 4
fi

if test ! -x "$dirDest"

    then
    echo "Errore non hai i permessi di esecuzione per $dirDest"
    exit 4
fi

case $num in
*[!0-9]*) echo "num deve essere un numero"

exit 5;;
esac

# Il case verifica se la variabile $num contiene almeno un carattere che non è una cifra, in qualunque posizione della stringa.
# * (primo): qualsiasi numero di caratteri prima

# [!0-9]: un solo carattere che NON è una cifra (cioè non un numero da 0 a 9)

# * (secondo): qualsiasi numero di caratteri dopo


#controllo estensioni (che non so quante sono )
#ricordo che avevo fatto gli shift quindi ora ho solo estensioni nei parametri passati
for est in $*

do 
    case $est in
    .*) ;;
    *)
    echo "Errore: deve iniziare con . per essere un'estensione"
    exit 6
    ;;
    esac
done

#aggiunta della directory script corrente alla variabile $PATH
PATH=$PATH:`pwd`
export PATH #senza $ perchè non mi interessa il valore...  è per dire ai figli che quello è il percorso

#creazione del file counter.tmp, dove ci salveremo una riga per ciascun file copiato
>/tmp/counter.tmp

#chiamata a ricerca.sh <parametri>
ricerca.sh "$num" "$dirSor" "$dirDest" $*   #$* sono tutte le estensioni che non so quante siano


#stampa del numero di righe del file counter.tmp
echo "Sono stati spostati: `wc -l </tmp/counter.tmp` file"  # < è la ridirezione dell output dove andare a leggere, se non lo mettessi mi stamperebbe anche il nome del file (è per avere solo il numero)

#finito tutto rimuovo file temporaneno di appoggio per contare

rm -f /tmp/counter.tmp
