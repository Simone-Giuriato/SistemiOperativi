#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define DIM 100

int main(int argc, char **argv)
{
    int p1p2[2]; // dichiaro array di interi che simula una pipe con 2 estremi
    int pid1, pid2, fd, status;
    if (argc != 2)
    { // controllo parametri
        printf("Errore nel numero degli argomenti");
        exit(1);
    }

    // controllo che il secondo parametro sia un nome assoluto di file
    if (argv[1][0] != '/')  //argv[1] è il primo argomento passato al programma dalla linea di comando.
    {                       //argv[1] [0] è il primo carattere del primo argomento
        printf("Il primo argomento deve essere un nome di file ASSOLUTO");
        exit(2);
    }
    char matricola[DIM];
    printf("Inserisci matricola (fine per terminare):");
    scanf("%s", matricola);

    fd = open(argv[1], O_RDONLY);   //per vedere se esisite il file, ma non devo fare operazioni di lettura e scrittura sul file
    if (fd < 0)
    {
        perror("open");
        exit(7);
    }
    close(fd);

    while (strcmp(matricola, "fine") != 0)
    {

        if (pipe(p1p2) < 0)
        { // creazione pipe p1p2
            perror("pipe p1--->p2");
            exit(8);
        }

        pid1 = fork(); // genero figlio 1

        if (pid1 < 0)
        {
            perror("fork p1");
            exit(3);
        }
        if (pid1 == 0)
        {
            // figlio P1, deve scirvere le robe del file
            close(p1p2[0]); // chiudo estremi di lettura che non mi serve
            close(1);       // chiudo stdout, libero 1
            dup(p1p2[1]);   // duplico l estremo di scrittura e lo metto nella prima posizione dei descriptor libera che è 1
            // 1 ora non è più collegato al terminale/video ma va tutto alla pipe
            close(p1p2[1]);  //chiudo l fd originale      // chiudo l'estremo di scrittura, tanto avrò un nuovo fd fatto con la dup
            execlp("grep", "grep", "-w", matricola, argv[1], (char *)0); // la grep manderà il risulato non sul video, ma alla pipe (opzione -w cerca solo parole intere)
            perror("grep");                                              // P1 arriva qui solo in caso di errore
            exit(4);
        }

        // ancora padre:
        pid2 = fork(); // genero figlio P2
        if (pid2 < 0)
        {
            perror("fork p2");
            exit(5);
        }
        if (pid2 == 0)
        {
            //figlio P2
            close(p1p2[1]); // P2 deve leggere dalla pipe, chiudo il lato scrittura
            close(0);       // libero stdin
            dup(p1p2[0]);   // duplico estremo lettura pipe, si mette in 0, ho fatto un altro fd
            close(p1p2[0]); // chiudo estremo lettura originale pipe, non mi serve più
            execlp("wc", "wc", "-l", (char *)0);
            perror("wc"); // arrivo qui solo se problemi con exec wc
            exit(6);
        }
        // sono nel padre   (distinguo con fork()==0, non serve fare >0)

        // chiudo i canali pipe non necessari   
        close(p1p2[0]); //il padre non deve leggere
        close(p1p2[1]); //il padre non deve scrivere

        // attendo morte dei figli:
        wait(&status);
        wait(&status);

        printf("Inserisci una nuova matricola: (fine per terminare):");
        scanf("%s", matricola);
    }

    return 0;
}


//SOTTOLINEO che la wc stampa a video perchè nel figlioP2 non abbiamo mai modificato lo stdout nella tabella dei descrittori del figlio P2, ma solo lo stdin

//Se io creo una pipe nel padre, i figli la ereditano
//sia nel padre, sua nei figli CHIUDERE gli estremi delle pipe che non mi servono
//SE NEL PADRE (come in questo caso) NON FACCIO NULLA CON LE PIPE, CHIUDO SUBITO ENTRAMBI GLI ESTREMI