#!/bin/sh

#trova_script dir

cd "$1" #mi sposto nella dir

COUNTER=0  #variabile per contare i file dentro una subdirecotry

for file in *.sh
do
    if test -f $file -a -r "$file" -a -w "$file"    #se  file e leggibile e scrivibile dall utente
    then
        if test `grep -c "#!/bin/bash" "$file"` -gt 0 #conto quante stringhe cos' ci sono, se son gt (maggiore di 0) uindi ne ho faccio cose
        then
        echo `pwd`/$file >> "$LIST_FILE"
        COUNTER=`expr "$COUNTER" + 1 `    #se son qua vuol dire che ho un altro file che rispetta i cirteri, aggiorno il counter per sapereil numero tot di file
        
        fi


    fi

done

if test `cat $MAX` -lt "$COUNTER"
    then
    echo $COUNTER > $MAX
    echo `pwd` > $MAX_DIR

fi


#ricorsione

for i in *
do
    if test -d "$i" -a -x "$i"      #evito loop
    then
    trova_script_ric.sh "$i"

    fi


done