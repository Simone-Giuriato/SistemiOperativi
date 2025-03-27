#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


int main(int argc, char **argv[]){
    
    int numero_stringhe=0;
    printf("Quante stringhe vuoi cercare nel file?");
    scanf("%d", &numero_stringhe);

    if(argc!=(2+numero_stringhe)){  //file name(questo), filename(dove operare), stringa 1, stringa 2..
        printf("Errore nell'inserimento dei parametri");
        exit(1);
    }
    int pid=0;
    int status;

    for(int i=2; i<argc; i++){   //per ogni stringa come parametro, creo un figlio che lavori con quella stringa
        //il for parte da 2 perche i primi 2 parametri sono il nome di questo file e il nome del file dove operare
        pid=fork();
        if(pid<0){
            printf("Errore:\n");
            exit(2);
        }else if(pid==0){
            //FIGLIO
            execlp("grep", "grep", "-c", argv[i], argv[1], (char*)0 );
            perror("Errore exec:");
            exit(3);
        }else if(pid>0){
            //PADRE
            wait(&status);
        }
    
    }
    return 0;
}

//si specifica che la consegna non mi richiede di verificare se il file esiste... o quindi di aprirlo e controllare se da errori

