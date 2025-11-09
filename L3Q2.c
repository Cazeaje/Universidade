#include <stdio.h>
#include <stdlib.h>

#define VAZIO -1

typedef struct NoLista {
    int chave;
    struct NoLista* prox;
} NoLista;

NoLista* novoNoLista(int chave) {
    NoLista* n = malloc(sizeof(NoLista));
    n->chave = chave;
    n->prox = NULL;
    return n;
}

void inserirLista(NoLista** lista, int chave) {
    NoLista* n = novoNoLista(chave);
    n->prox = *lista;
    *lista = n;
}

typedef struct Hash {
    int m;
    int* tabela;
    NoLista* overflowList;
    int ocupados;
} Hash;

Hash* criarHash(int m) {
    Hash* h = malloc(sizeof(Hash));
    h->m = m;
    h->ocupados = 0;
    h->tabela = malloc(sizeof(int) * m);
    for (int i = 0; i < m; i++)
        h->tabela[i] = VAZIO;
    h->overflowList = NULL;
    return h;
}

int hash(int k, int m) {
    return k % m;
}

void inserirHash(Hash* H, int chave) {
    int m = H->m;
    if (H->ocupados < m) {
        int pos = hash(chave, m);
        while (H->tabela[pos] != VAZIO)
            pos = (pos + 1) % m;
        H->tabela[pos] = chave;
        H->ocupados++;
        return;
    }
    inserirLista(&(H->overflowList), chave);
}
