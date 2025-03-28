#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h> //libreria per i segnali

int CONTATORE=0;

void handler(int segnale){  //funzione per gestire il segnale SIGUSR1, si fa così, PASSARE SEMPRE IL PARAMETRO INT SEGNALE (si arrangio il S.O.)

    printf("segnale ricevuto. Il contatore è a: %d\n", CONTATORE);
    exit(3);

}

int main(int argc, char **argv)
{

    
    int status = 0;

    if (argc != 3)
    {
        printf("Errore nell inserimento dei parametri\n");
        exit(1);
    }

    int Nf, Nsec = 0;
    int pid[Nf]; // array di pid, dove ogni cella contiene il pid di un figlio

    // converto argv (stringa in un intero), valori che suerò dopo nel ciclo
    Nf = atoi(argv[1]);   // numero figli da generare
    Nsec = atoi(argv[2]); // numero secondi che padre deve dormire

    for (int i = 0; i < Nf; i++)
    {

        pid[i] = fork();

        if (pid[i] < 0)
        {
            printf("Errore nella fork:\n");
            exit(2);
        }
        else if (pid[i] == 0)
        {
            // FIGLIO

            signal(SIGUSR1, handler); // gestisco il segnale SIGUSR1 con il comportamento che dico nella procedura handler

            while (1)
            {             // ciclo infinito
                sleep(1); // figlio dorme per 1 secondo
                CONTATORE++;
            }
        }
        
        
    }
    // PADRE
    //il padre va fuori dal for, perchè se fosse dentro generava un figlio e lo uccide, poi genera e uccide
    //così facendo invece, lui genera tutti i figli e poi li uccide tutti insieme
    //(ai fini dell output non sarebbe cambiato nulla)

    sleep(Nsec);

    for(int j=0; j<Nf;j++){
        kill(pid[j], SIGUSR1); // mando ad ogni processo il segnale SIGUSR1 (modificato da me).
       
    }

    /*
    signal(SIGUSR1,SIG_IGN) //dico al padre di ignoraa il segnale
     kill(0, SIGUSR1); // mando ad ogni processo il segnale SIGUSR1 (modificato da me).

    */


    for (int k = 0; k < Nf; k++)
    { // attendo la morte dei figli (di ogni figlio) con la wait
        wait(&status);
    }

   
}