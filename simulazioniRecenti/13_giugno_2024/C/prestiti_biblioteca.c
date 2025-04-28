// prestiti_biblioteca dir

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

int n_richieste;

void handler(int signo)
{
    printf("\nSono state effettuate: %d richieste\n", n_richieste);
    exit(0); // esco con codice errore 0--> OK
}
void handler2(int signo)
{
    printf("\nHo ricevuto un segnale, mi sono risvegliato\n");
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore: insierisci 2 parametri");
        exit(1);
    }

    if (argv[1][0] == '/')
    {
        printf("Errore il il path di <dir> deve essere relativo");
        exit(2);
    }

    int pid1, pid2, pid3;
    int p1p2[2], p2p3[2];
    int fd;
    char codice_libro[100];
    char mese[50];
    char n_risultati[50]; // n risultati da mostrare, LA EXEC ACCETTA STRINGHE, QUINDI LO METTO COME STRINGA (ARRAY DI CARATTERI)

    fd = open(argv[1], __O_DIRECTORY);  //boh funziona io uso questo

    if (fd < 0)
    {
        perror("open direcotry:");
        exit(6);
    }

    printf("Inserisci codice libro da cercare:");
    scanf("%s", codice_libro);
    printf("Inserisci mese di interesse (formato AAAAMM):");
    scanf("%s", mese);
    printf("Inserisci numero di risultati da mostrare:");
    scanf("%s", n_risultati);
    char to_open[1024];
    int status;
    signal(SIGINT, handler);   // configurazione ctrl-C
    signal(SIGUSR1, handler2); // configuro segnale SIGUSR1

    while (1)
    { // per uscire non mi dice di far inserire fine all'utente, ma si esce solo col segnale di ctrl-C

        sprintf(to_open, "%s/%s.txt", argv[1], mese); // compongo il nome del file in cui cercare, inserendolo nella variabile

        fd = open(to_open, O_RDONLY); // verifico file esista

        if (fd < 0)
        {
            perror("open file:");
            exit(5);
        }

        if (pipe(p1p2) < 0)
        {
            perror("p1p2:");
            exit(3);
        }

        pid1 = fork();
        if (pid1 < 0)
        {
            perror("fork1:");
            exit(4);
        }

        if (pid1 == 0)
        {
            // figlioP1
            pause(); // si sospende finchè non riceve un segnale (il sigurs1)
            close(1);
            dup(p1p2[1]);
            close(p1p2[0]);
            close(p1p2[1]);

            execlp("grep", "grep", "-w", codice_libro, to_open, (char *)0);
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
            signal(SIGUSR1, SIG_IGN); // ignoro il segnale  SIGUSR1 perchè serve solo a P1 (non servirebbe metterlo)
            close(0);                 // leggo ciò che P1 ha scritto in p1p2
            dup(p1p2[0]);
            close(p1p2[0]);
            close(p1p2[1]);

            close(1); // scrivo l'elaborazione in p2p3
            dup(p2p3[1]);
            close(p2p3[1]);
            close(p2p3[0]);

            execlp("sort", "sort", "-r", "-n", (char *)0);
            perror("sort:");
            exit(10);
        }
        // padre
        // chiudo le pipe che ad ora non mi servono più (p1p2) OBBLIGATARIO FARLO QUA
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
            // figlio P3
            signal(SIGUSR1, SIG_IGN); // ignoro il segnale  SIGUSR1 perchè serve solo a P1 (non servirebbe metterlo)
            close(0);                 // leggo ciò che P2 ha scritto in p2p3
            dup(p2p3[0]);
            close(p2p3[0]);
            close(p2p3[1]);

            execlp("head", "head", "-n", n_risultati, (char *)0); // ERRORE PASSATO: mettere n_risultati come int, ma la exec accetta solo stringhe... mi dava BAD ADDRESS
            perror("head:");
            exit(12);
        }

        // padre
        // chiudo le pipe che non servono più (p2p3, p1p2 le avevo già chiuse)
        close(p2p3[1]);
        close(p2p3[0]);

        // METTERE IL RISVEGLIO DEL FIGLIO, PRIMA DELLA WAIT(che simboleggia la morte del figlio)
        printf("Sono il padre Invio SIGUSR1 a P1 per farlo uscire dalla pausa\n");
        kill(pid1, SIGUSR1); // invio il segnale SIGUSR1 al figlio P1

        wait(&status);
        wait(&status);
        wait(&status);

        n_richieste++; // aggiorno le richieste

        // SE non metto le printf essendo ciclo infinito, mi darebbe sempre quei risultati, ma tanto le printf e scanf le devo mettere per continuare a chiedere all'utente
        printf("Inserisci codice libro da cercare:");
        scanf("%s", codice_libro);
        printf("Inserisci mese di interesse (formato AAAAMM):");
        scanf("%s", mese);
        printf("Inserisci numero di risultati da mostrare:");
        scanf("%s", n_risultati);
    }

    return 0;
}