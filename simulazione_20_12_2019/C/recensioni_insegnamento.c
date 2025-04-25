#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

// Recensioni_insegnamento archivio_CdL  --> archivio_CdL è file assoluto

int n_richieste=0;

void handler(int signo){
    printf("\nNumero di richieste servite: %d\n",n_richieste);
    exit(0);    //premendo ctrl c esco (dopo aver detto il numero di richieste)
}
int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore,servono 2 argomenti");
        exit(1);
    }

    if (argv[1][0] != '/')
    {
        printf("Errore, deve essere un nome assoluto di file");
        exit(2);
    }

    int fd;
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open:");
        exit(3);
    }
    close(fd);
    char materia[200];
    int p1p2[2];
    int p2p0[2];
    int pid1, pid2;
    printf("Inserisci materia d'insegnamento('fine' per terminare):");
    scanf("%s", materia);
    int nread = 0;
    char results[1024];
    //imposto gestione segnale SIGIINT (ctrl c)
    signal(SIGINT,handler);
    while (1)
    {
        if (strcmp(materia, "fine") == 0)
        { // se inserisco fine break,esco
           
            break;
        }

        if (pipe(p1p2) < 0)
        { // creazione p1p2 pipe
            perror("p1p2:");
            exit(4);
        }
        pid1 = fork();
        if (pid1 < 0)
        {
            perror("fork1:");
            exit(5);
        }
        if (pid1 == 0)
        {
            // P1
            signal(SIGINT,SIG_DFL);
            close(1);       // chiudo std1
            dup(p1p2[1]);   // copio p1p2[1] in 1
            close(p1p2[1]); // chiudo originale p1p2[1]
            close(p1p2[0]); // chiudo estremo lettura che non mi serve mai

            execlp("grep", "grep", "-w", materia, argv[1], (char *)0);
            perror("grep:");
            exit(6);
        }

        if (pipe(p2p0) < 0)
        {
            perror("p2p0:");
            exit(7);
        }

        pid2 = fork();
        if (pid2 < 0)
        {
            perror("frok2:");
            exit(8);
        }

        if (pid2 == 0)
        {
            // P2

            // legge da p1p2
            signal(SIGINT,SIG_DFL);
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);
            close(p1p2[1]);

            // scrive in p2p0
            close(1);
            dup(p2p0[1]);
            close(p2p0[1]);
            close(p2p0[0]);
            execlp("sort", "sort", "-r", "-n", (char *)0);
            perror("sort:");
            exit(9);
        }
        // chiudo ciò che fin qua non mi serve (sono padre)

        close(p1p2[1]);
        close(p1p2[0]);
        close(p2p0[1]);

        while ((nread = read(p2p0[0], results, sizeof(results))) > 0)   //leggo da estremo lettura, metto in array char, di grandezza sizeof array char
        { // read>0 finchè leggi

            write(1, results, nread);   //scrivo in std1 (terminale), cosa? ciò che ho in array results, quanto grande? di grandezza ciò che ho letto (nread)
            memset(results,0,sizeof(results));  //Pulisce il buffer-> "Azzera tutto il contenuto dell’array results, cioè imposta ogni byte a zero.
            //NON SERVE
        }

        // chiudo la pipe che non  mi serve più
        close(p2p0[0]);

        wait(NULL);
        wait(NULL);
        n_richieste++;
        printf("Inserisci materia d'insegnamento('fine' per terminare):");
        scanf("%s", materia);
    }
    printf("\nNumero di richieste servite: %d\n",n_richieste);  //quando esco, quindi inserisce fine, mostro numero di richieste
    return 0;
}