#!/bin/sh

#$ sh trova.sh dir file
#controllo passi almeno 2 parametri (nome file comandi escluso)
#passo nome assoluto di una direcorty e un nome di un file, mi dirà se c'è quel file il percorso assoluto

if test $# -ne 2    #numero argomenti not equal (potevo anche !=) di 2:
then
    echo "Errore: Il numero dei parametri deve essere 2"
    exit 1


fi

#controllo mi passi un file e una cartella, la cartella deve essere assoluta

case $1 in
    /*) ;;    # Se il primo argomento è una directory assoluta, non fai nulla
    *)          
    echo "Errore il primo argomento deve essere una directory assoluta"
    exit 2
    ;;

esac


#   *): Controlla se il primo argomento (in $1) inizia con una barra (/), che è tipico delle directory assolute.

#   *: Se non è una directory assoluta, esegue l'azione di errore, ovvero l'echo e exit.

if test ! -d "$1"
then
     echo "Errore: il primo parametro deve essere una directory"
     exit 3

fi





#RICERCA
file=$2
dir=$1
#Aggiungo la directory corrente al PATH → serve a trovare il comando trova.sh anche in ricorsione.
PATH=$PATH:`pwd`    #aggiunge direcotry script corrente al $PATH
export PATH #esporta il PATH
#queste 2 righe sempre così





if test -x "$dir"   #evita loop se non ho permessi di eseguire una direcotry
then 
cd $dir     #mi sposto nella direcotry passata con nome assoluto
for i in *  # * sta Per ogni elemento nella directory corrente, fai qualcosa.

do 

    if test -d "$i"     #Se $i è una directory (una direcorty dentro a dir che gli avevo passato) → richiama ricorsivamente lo script su quella sottodirectory.
        then 
        
        #ricorsione
        trova.sh `pwd`/"$i" "$file" #ricorione eseguo trova.sh passando come parametro il percorso assoluto della dir e secondo parametro il file, passo ad 

    else

        if test -f "$i" -a "$i" = "$file"
            then
            echo `pwd`/$i
        fi

    fi



done

fi



# * sta per  Per ogni elemento nella directory corrente, fai qualcosa.
# $* Rappresenta l'insieme degli argomenti da riga di comando (cioè quelli che lanci tu da terminale quando esegui lo script).

#L'asterisco * è un metacarattere (wildcard) che si espande a tutti i file e cartelle nella directory corrente.

