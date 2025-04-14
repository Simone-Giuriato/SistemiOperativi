#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int tot_richieste = 0;

void handler(int signo)
{
    printf("\nNumero di richieste effettuate: %d\n", tot_richieste);
    exit(0); // exit(0) perchè è un uscita regolare va tutto come deve andare
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore nell'inserimento dei parametri");
        exit(1);
    }

    int fd;
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open:");
        exit(2);
    }
    close(fd);

    if (argv[1][0] != '/')
    {
        printf("Il file deve essere un nome assoluto");
        exit(3);
    }

    char tipo[100];
    printf("Inserisci tipologia di bolletta (esci per terminare):");
    scanf("%s", tipo);
    int pid1 = 0, pid2, pid3;
    int p1p2[2], p2p3[2];
    // configuro segnale CTRL^C
    signal(SIGINT, handler);
    while (strcmp(tipo, "esci") != 0)
    {

        if (pipe(p1p2) < 0)
        {
            perror("p1p2:");
            exit(4);
        }

        if (pipe(p2p3) < 0)
        {
            perror("p2p3:");
            exit(9);
        }

        pid1 = fork();
        if (pid1 < 0)
        {
            perror("fork1:");
            exit(5);
        }
        if (pid1 == 0)
        {
            // figlio P1
            signal(SIGINT, SIG_DFL); // i figli ereditano anche la modifica di prima riguardo al SIGINT, ora per i figli ripristino il comportamento per non avere un comportamente sgradito (in teoria in questo programma non cammbiava)
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            close(p1p2[0]);

            execlp("grep", "grep", "-w", tipo, argv[1], (char *)0);
            perror("grep1:");
            exit(6);
        }

        pid2 = fork();
        if (pid2 < 0)
        {
            perror("fork2:");
            exit(7);
        }
        if (pid2 == 0)
        {
            // figlio P2
            // faccio in modo di leggere dalla pipe p1p2
            signal(SIGINT, SIG_DFL); // i figli ereditano anche la modifica di prima riguardo al SIGINT, ora per i figli ripristino il comportamento per non avere un comportamente sgradito (in teoria in questo programma non cammbiava)
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);
            close(p1p2[1]);

            // faccio in modo di scrivere la roba che trovo sulla pipe p2p3 comunicando con p3
            close(1);
            dup(p2p3[1]);
            close(p2p3[1]);
            close(p2p3[0]);

            execlp("grep", "grep", "-w", "da pagare", (char *)0);
            perror("grep2:");
            exit(8);
        }

        // OBBLIGATORIO CHIUDERE LE PIPE QUI PERCHÈ NON USO PIÙ P1P2
        close(p1p2[0]);
        close(p1p2[1]);

        pid3 = fork();
        if (pid3 < 0)
        {
            perror("fork3");
            exit(10);
        }

        if (pid3 == 0)
        {
            signal(SIGINT, SIG_DFL); // i figli ereditano anche la modifica di prima riguardo al SIGINT, ora per i figli ripristino il comportamento per non avere un comportamente sgradito (in teoria in questo programma non cammbiava)
            close(0);
            dup(p2p3[0]);
            close(p2p3[1]);
            close(p2p3[0]);

            execlp("sort", "sort", "-r", "-n", (char *)0);
            perror("sort:");
            exit(11);
        }

        // il padre chiude le pipe perchè non gli servono
        close(p2p3[1]);
        close(p2p3[0]);

        // attendo terminazione dei figli
        wait(NULL); // metto NULL e non &status perchè non mi interessa l exit code del figlio (andava bene anche &status)
        wait(NULL);
        wait(NULL);

        tot_richieste++;

        printf("Inserisci tipologia di bolletta (esci per terminare):\n"); //(se mi dimentico di mettere questi printf vado in un loop infinito)
        scanf("%s", tipo);
    }
    printf("Fine applicazione, svolti %d servizi\n", tot_richieste);

    return 0;
}

/*Per default, sort ordina le righe in ordine alfabetico (lessicografico).

Esempio:

100
50
5

Risultato con sort (senza -n):

100
5
50

Flag -n: ordine numerico

Con -n, sort interpreta i numeri come numeri reali e non come stringhe:

sort -n

Risultato:

5
50
100

Con -r, sort inverte l'ordine (quindi decrescente):

sort -r

(ma senza -n sarebbe ancora in ordine alfabetico inverso)

//SE HO NUMERI METTERE SEMPRE -n anche in combo con altro flag

*/