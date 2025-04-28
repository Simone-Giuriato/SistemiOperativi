// libri_prestati dir(assoluto)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

int n_richieste;

void handler(int signo){
    printf("\nSono state effettuate %d richieste\n",n_richieste);
    exit(0);
}
void handler2(int signo){
    printf("SVEGLIATIII\n");
    //non metto exit(0)--> non deve uscire
}

int main(int argc, char **argv){

    if(argc!=2){
        printf("Errore numero degli argomenti sbagliati");
        exit(1);
    }

    if(argv[1][0]!='/'){
        printf("Il secondo parametro deve avere un percorso assoluto");
        exit(2);
    }

    int fd;
    fd=open(argv[1],O_RDONLY | __O_DIRECTORY);
    if(fd<0){
        perror("open dir:");
        exit(3);
    }
    close(fd);
    char utente[200];
    int n_risultati; //lo metto int per fare il controllo, ma poi andrà convertito in stringa con sprintf
    char to_open[1024];

    int pid1,pid2,pid3;
    int p1p2[2],p2p3[2],p3p0[2];
    int nread;
    char buff[1024];

    signal(SIGINT,handler); //gestione modifica ctrl-C
    signal(SIGUSR1,handler2);   //gestione segnale SIGURS1 per P1
    while(1){

        printf("Inserisici un untente (fine per terminare):");
        scanf("%s", utente);

        if(strcmp(utente,"fine")==0){
            break;
        }

        sprintf(to_open, "%s/%s.txt", argv[1],utente);
        fd=open(to_open,O_RDONLY);
        if(fd<0){
            perror("open file:");
            exit(4);
        }
        close(fd);

        printf("Inserisci un numero di risultati da mostrare:");
        scanf("%d", &n_risultati);

        if(n_risultati<=0){ //controllo che n_risulati sia maggiore di 0
            printf("Il numero di risulatati da visualizzare deve essere almeno 1");
            exit(15);
        }

        if(pipe(p1p2)<0){
            perror("p1p2:");
            exit(5);
        }

        pid1=fork();
        if(pid1<0){
            perror("fork1:");
            exit(6);
        }
        if(pid1==0){
            //figlio P1
            pause();    //P1 si mette in pausa finchè non riceve il segnale SIGURS1 dal padre
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            close(p1p2[0]);

            execlp("sort", "sort", "-n", to_open,(char *)0);
            perror("sort:");
            exit(7);
        }
        //non mettere dopo la creazione di solo P1 il kill per la gestione di SIGUSR1

        if(pipe(p2p3)<0){
            perror("p2p3:");
            exit(8);
        }
        pid2=fork();
        if(pid2<0){
            perror("fork2:");
            exit(9);
        }
        if(pid2==0){
            //figlio P2
            signal(SIGUSR1,SIG_IGN);    //ignora SIGURS1 che è solo per P1
            close(0);
            dup(p1p2[0]);
            close(p1p2[1]);
            close(p1p2[0]);

            close(1);
            dup(p2p3[1]);
            close(p2p3[1]);
            close(p2p3[0]);

            execlp("grep", "grep", "-w", "NON RESTITUITO", (char *)0);  //elabora (leggendoli)i dati che sono in p1p2 e li scrive in p2p3
            perror("grep:");
            exit(10);
        
        }
        //sono nel padre chiudo le pipe che  non mi servono più
        close(p1p2[0]);
        close(p1p2[1]);

        if(pipe(p3p0)<0){
            perror("p3p0:");
            exit(11);
        }

        pid3=fork();
        if(pid3<0){
            perror("fork3:");
            exit(12);
        }
        if(pid3==0){
            //figlio P3
            char n_str[100];    //variabile per convertire numero risultati da vedere in stringa
            signal(SIGUSR1,SIG_IGN);    //ignora SIGURS1 che è solo per P1
            close(0);
            dup(p2p3[0]);
            close(p2p3[0]);
            close(p2p3[1]);

            close(1);
            dup(p3p0[1]);
            close(p3p0[1]);
            close(p3p0[0]);

            sprintf(n_str,"%d",n_risultati);   //converto perchè  execl vuole stringa

            execlp("tail","tail", "-n", n_str,(char *)0); //siccome ordino cronologico, i primi saranno i più vecchi, con tail prendo ultimi 5 che sono i più recenti. Leggo da p2p3, e mando a p3p0 verso il padre
        }
        
        
        //padre chiudo le pipe che non mi servono più
        close(p2p3[0]);
        close(p2p3[1]);
        close(p3p0[1]);

        //mando segnale a P1
        kill(pid1,SIGUSR1); //mettilo dopo la creazione di tutti 3 i figli

        //ora scrivo le cose che i figli mi han passato:

        while((nread= read(p3p0[0],buff,sizeof(buff)))>0){  //read >0 --> finchè leggi

            write(1,buff,nread);    //buff sarà di grandezza quanto letto--> nread
            memset(buff,0,sizeof(buff));//pulisco mettendo tutto a 0 buff (inutile)

        }
        //chiudo
        close(p3p0[0]);

        //terminazione figli
        
        wait(NULL);
        wait(NULL);
        wait(NULL);
        n_richieste++;

    }
    //uscita perchè utente ha inserito "fine":
    printf("\nSono state effettuate %d richieste\n",n_richieste);
    

    return 0;
}