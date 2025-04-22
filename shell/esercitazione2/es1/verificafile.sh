#!bin/sh

#verifico utente non abbia inserito fine
#chiedo:
echo "Scrivi il nome del file  (inserisci 'fine' per terminare)"
read nome   #non devo dichiarare la var prima,  e nell'assegnamento non devo mettere $

while test "$nome" != fine
do



#testo
if test -f "$nome"      #-f controlla sia un file
    then
        echo $nome è un file
    else
        if test -d "$nome"  #-d controlla sia directory
            then
            echo "$nome è un direttorio"


            else
            echo $nome Non esiste

        fi

fi

#richiedo:
echo "Scrivi il nome del file  (inserisci 'fine' per terminare)"
read nome


done


#Buona abitudine mettere le " " agli estremi delle variabili, ma funzione lo stesso
#Buona abitudine mettere le "" negli echo, ma funziona lo stess in teora