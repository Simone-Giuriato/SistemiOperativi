#!/bin/sh

#CONTROLLO ARGOMENTI

if test $# -ne 3

    then 
    echo "ERRORE: servono 3 parametri"
    exit 1


fi


#il primo argomento deve essere estensione
estensione=$1

case $estensione in
    .*) ;;  #estrensione--> inizia con punto ok

    *)
    echo "Errore: il primo parametro deve essere un'estensione che inizi con ."
    exit 2
    ;;

esac


#gli altri 2 argomenti devono essere nomi assoluti di directory
dir=$2

case $dir in
    /*) ;;
    *)
    echo "Errore: il secondo parametro deve essere un nome assoluto di directory"
    exit 3
    ;;
esac

if test ! -d $dir
    then
    echo "Errore: il secondo parametro DEVE essere una directory"
    exit 4
fi

dir2=$3

case $dir2 in 
    /*) ;;
    *)
    echo "Errore: il terzo parametro deve essere un nome assoluto di directory"
    exit 5
    ;;
esac

if test ! -d $dir2
    then
    echo "Errore: il terzo parametro DEVE essere una directory"
    exit 6
fi

#AGGIORNAMENTO PATH
PATH=$PATH:`pwd`
export PATH

#Creo file temporaneo di appoggio percontare il numero di file effettivamente copiati
>/tmp/copiati   #> ridirezione, cos' creo file, dove inserirò righe che corrispondono ai file copiati, poi conterò le rigo e saprò numero file
#chiamata ricorsiva
esame_rec.sh "$estensione" "$dir" "$dir2"  #chiamo filecomandi esame_rec passandogli gli argomenti che mi son stati passati

#conto
echo "Copiati `wc -l < /tmp/copiati` files" #conto le righe nel file tmp così so quanti file ho copaiti

#rimuovo file temporaneo
rm -f /tmp/copiati


#METTERE SEMPRE I PERMESSI DI ESECUZIONE DEI FILE CON chmod 777 su entrmabi se no la ricorsione potrebbe dare problemi
