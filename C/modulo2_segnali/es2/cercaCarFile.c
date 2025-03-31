//NON FUNZIONA BENISSIMO È DA RIVEDERE, MA LA SOSTANZA C'È
//PER CAPIRE I SEGNALI BASARSI SULL ES 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

char target_car;
int car_trovati;
int early_termination;

void handler_start(int segnale)
{
    printf("Il figlio %d ha ricevuto START-> il suo carattere:%c\n", getpid(), target_car);
}

void handler_end(int segnale)
{
    printf("Il figlio %d ha ricevuto END: il suo carattere %c, caratteri trovati: %d\n", getpid(), target_car, car_trovati);
}
void handler_early()
{
    early_termination = 1;
}

int main(int argc, char **argv)
{
    char read_car;

    int nFigli = 0;
    nFigli = argc - 3; // minimo devo avere 4 argomenti (quindi solo un carattere)--> 4-3= 1 carattere... 5-3=2 caratteri. Ogni carattere è un figlio
    int pid[nFigli];   // avrò tanti pid quanti sono i figli, ovvero quanti sono i caratteri

    if (argc < 4)
    {
        printf("Inserire alemno 4 argomenti: programma-caratteri-nomeFile-Numsec\n");
        exit(1);
    }

    char filename[1024];
    strcpy(filename, argv[argc - 2]);
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("open:");
        exit(3);
    }
    close(fd); // posso chiudere il file

    int seconds = 0;
    seconds = atoi(argv[argc - 1]);

    //configuro i segnali
    signal(SIGUSR1, handler_start); //start
    signal(SIGUSR2, handler_end);   //fine

    for (int i = 0; i < nFigli; i++)
    {
        pid[i] = fork();

        if (pid[i] < 0)
        {
            perror("fork:");
            exit(2);
        }
        if (pid[i] == 0)
        {
            //FIGLIO
            target_car = argv[i + 1][0]; // per dire quel argomento/stringa ma voglio il primo carattere (in questo caso l unico)

            printf("Il figlio %d sta aspettando...\n", getpid());
            pause(); // metto in pausa il figlio finchè non riceve un segnale (accadrà quando tutti i figli son generati)
            car_trovati = 0;
            fd = open(filename, O_RDONLY);     // i fd non sono condivisi tra padre e figlio, devo riaprire il file
            while (read(fd, &read_car, 1) > 0 ) // va sempre la & (a meno che non sia una stringa--> array)
            {
                if (read_car == target_car)
                {
                    car_trovati++;
                }
            }

            // se arrivo qui è perchè ho finito il file... quindi ho vinto
            printf("Ha vinto il processo %d\n", getpid());
            kill(0, SIGUSR2); // mando a tutto il gruppo la fine (padre e figli)
            close(fd);
            exit(0);
        }
    }
    // SONO NEL  processo PADRE
    sleep(2); // Assicurati che tutti i figli siano pronti
    signal(SIGUSR1, SIG_IGN);
    early_termination = 0;
    signal(SIGUSR2, handler_early);
    // inizio della gara:
    kill(0, SIGUSR1);
    printf("vaiiiii!!!\n");

    sleep(seconds); // faccio addormentare il padre per i secondi che mi ha detto l utente, (tempo che dura la gara)

    // fine
    if (early_termination == 0)
    {
        printf("TEMPO SCADUTO\n");
        kill(0, SIGUSR2);
    }
    else
    {
        printf("FINITO PRIMA\n");
    }

    int status = 0;
    for (int j = 0; j < nFigli; j++)
    { // attendo la morte di ogni figlio
        wait(&status);
    }

    return 0;
}