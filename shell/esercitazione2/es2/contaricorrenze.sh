#!/bin/sh

#verifico ci siano almeno 2 argomenti ($# non conta $0 il nome del file comandi, quindi in questo caso ho messo 2 perchè uno è il file da guardare e il secondo è almeno una parola da cercare)

if test $# -lt 2
    then
    echo "Numero di argomenti insufficiente"
    exit 1

fi

file=$1 #salvo il nome del file in una varibile, ricordo che file sta subendo un assegnamento e non vuole $ 
#controllo se primo parametri $1 è file

if test ! -f $1     #diverso da file

    then
    echo "Errore: Inserire come primo parametro un file"
    exit 2

fi


# $* è l'insieme di tutte le variabili che corrispondono agli argomento "arg1 arg2 arg3"
shift   #sposto a sinistra gli argomenti andando in questo caso a nascondere il file dove fare ricerca ($1 è la prima parola, $0 rimane il filecomandi)--> fondamentale salvare precedentemente il nome del file in una variabile
for i in $*

do

contatore=`grep -c "$i" "$file"`

echo "Parametro:$i   - Riccorrenze: $contatore"

done



#se non mettessi shift: (sposta argomenti verso sinistra)

#simone@simone-VirtualBox:~/Scrivania/esercitazione2/es2$ sh contaricorrenze.sh file evviva
#Parametro:file   - Riccorrenze: 0
#Parametro:evviva   - Riccorrenze: 3


#mi fa la grep anche per l argomento 1 che è il file, ma io non voglio che cerchi la parola file, quindi con shift sposto gli argomenti verso sinistra (andando a nascondere il primo argomento che è il file dove cercare)
# per questo ho dovuto precedentemente salvare nella varibile file il nome file

#prima e dopo lo shift $1 assume valore diversi prima è il file, dopo è il secondo argomento


# con S* indico argomento1 argomento2..., quindi io faccio per i=argomento1 (la parola da cercare) fai la grep, questo per ogni argomento che si indica con $*
# con $# indico il numero argomenti