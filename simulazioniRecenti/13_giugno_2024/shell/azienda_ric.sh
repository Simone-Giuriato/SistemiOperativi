#!/bin/sh


#azienda veicolo anno

cd "$2"   #mi sposto in dir anno o sub_dir

COUNTER=0
for file in *.txt
do 
    if test -f "$file" -a -r "$file"        #se è un file e posso leggerlo
    then 
    #echo "Esecuzione di grep sul file: $file"
    echo `grep -w "$1" "$file" | cut -d , -f2,4,5` >> "$OUTPUT"
    

        if test `wc -l < "$file"` -gt `cat "$MAX"`

            then
            echo "$file" > "$MAX_FILE"
            echo `wc -l < "$file"` > "$MAX"

        fi
    fi

done

#ricorsione

for d in *
do
if test -d "$d" -a -x "$d"
then
    azienda_ric.sh "$1" "$d"

fi

done


#`wc -l < "$file"` LA WC -L RITORNA NUMERO RIGHE E NOME FILE
# COSÌ HO SOLO IL NUMERO RIGHE, PERCHÈ NON SA DA DOVE VIENE LE INFO È ANONIMO, quindi mette solomle righe
