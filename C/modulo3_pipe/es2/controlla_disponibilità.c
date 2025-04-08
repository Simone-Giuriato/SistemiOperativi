/*L'applicazione dovrà essere realizzata in C e presentare la seguente interfaccia:
controlla_disponibilita magazzino-1 magazzino-2 ... magazzino-N
dove magazzino-1, magazzino-2, ..., magazzino-N sono nomi relativi di file.

il file di nome magazzino-i che si suppone
contenere una riga (con nome della materia prima e informazioni sullo scaffale dove si
trova) per ogni unità di stoccaggio presente in magazzino.
*/

/*
Il codice fa questo: (da consegna)

    Il padre genera argc - 1 figli (Fi)

    Ogni figlio crea un proprio nipote (Ni) per ogni richiesta

    I nipoti fanno il lavoro vero (eseguono grep) e muoiono subito dopo

    I figli restano vivi, pronti a ricevere nuove richieste*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

typedef int pipe_t[2]; // creo un tipo di dato che rappresenta le pipe: un array di interi di grandezza 2, posso gestire al massimo MAX pipe, non so mai quanti file e quindi processi e quindi pipe avrò

#define MAX 10 // numero massimo di file/processi che posso gestire

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Errore nell'inserimento dei parametri");
        exit(1);
    }
    int i, j;
    int pid;
    char nome[100]; // nome materia prima
    char ans[100];
    int n_read;
    int status; // per la wait

    // non posso fare p1p2[2]; p2p3[2].... perchè non so quanti ne devo creare di pipe, magari una volta 1 poi2 altre 5....
    /*
     * prealloco un numero massimo, predefinito, di int[2] nello stack
     * del processo
     * Un alternativa poteva essere allocare dinamicamente con malloc*/

    // Essendo le pipe mondorezionali, pipeIn--> da padre a figlio, pipeOut--> da figlio a padre così Padre e figlio dialogano bidirezionalmente (entrambi possono scrivere entrambi possono leggere)
    pipe_t pipeIn[MAX]; // faccio un array grande MAX PROC(Massimo processi), dove ogni cella array è un array di due che rappresenta una pipe: ho fatto un array di pipe
    pipe_t pipeOut[MAX];

    for (i = 0; i < (argc - 1); i++)
    { // ho bisgono di argc-1 figli (-1 è perchè nell argc ho anche il nome del file-->processo padre che ho gia)

        if (pipe(pipeIn[i]) < 0)
        { // creo pipe che padre scrive, figlio legge
            perror("pipeIn");
            exit(2);
        }

        if (pipe(pipeOut[i]) < 0)
        { // creo pipe che figlio scirve, padre legge
            perror("pipeOut");
            exit(3);
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork:");
            exit(4);
        }
        if (pid == 0)
        {
            // figlio

            /* chiudo le pipe non utilizzate: quelle create dal padre
             * per i figli precedenti e ancora aperte e quelle create
             * per il figlio i-esimo, ma al quale non servono.
             * gli estremi pipes_in[x][0] per x da 0 a i-1 sono
             * chiusi dal padre dopo avere creato l'x-esimo figlio
             * e non sono quindi presenti nel figlio i-esimo */

            for (j = 0; j <= 1; j++)
            {
                close(pipeIn[j][1]);
                close(pipeOut[j][0]);
            }

            /* sfrutto il fatto che la read su pipe ritorni con valore 0
             * quando tutti i processi che la condividono chiudono
             * il loro estremo in scrittura per quella pipe. */

            while ((read(pipeIn[i][0], nome, sizeof(nome)) > 0)) // il padre ha scritto sulla pipe il nome,il figlio lo legge ecco perchè uso l estremo di lettura della pipe che mi permette questo
            {

                /* genero un nipote per contare le righe in cui compare
                 * l'elemento selezionato e comunicare al processo padre
                 * il risultato. */

                if ((pid = fork()) < 0)
                {
                    perror("fork generazione nipote");
                    exit(7);
                }

                if (pid == 0)
                {
                    // nipote

                    close(pipeIn[i][0]); // chiudo la pipe in lettura dal padre, che alnipote non serve
                    // redireziono lo stdout sulla pipe diretta al padre

                    dup2(pipeOut[i][1], 1); // forzo che duplichi in fd=1
                    close(pipeOut[i][1]);
                    // close(1); //non andava.... uso la forzatura dup2
                    // dup(pipeOut[i][1]);   // duplico
                    // close(pipeOut[i][1]); // chiudo l originale, sapendo che il duplicato ha fd=1

                    execlp("grep", "grep", "-c", nome, argv[i + 1], (char *)0); // argv[i+1] percheè se i=0 devo prendere il secondo parametro visto che il primo è il file del codice, di conseguenza così per i successivi file
                    perror("grep:");
                    exit(6);
                }

                wait(&status); // Il figlio attende la terminazione del nipote
            }

            // sono nel figlio (NON NEL NIPOTE)

            // chiudo le pipe che non mi servono (ovvero entrambe):
            close(pipeIn[i][0]);
            close(pipeOut[i][1]);
            exit(0); // termino
        }

        // sono nel Padre
        /* chiudo estremità delle pipe non usate dal padre */
        close(pipeIn[i][0]);
        close(pipeOut[i][1]);
    }

    /* ciclo principale del padre: si interfaccia con l'utente per
     * leggere il nome dell'oggetto da cercare e lo manda
     * a tutti i figli per la ricerca */

    printf("\nInserisci il nome dell'oggetto da cercare:\n"); // chiedo allutente cosa cercare
    scanf("%s", nome);

    while (strcmp(nome, "fine") != 0)
    {
        // La consegna mi chiede di passare il nome da cercare (la materia) ai figli
        for (i = 0; i < (argc - 1); ++i)
        {
            /* ATTENZIONE: si deve inviare anche carattere
             * terminazione stringa */
            write(pipeIn[i][1], nome, strlen(nome) + 1);
        }

        /* leggo le risposte dai figli */
        for (i = 0; i < (argc - 1); ++i)
        {
            /* reset memoria a ogni ciclo, per evitare output sporchi */
            // memset(ret, 0, sizeof(ret));
            /* lettura da pipe i-esima */
            if ((n_read = read(pipeOut[i][0], ans, sizeof(ans))) < 0)
            {
                perror("read");
                exit(8);
            }
            printf("Nel file %s ci sono %d unità di materia prima %s\n",
                   argv[i + 1], atoi(ans), nome);
        }
        printf("\n Inserisci il nome del nuovo oggetto da cercare:\n");
        scanf("%s", nome);
    }

    /* faccio terminare i figli chiudendo gli estremi in scrittura

* sulle pipe pipes_in, quindi chiudo le altre pipe e pulisco
* la memoria aspettando tutti gli zombie.
* E' FONDAMENTALE chiudere l'estremo in scrittura di
* pipeIn PRIMA di effettuare la chiamata a wait(),
* per evitare DEADLOCK. */
    for (i = 0; i < (argc - 1); ++i)
    {
        close(pipeIn[i][1]);
        close(pipeOut[i][0]);
        wait(&status);
    }

    return 0;
}

// È inutile tenere traccia dei Pid ecco perchè nei for faccio pid=fork(), e non faccio un array di pid...