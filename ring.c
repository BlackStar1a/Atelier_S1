//@Salmane BABA - Abderrahmane OUARACH - Yassin NAJMI
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void node(int nodeid,int readfd,int writefd) {
    char token;
    while(1) {
        read(readfd, &token,1);
        printf("Noeud %d: jeton reçu\n",nodeid);
        if(rand()%2) {
            printf("Noeud %d : transmission de trame\n",nodeid);
            sleep(rand()%3+1);
        }
        printf("Noeud %d : passe le jeton au noeud suivant \n",nodeid);
        write(writefd,"T",1);
    }
}

int main() {
    int numnodes;
    printf("Entrez le nombre de noeuds dans le réseau :  ");
    scanf("%d",&numnodes);

    int pipes[numnodes][2];
    for(int i=0;i<numnodes; i++) pipe(pipes[i]);

    srand(time(NULL));
    for(int i=0; i<numnodes; i++) {
        int pid=fork();
        if(pid==0) {
            for(int j=0; j<numnodes; j++) {
                if(j!=i) close(pipes[j][0]);
                if(j!=(i+1)%numnodes) close(pipes[j][1]);
            }
            node(i+1,pipes[i][0],pipes[(i+1)%numnodes][1]);
            exit(0);
        }
    }

    write(pipes[0][1],"T",1);
    for(int i=0; i<numnodes; i++) close(pipes[i][0]), close(pipes[i][1]);
    for(int i=0; i<numnodes; i++) wait(NULL);
    return 0;
}
