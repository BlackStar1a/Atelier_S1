//@Salmane BABA - Abderrahmane OUARACH - Yassine NAJMI
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void node(int nodeid,int readfd,int writefd) {
    char token;
    while(1) {
        read(readfd, &token,1);
        printf("Noeud %d: jeton recue\n",nodeid);
        if(rand()%2) {
            printf("Noeud %d: envoi de données\n",nodeid);
            sleep(rand()%3+1);
        }
        printf("Noeud %d: passe le jeton\n",nodeid);
        write(writefd,"T",1);
    }
}

int main() {
    int numnodes;
    printf("Nombre de nœuds: ");
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

    printf("Simulation terminée\n");
    return 0;
}
