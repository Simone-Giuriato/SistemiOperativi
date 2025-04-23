#!/bin/sh

#museo esposizione anno

esposizione=$1
shift
anno=$1
shift

#entro in dir anno
cd $anno

#passo ogni file txt in dir anno

for i in *.txt
do 
#controllo che il file sia un file e abbia permessi di lettura
    if test -f $i -a -r $i
    then
        #cerco le info solo per l'esposzioni che mi ha passato l'utente
        grep -w $esposizione $i | cut -d , -f1,3 >> $OPERE  #non usando il valore, non hi bisogno delle backuquotes
        # aggiorno il massimo
        val=`wc -l < $i`   #conta le righe del file mese.txt, se è più grande del valore precedente in file massimo sarà il nuovo massimo di esposizioni
        if test "$val" -gt `cat /tmp/max.tmp`
        then
        #valore massimo lo scrivo su u  file
        echo $val>/tmp/max.tmp

        #nome del file lo metto su un file maxfile
        echo "$i" > /tmp/file_max.tmp

        fi


    fi


done


#RICORSIONE

for d in *
do
    if test -d "$d" -a -x "$d"      #controllo sia dir e abbia permessi esecuzione, per evitare loop
    then

    museo_ric.sh "$esposizione" "$d"

    fi


done



#Il comando cut in Unix/Linux serve per estrarre parti specifiche di ogni riga di un file o di uno stream di testo.
#-d	Imposta il delimitatore (es: , o :)
#-f	Specifica i campi (colonne) da prendere (colonne tutte attaccate)
#-c	Estrae caratteri specifici per posizione