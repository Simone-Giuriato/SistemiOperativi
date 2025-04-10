
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

int richieste = 0;

void handler(int signo)
{
    printf("\nSono state effettuate %d richieste\n", richieste);
    exit(0);
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore nell'inserimento dei parametri");
        exit(1);
    }

    if (argv[1][0] == '/')
    {
        printf("Inserire nome relativo di una directory"); // deve essere nome relativo non assoluto
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

    int pid1, pid2, status;
    char tipo[100];
    char data[50];
    int p1p2[2];
    char to_open[200]; // file da aprire composto da cartla/data +.txt

    signal(SIGINT, handler); // modifico comportamento

    while (1)
    {

        printf("Inserire tipo di video:");
        scanf("%s", tipo);
        printf("Inserire data di interesse YYYYMM:");
        scanf("%s", data);

        // io so la data del file ma devo aggiugerci l'estensione:
        sprintf(to_open, "%s/%s.txt", argv[1], data);
        // printf("\n %s", to_open); // magari controlla che esista questo file se concatena giusto
        if ((fd = open(to_open, O_RDONLY)) < 0)
        {
            printf("Errore nell'apertura del file %s\n", to_open);
            exit(10);
        }
        close(fd);
        if (pipe(p1p2) < 0)
        {
            perror("pipe:");
            exit(4);
        }

        pid1 = fork();
        if (pid1 < 0)
        {
            perror("Fork1:");
            exit(5);
        }

        if (pid1 == 0)
        {
            // figlio P1
            signal(SIGINT, SIG_DFL); // i figli non devono richiamare la handler, ma eseguiranno il comportamento del CTRL c
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            close(p1p2[0]);

            execlp("grep", "grep", "-w", tipo, to_open, (char *)0);
            perror("grep:");
            exit(7);
        }

        pid2 = fork();
        if (pid2 < 0)
        {
            perror("Fork2:");
            exit(6);
        }

        if (pid2 == 0)
        {
            // figlio P2
            signal(SIGINT, SIG_DFL);
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);
            close(p1p2[1]);

            execlp("sort", "sort", "-r", "-n", (char *)0);
            perror("sort");
            exit(8);
        }

        // padre
        // chiudo pipe
        close(p1p2[0]);
        close(p1p2[1]);

        // attendo terminazione figli
        wait(&status);
        wait(&status);
        richieste++;
    }

    return 0;
}

// COME FORMO IL NOME DEL FILE? argv[1]/data.txt ([argv1]/data.txt devo aggiungere uno / che dice che il file data.txt sta dentro argv[1])
/*sprintf è una funzione della libreria standard del C (stdio.h)
che scrive una stringa formattata dentro un buffer di caratteri
(cioè dentro un array di char), invece di stamparla su schermo come farebbe printf.

sprintf(buffer, "Nome: %s, Età: %d", nome, eta);

La stringa "Nome: Mario, Età: 25" viene salvata in buffer.

io faccio:
sprintf(to_open, "%s/%s.txt", argv[1], data);   (nota lo / dopo il primo%s)

Froma la stringa

info/202006.txt
info è la cartella che passo
/ per dire che il file sta dentro quella cartella, lo aggiungo io
data
.txt


La parte "info/" nel tuo esempio si riferisce a un percorso relativo di una directory.
Nel contesto del programma che stai sviluppando, questa parte specifica la cartella che contiene i file di testo con le informazioni sui video.

*/