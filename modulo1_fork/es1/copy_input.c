#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore, inserire 2 argomenti: ./copy_inputc.c <nomefile>\n");
        exit(1);
    }

    int fd; // file descriptor del file passato come argomento

    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644); // APRO: il secondo argomento è il nome file
    // apro il file che ho come secondo parametro: O_WRONLY apri in scritturs, O_CREAT crea se non esiste, O_TRUNC se esiste sovrascrivi cancellando ciò che c'era
    // avendo l O_CREAT devo mettere anche i diritti 0644 qui esempio
    char stringa[1024];
    printf("Inserisci una stringa (fine per uscire):");
    fgets(stringa, 1024, stdin);
    // fgets legge una linea da uno stream (ad esempio dallo standard input stdin)
    // alternativamente posso leggere una stringa senza spazi con scanf scanf("%s", stringa)
    int nwrite = 0; // varibaile che contiene quanti caratteri ha effettivamente scritto (inutile qua)
    while (strcmp(stringa, "fine\n") != 0)
    { // se utente inserisce fine con vai a capo (\n), interrompo

        nwrite = write(fd, stringa, strlen(stringa)); // scrivo la stringa sul file, uso system call write
        // write(dove scrivo (file descriptor), cosa scrivo, lunghezza/grandezza di ciò che scrivo)

        printf("Inserisic un'altra stringa (fine per uscire):");
        fgets(stringa, 1024, stdin);
    }

    close(fd); // non mi serve più il file apertom lo chiudo

    return 0;
}