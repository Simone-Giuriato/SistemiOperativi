#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore devono esserci 2 argomenti: anteprima.c <num>");
        exit(1);
    }
    char filename[1024];
    printf("Inserisci il nome del file di cui visualizzare l'anteprima (fine per uscire):");
   scanf("%s", filename);

    int fd;      // file descriptor che la open mi restituirà del file
    int pid = 0; // conterrà pid figlio
    int status = 0; //mi serve per la wait

    while (strcmp(filename, "fine\n")!= 0)
    {

        fd = open(filename, O_RDONLY); // apro in lettura

        if (fd < 0)
        { // se open resitutisce un fd<9, vuol dire che non esiste/errore
            printf("Non esistono file con questo nome: %s", filename);
            exit(2);
        }
        else
        { // se esiste, chiudo il file per non creare leak
            //(tanto la exec non lavora con file aperti, ma usa logica shell)
            close(fd);
        }

        pid = fork();

        if (pid < 0)
        {
            perror("Errore");
            exit(3);
        }
        else if (pid == 0)
        {
            // FIGLIO

            printf("Anteprima del file: %s\n", filename);
            execlp("head", "head", "-n", argv[1], filename, (char *)0);
            // comando head, alla fine di ogni exec mettere terminatore (char*)0 sempre... execlp non ho bisogno di emttere il percorso del comando
            perror("Errore exec:"); //se exec da errore, torna qui e perror scrive nello stderr (collegato in terminale), dicendo codice errore
            exit(4);
        }
        else
        { //(quindi pid>0)
            // PADRE

            wait(&status);  //La funzione wait() è usata nei processi padre per aspettare la terminazione di un processo figlio.
            //Salva lo stato  del figlio di uscita in status
        }

        printf("Inserisci il nome del file di cui visualizzare l'anteprima (fine per uscire):");
        scanf("%s", filename);  //usare scanf, non fgets va
    }
}