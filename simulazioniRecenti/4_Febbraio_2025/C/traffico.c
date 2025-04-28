// traffico anno(relativo)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int n_richieste;
void handler(int signo){
    printf("\nSono state effettuate %d richieste\n", n_richieste);
    exit(0);    //esco
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Numero argomenti deve essere 2");
        exit(1);
    }
    if (argv[1][0] == '/')
    {
        printf("Il path della directory deve essere relativo");
        exit(2);
    }
    int fd;
    fd = open(argv[1], O_RDONLY | __O_DIRECTORY);
    if (fd < 0)
    {
        perror("open dir:");
        exit(3);
    }
    close(fd);
    int status;
    int pid1, pid2, pid3;
    int p1p2[2], p2p3[2];
    char giorno[100]; // formato MMDD
    char strada[200];
    char to_open[1024]; // conterrà il nome composto da giorno + .trf

    signal(SIGINT,handler);

    while (1)
    {
        printf("Inserisci il giorno (fine per terminare):");
        scanf("%s", giorno);

        if (strcmp(giorno, "fine") == 0)
        { // se inserisce fine esco
            break;
        }

        sprintf(to_open, "%s/%s.trf", argv[1], giorno);
        fd = open(to_open, O_RDONLY);
        if (fd < 0)
        {
            perror("open file:");
            exit(4);
        }
        close(fd);

        printf("Inserisci strada:");
        scanf("%s", strada);

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
            signal(SIGINT,SIG_DFL);
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            close(p1p2[0]);

            execl("/bin/grep", "grep", "-w", strada, to_open, (char *)0);
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
            signal(SIGINT,SIG_DFL);
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);
            close(p1p2[1]);

            close(1);
            dup(p2p3[1]);
            close(p2p3[1]);
            close(p2p3[0]);

            execl("/bin/sort", "sort", "-n","-r", "-t,","-k1", (char *)0);  //-n per i numeri, -r per decrescente, "-t,"" per indicare che i campi son divisi da ',', "-k 1" ordina sul primo campo ma lo ordina in automatico sul primo
            perror("sort:");
            exit(10);
        }

        // sono padre chiudo pipe che non mi servono:
        close(p1p2[1]);
        close(p1p2[0]);

        pid3 = fork();
        if (pid3 < 0)
        {
            perror("fork3:");
            exit(11);
        }
        if (pid3 == 0)
        {
            signal(SIGINT,SIG_DFL);
            close(0);
            dup(p2p3[0]);
            close(p2p3[0]);
            close(p2p3[1]);

            execl("/bin/cut", "cut", "-d,","-f1 2", (char *)0);
            perror("cut");
            exit(12);
        }

        // padre chiudo pipe non necessarie
        close(p2p3[1]);
        close(p2p3[0]);

        wait(&status);
        wait(&status);
        wait(&status);
        n_richieste++;
    }
    printf("\nSono state effettuate %d richieste\n", n_richieste);  //entra in gioco, se si esce con la stringa "fine"
    return 0;
}

//comando cut:
// -d,    -f1 2 3

//comando sort:
// -t,     -k2 3 