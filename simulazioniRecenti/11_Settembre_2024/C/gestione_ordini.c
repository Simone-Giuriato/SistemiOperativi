// gestione_ordini dir (relativo)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int n_richieste = 0;
void handler(int singo)
{
    printf("\nSono state eseguite %d richieste\n", n_richieste);
    exit(0); // esco, codcie errore va bene
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore: il numero degli argomenti passati deve essere 2");
        exit(1);
    }

    if (argv[1][0] == '/')
    {
        printf("Errore la directory deve avere path relativo");
        exit(2);
    }

    int pid1, pid2, pid3;
    int status;
    int p1p2[2], p2p3[2], p3p0[2];
    int fd;

    fd = open(argv[1], __O_DIRECTORY);
    if (fd < 0)
    {
        perror("open direcotry");
        exit(3);
    }
    char codice[100];
    char data[100];
    char n_risultati[50]; // lo vuole intero, ma per metterlo nella exec mi serve stringa
    char to_open[200];    // contiene il nome composto del file da aprire

    char buff[1024];
    int nread;
    signal(SIGINT, handler); // modifico comportamento ctrl-C

    while (1)
    {

        printf("Inserisci codice del corriere:");
        scanf("%s", codice);
        printf("Inserisci data (formato YYYYMMDD):");
        scanf("%s", data);

        sprintf(to_open, "%s/%s.txt", argv[1], data);

        fd = open(to_open, O_RDONLY);
        if (fd < 0)
        {
            perror("open file:");
            exit(4);
        }
        // se esiste file allora procedo con richieste
        printf("Inserisci numero di risultati da mostrare:");
        scanf("%s", n_risultati);

        if (pipe(p1p2) < 0)
        {
            perror("p1p2:");
            exit(5);
        }

        pid1 = fork();
        if (pid1 < 0)
        {
            perror("fork1:");
            exit(6);
        }
        if (pid1 == 0)
        {
            // figlio P1
            // printf("ciao sono p1\n"); //se voglio fare debug con printf metterle prima di manipolare std e con /n perchè se no non le mostra per bufferr...
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            close(p1p2[0]);

            execlp("grep", "grep", "-w", codice, to_open, (char *)0);
            perror("grep:");
            exit(7);
        }

        if (pipe(p2p3) < 0)
        {
            perror("p2p3:");
            exit(8);
        }

        pid2 = fork();
        if (pid2 < 0)
        {
            perror("fork2:");
            exit(9);
        }
        if (pid2 == 0)
        {
            // figlio P2

            close(0); // leggo ciò che P1 ha scritto nella pipe p1p2
            dup(p1p2[0]);
            close(p1p2[0]);
            close(p1p2[1]);

            close(1); // scrivo ciò che elaboro per P3 in p2p3
            dup(p2p3[1]);
            close(p2p3[1]);
            close(p2p3[0]);

            execlp("sort", "sort", "-n", (char *)0);
            perror("sort:");
            exit(10);
        }

        // padre--> chiudo le pipe che qua non servono più
        close(p1p2[1]);
        close(p1p2[0]);

        if (pipe(p3p0) < 0)
        {
            perror("p3p0:");
            exit(11);
        }

        pid3 = fork();
        if (pid3 < 0)
        {
            perror("fork3:");
            exit(12);
        }

        if (pid3 == 0)
        {
            // figlio P3

            close(0); // leggo ciò che mi ha scritto P2 in p2p3
            dup(p2p3[0]);
            close(p2p3[0]);
            close(p2p3[1]);

            close(1); // scrivo elaborazione in p3p0 per il padre
            dup(p3p0[1]);
            close(p3p0[1]);
            close(p3p0[0]);

            execlp("head", "head", "-n", n_risultati, (char *)0);
            perror("head:");
            exit(13);
        }

        // padre--> chiudo le pipe che non mi servono
        close(p2p3[1]);
        close(p2p3[0]);
        close(p3p0[1]);

        while ((nread = (read(p3p0[0], buff, sizeof(buff)))) > 0)
        {

            write(1, buff, nread);
            memset(buff, 0, sizeof(buff)); // per pulire, metto 0 buff non lascio residui
        }
        close(p3p0[0]);

        // attendo terminazione figli
        wait(&status);
        wait(&status);
        wait(&status);
        n_richieste++; // aggiorno le richieste
    }

    return 0;
}