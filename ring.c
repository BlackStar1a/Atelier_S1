#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define MAX_NODES 10  // Nombre maximal de nœuds dans le réseau

// Structure pour représenter un nœud
typedef struct {
    int id;              // Identifiant du nœud
    bool has_token;      // Indique si le nœud a le jeton
    char data_to_send[50]; // Données à transmettre
} Node;

// Fonction pour créer un nœud
Node create_node(int id) {
    Node node;
    node.id = id;
    node.has_token = false;
    node.data_to_send[0] = '\0';  // Aucune donnée à transmettre initialement
    return node;
}

// Fonction pour passer le jeton au nœud suivant
void pass_token(Node *current_node, Node *next_node) {
    if (current_node->has_token) {
        current_node->has_token = false;
        next_node->has_token = true;
        printf("Le nœud %d a passé le jeton au nœud %d.\n", current_node->id, next_node->id);
    }
}

// Fonction pour transmettre des données
void transmit_data(Node *node) {
    if (node->has_token && node->data_to_send[0] != '\0') {
        printf("Le nœud %d transmet les données : '%s'\n", node->id, node->data_to_send);
        node->data_to_send[0] = '\0';  // Effacer les données après transmission
    } else if (node->has_token) {
        printf("Le nœud %d a le jeton mais n'a rien à transmettre.\n", node->id);
    }
}

// Fonction principale
int main() {
    srand(time(NULL));  // Initialisation du générateur de nombres aléatoires

    int num_nodes;
    printf("Entrez le nombre de nœuds dans le réseau (max %d) : ", MAX_NODES);
    scanf("%d", &num_nodes);

    if (num_nodes < 2 || num_nodes > MAX_NODES) {
        printf("Nombre de nœuds invalide. Le réseau doit avoir entre 2 et %d nœuds.\n", MAX_NODES);
        return 1;
    }

    // Création des nœuds
    Node nodes[MAX_NODES];
    for (int i = 0; i < num_nodes; i++) {
        nodes[i] = create_node(i);
    }

    // Le premier nœud reçoit le jeton initialement
    nodes[0].has_token = true;

    // Simulation du réseau Token Ring
    int current_node_index = 0;
    while (1) {
        Node *current_node = &nodes[current_node_index];

        // Simuler la transmission de données (aléatoire pour l'exemple)
        if (rand() % 2 == 0) {  // 50% de chance d'avoir des données à transmettre
            snprintf(current_node->data_to_send, sizeof(current_node->data_to_send), "Données du nœud %d", current_node->id);
        }

        // Transmettre les données si le nœud a le jeton
        transmit_data(current_node);

        // Passer le jeton au nœud suivant
        int next_node_index = (current_node_index + 1) % num_nodes;
        pass_token(current_node, &nodes[next_node_index]);

        // Mettre à jour l'indice du nœud courant
        current_node_index = next_node_index;

        // Pause pour simuler le temps de circulation du jeton
        sleep(2);
    }

    return 0;
}
