#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/header.h"

Vertice* createVertice(Vertice* head, char id[], char name[]) {
    if (existVertice(head, id)) return head;

    Vertice* new = malloc(sizeof(struct vertice));

    if (new == NULL) return 0;

    strcpy(new->id, id);
    strcpy(new->name, name);
    new->adjacents = NULL;
    new->next = NULL;

    if (head == NULL) return new;

    Vertice* aux = head;

    while (aux->next != NULL) aux = aux->next;
    aux->next = new;

    return head;
}

int existVertice(Vertice* head, char id[]) {
    while (head != NULL) {
        if (strcmp(head->id, id) == 0) return 1;

        head = head->next;
    }

    return 0;
}

Vertice* createEdge(Vertice* head, char origin[], char destination[], float distance) {
    if (!existVertice(head, origin) || !existVertice(head, destination)) return 0;

    while (strcmp(head->id, origin) != 0) head = head->next;

    Adjacent* new = malloc(sizeof(struct adjacent));

    if (new == NULL) return 0;

    strcpy(new->id, destination);
    new->distance = distance;
    new->next = NULL;

    if (head->adjacents == NULL) {
        head->adjacents = new;
        return head;
    }

    Adjacent* aux = head->adjacents;

    while (aux->next != NULL) aux = aux->next;
    aux->next = new;

    return head;
}

void listAdjacents(Vertice* head, char id[]) {
    if (existVertice(head, id)) {
        while (strcmp(head->id, id) != 0) head = head->next;
        Adjacent* aux = head->adjacents;
        while (aux != NULL) {
            printf("Localizacao: %s | Distancia: %.3f km\n", aux->id, aux->distance);
            aux = aux->next;
        }
    }
}

Vertice* createGraph() {
    Vertice* v = NULL;

    v = createVertice(v, "tatica.ideia.morno", "Volt");
    v = createVertice(v, "chia.tigela.palmitos", "Escola Secundaria de Barcelos");
    v = createVertice(v, "conta.mumias.bolota", "Central Camionagem Barcelos");
    v = createVertice(v, "entrelinha.resolvidas.fixo", "Instituto Politecnico do Cavado e do Ave (IPCA)");
    v = createVertice(v, "evolucao.adido.vitorioso", "Estadio Cidade de Barcelos");
    v = createVertice(v, "regua.vinco.velho", "Piscinas Municipais de Barcelos");
    v = createVertice(v, "tirei.manhas.lagoa", "Pavilhao Municipal de Barcelos");
    v = createVertice(v, "tropa.ocio.garota", "Parque Municipal de Barcelos");
    v = createVertice(v, "uteis.margem.forem", "Estacao CP - Barcelos");

    v = createEdge(v, "tatica.ideia.morno", "chia.tigela.palmitos", 1.9F);
    v = createEdge(v, "tatica.ideia.morno", "conta.mumias.bolota", 1.7F);
    v = createEdge(v, "tatica.ideia.morno", "entrelinha.resolvidas.fixo", 1.2F);
    v = createEdge(v, "tatica.ideia.morno", "evolucao.adido.vitorioso", 2.9F);
    v = createEdge(v, "tatica.ideia.morno", "regua.vinco.velho", 1.6F);
    v = createEdge(v, "tatica.ideia.morno", "tirei.manhas.lagoa", 1.1F);
    v = createEdge(v, "tatica.ideia.morno", "tropa.ocio.garota", 1.0F);
    v = createEdge(v, "tatica.ideia.morno", "uteis.margem.forem", 1.7F);

    return v;
}