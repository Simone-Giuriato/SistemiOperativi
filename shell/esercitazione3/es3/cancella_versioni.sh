#/bin/sh

#cancella_versioni_preliminari dir_articoli

#controllo argomenti

if test $# -ne 1
    then 
    echo "Errore: serve solo 1 parametro"
    exit 1
fi

#dir percorsor relativo:

case $1 in
    */) 
    echo "Errore: deve essere un nome relativo di directory"
    exit 2;;
    *);;
esac

if test ! -d $1
    then 
    echo "Errore: $1 deve essere una directory"
    exit 3

fi

if test ! -x $1
    then
    echo "Errore: non hai i permessi di esecuzione in $1"
    exit 4
fi

#aggiunta della directory script corrente alla variabile $PATH
PATH=$PATH:`pwd`
export PATH

#creazione dei file max_counter.tmp (con valore 0)
echo '0' > /tmp/max_counter.tmp

#creazione max_dirname.tmp
>/tmp/max_dirname.tmp


#chiamata a cancella_versioni_preliminari_aux.sh <parametri> 
cancella_versioni_preliminari_aux.sh `pwd`/"$1"  #perche $1 è relativo quindi ci aggiungo tutto il percorso con pw


#stampa risultato

echo "La directory che conteneva il maggior numero di file cancellati è: `cat /tmp/max_dirname.tmp` "


#rimuovo file tmp

rm -f /tmp/max_dirname
rm -f /tmp/max_counter
