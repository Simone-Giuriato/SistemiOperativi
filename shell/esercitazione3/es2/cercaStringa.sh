#!/bin/sh
#cercaFile stringa dir num

#mi sposto nella dir
cd $2

#creo file tmp che fa da contatore...
>/tmp/.counter.tmp

for file in *.txt       #per ogni file con estensione .txt--> * sta per qualsiasi cosa.txt
do 

    if test -r "$file" -a `grep -c "$1" "$file"` -ge "$3"
    then
    #scrivo una riga nel file counter, sarà sempre in append >>

    echo $file >> /tmp/.counter.tmp

    fi

done

if test `wc -l < /tmp/.counter.tmp` -gt `wc -l < /tmp/.max_counter.tmp`
then

cp /tmp/.counter.tmp /tmp/.max_counter.tmp

echo `pwd` > /tmp/.max_dirname.tmp  #il percorso lo butto in max_dirname


fi

rm -f /tmp/.counter.tmp


#ricorsione

for i in *
do
    if test -d "$i" -a -x "$i"

    then
    cercaStringa.sh "$1" `pwd`/"$i" "$3"        #non devo passare la dir di prima (sarebbe sempre queloo), ma il percorso della subdir presente in dir

    fi

done



