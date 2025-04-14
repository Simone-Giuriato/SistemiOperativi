#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int n_sigUsr2 = 0;

void handler(int signo) // variante per la gestione dei segnali ho SIGUSR2 e non control C
{
    n_sigUsr2++;
    printf("Numero di segnali SIGUSR2: %d\n", n_sigUsr2);
    // exit(0); NON SERVE PERCHÈ SE NO USCIREI DAL PROGRAMMA INVECE IO QUA VOGLIO SOLO STAMAPRE QUANTI SIGUSR2 HO FINO A QUEL MOMENTO, MENTRE COL CONTROL C DOVEVIO USCIRE DAL PROGRAMMA
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore, devi passare 2 argomenti");
        exit(1);
    }

    if (argv[1][0] == '/')
    {
        printf("Devi passare un nome RELATIVO di directory");
        exit(2);
    }

    int fd;
    /* controllo esistenza directory */
    if ((fd = open(argv[1], __O_DIRECTORY)) < 0)
    {
        perror("Errore nell'apertura di dir");
        exit(3);
    }
    close(fd);
    char identificativo[100];
    char nome_file[200];
    int fd2;
    char data[50];
    char n[60]; // risultati da mostrare, siccome va nella exec e vuole le stringhe li chiedo in formato stringa fin da subito, se lasciavo intero mi dava problemi con la exec
    int pid1, pid2, pid3;
    int p1p2[2], p2p3[3];

    // modifico comportamento siguusr2
    signal(SIGUSR2, handler);

    while (1)   //(quando mi chiede più campi usare il while infinito con break nell if compare)
    {

        printf("Inserisci identificativo del veivolo (fine per terminare):");
        scanf("%s", identificativo);

        // esco se l'utente inserisce "fine"
        if (strcmp(identificativo, "fine") == 0)
        {
            break;
        }

        // ora so l'identificativo: il file si crea con identificativo + .txt
        //  controllo che <dir>/<id_aereo>.txt esista
        sprintf(nome_file, "%s/%s.txt", argv[1], identificativo); // nome_file contiene la formattazione finale

        fd2 = open(nome_file, O_RDONLY);
        if (fd2 < 0)
        {
            perror("open");
            exit(4);
        }

        close(fd2);

        printf("Inserire la data:");
        scanf("%s", data);

        printf("Inserire numero di risultati da mostrare:");
        scanf("%s", n);

        if (pipe(p1p2) < 0)
        {
            perror("p1p2");
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
            signal(SIGUSR2, SIG_DFL);
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            close(p1p2[0]);

            execlp("grep", "grep", "-w", data, nome_file, (char *)0);
            perror("grep1:");
            exit(7);
        }

        if (pipe(p2p3) < 0)
        {
            perror("p2p3");
            exit(9);
        }

        pid2 = fork();
        if (pid2 < 0)
        {
            perror("fork2:");
            exit(8);
        }

        if (pid2 == 0)
        {
            // figlio P2
            signal(SIGUSR2, SIG_DFL);
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);
            close(p1p2[1]);

            close(1);
            dup(p2p3[1]);
            close(p2p3[1]);
            close(p2p3[0]);

            // ordino per durata
            execlp("sort", "sort", "-n", (char *)0); // sort ordina in ordine crescente di suo
            perror("sort1:");
            exit(10);
        }
        // PADRE
        //  chiudo pipe non necessarie
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
            signal(SIGUSR2, SIG_DFL); // per i figli SIGUSR2 fa comportamento default

            /* A tal fine si ricorda
            l’esistenza della funzione pid_t getppid(void); che restituisce il PID
            del processo padre.*/
            kill(getppid(), SIGUSR2); // invio segnale SIGUSR2 al padre

            close(0);
            dup(p2p3[0]);
            close(p2p3[1]);
            close(p2p3[0]);
            // devo visualizzare quelli più lunghi, quindi essendo in crescente devo prendere gli ultimi, uso tail e non head
            execlp("tail", "tail", "-n", n, (char *)0);
            perror("tail:");
            exit(12);
        }

        // PADRE
        // chiudo pipe non necessarie
        close(p2p3[1]);
        close(p2p3[0]);

        wait(NULL);
        wait(NULL);
        wait(NULL);
    }

    return 0;
}

/*sprintf è una funzione della libreria standard del C (stdio.h)
che scrive una stringa formattata dentro un buffer di caratteri
(cioè dentro un array di char), invece di stamparla su schermo come farebbe printf.

sprintf(buffer, "Nome: %s, Età: %d", nome, eta);

La stringa "Nome: Mario, Età: 25" viene salvata in buffer.*/