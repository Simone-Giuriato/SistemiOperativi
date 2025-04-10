// ARGOMENTI: trovaParola.c fileAnagrafica(nome assoluto)
/*P1 deve selezionare
all’interno del file fileAnagrafica tutti i clienti con il cognome indicato e inviare tali
informazioni al processo P2, che è incaricato di filtrare i dati ricevuti stampando a video
solo i clienti che risiedono nell’area geografica specificata dal prefisso indicato.
*/

/*L’applicazione termina quando l’utente inserisce la stringa “fine” oppure se l’utente invia
un “ctrl- c”. In entrambi i casi prima di terminare l’applicazione visualizza il numero totale
di ricerche effettuate fino a quel momento.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

int richieste = 0;

void handler(int signo)
{
    printf("\nSono state effettuate %d richieste\n", richieste);
    exit(0); // codice 0 perchè è un uscita che va bene, non un errore
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Errore nell'inserimento dei parametri");
        exit(1);
    }

    if (argv[1][0] != '/')
    {
        printf("Errore: non è un percorso assoluto");
        exit(2);
    }
    int p1p2[2]; // array che simula pipe

    int pid1, pid2, status;
    char cognome[100];
    char prefisso_tel[50]; // il prefisso contiene numeri, ma ho messo char/stringa perche non si fanno operazioni con esso

    // controllo se fileAnagrafica esiste:
    int fd;
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    { // se open resitutisce minore di 0, problemi col file
        perror("open:");
        exit(3);
    }
    close(fd); // chiudo file perchè i comandi come grep.. non hanno bisgno di lavorare con file aperto
    // imposto la gestione per il segnale di Ctrl-C (SIGINT)
    signal(SIGINT, handler);
    while (1)
    {
        printf("Inserisci il cognome del cliente (fine per terminare):");
        scanf("%s", cognome);

        if ((strcmp(cognome, "fine")) == 0) // se mette fine, break esco
        {
            break;
        }
        printf("Inserisci il prefisso telefono fisso (simbolo di Area geografica):");
        scanf("%s", prefisso_tel);

        // creazione pipe nel PADRE (che la passerà anche ai figli) della pipe p1p2
        if (pipe(p1p2) < 0)
        {
            perror("pipe");
            exit(7);
        }
        pid1 = fork();
        if (pid1 < 0)
        {
            perror("Fork1:");
            exit(4);
        }
        if (pid1 == 0)
        {

            signal(SIGINT, SIG_DFL); // imposto gestione default di SIGINT, il figlio eseguirà il segnale control C di default terminando (anche per figlio2)
            // figlio P1
            close(1);
            dup(p1p2[1]);
            close(p1p2[0]);
            close(p1p2[1]);

            execlp("grep", "grep", "-w", cognome, argv[1], (char *)0);
            perror("grep1:");
            exit(5);
        }
        // padre
        pid2 = fork(); // creo figlio P2
        if (pid2 < 0)
        {
            perror("Fork2:");
            exit(4);
        }
        if (pid2 == 0)
        {
            // figlio P2
            signal(SIGINT, SIG_DFL); // imposto gestione default di SIGINT
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);
            close(p1p2[1]);

            execlp("grep", "grep", "-w", prefisso_tel, (char *)0);
            perror("grep2:");
            exit(6);
        }

        // padre, ha ancora la modifica del segnale SIGINT (control C) gestita da handler
        // chiudo la pipe che non serve
        close(p1p2[0]);
        close(p1p2[1]);

        // attendo terminazione figli
        wait(&status);
        wait(&status);

        richieste++;
    }

    printf("Sono state effettuate %d richieste\n", richieste);

    return 0;
}

//in teoria andava bene anche fare un while con nella condizione strcmp ma ogni tanto dava problemi con loop infiniti