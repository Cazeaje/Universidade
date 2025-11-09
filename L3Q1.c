#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int chave;
    struct Node *esq;
    struct Node *dir;
} Node;

Node* novoNo(int chave) {
    Node* n = malloc(sizeof(Node));
    n->chave = chave;
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

Node* busca(Node* raiz, int chave) {
    if (raiz == NULL) return NULL;
    if (chave == raiz->chave) return raiz;
    if (chave < raiz->chave)
        return busca(raiz->esq, chave);
    else
        return busca(raiz->dir, chave);
}

Node* inserir(Node* raiz, int chave) {
    if (raiz == NULL)
        return novoNo(chave);
    if (chave < raiz->chave)
        raiz->esq = inserir(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = inserir(raiz->dir, chave);
    return raiz;
}

Node* sucessor(Node* raiz) {
    Node* atual = raiz;
    while (atual && atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

Node* remover(Node* raiz, int chave) {
    if (raiz == NULL) return NULL;
    if (chave < raiz->chave)
        raiz->esq = remover(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = remover(raiz->dir, chave);
    else {
        if (raiz->esq == NULL && raiz->dir == NULL) {
            free(raiz);
            return NULL;
        }
        if (raiz->esq == NULL) {
            Node* temp = raiz->dir;
            free(raiz);
            return temp;
        }
        if (raiz->dir == NULL) {
            Node* temp = raiz->esq;
            free(raiz);
            return temp;
        }
        Node* s = sucessor(raiz->dir);
        raiz->chave = s->chave;
        raiz->dir = remover(raiz->dir, s->chave);
    }
    return raiz;
}
