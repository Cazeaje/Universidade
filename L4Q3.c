#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef long long ll;
const ll INFLL = (1LL<<60);

typedef struct Edge {
    int to;
    ll w;
    struct Edge* next;
} Edge;

typedef struct {
    Edge** adj;
    int n;
} Graph;

Graph* create_graph(int n) {
    Graph* g = malloc(sizeof(Graph));
    g->n = n;
    g->adj = calloc(n+1, sizeof(Edge*));
    return g;
}

void add_edge(Graph* g, int u, int v, ll w) {
    Edge* e = malloc(sizeof(Edge));
    e->to = v; e->w = w;
    e->next = g->adj[u];
    g->adj[u] = e;
}

typedef struct {
    int node;
    ll dist;
} HeapNode;

typedef struct {
    HeapNode *data;
    int size, cap;
} MinHeap;

MinHeap* heap_create(int cap) {
    MinHeap* h = malloc(sizeof(MinHeap));
    h->data = malloc(sizeof(HeapNode)*(cap+5));
    h->size = 0; h->cap = cap+5;
    return h;
}
void heap_swap(HeapNode* a, HeapNode* b){ HeapNode t=*a; *a=*b; *b=t; }
void heap_push(MinHeap* h, int node, ll dist) {
    int i = ++h->size;
    h->data[i].node = node; h->data[i].dist = dist;
    while (i>1) {
        int p = i/2;
        if (h->data[p].dist <= h->data[i].dist) break;
        heap_swap(&h->data[p], &h->data[i]);
        i = p;
    }
}
int heap_empty(MinHeap* h){ return h->size==0; }
HeapNode heap_pop(MinHeap* h) {
    HeapNode ret = h->data[1];
    h->data[1] = h->data[h->size--];
    int i = 1;
    while (1) {
        int l = 2*i, r = 2*i+1, smallest = i;
        if (l <= h->size && h->data[l].dist < h->data[smallest].dist) smallest = l;
        if (r <= h->size && h->data[r].dist < h->data[smallest].dist) smallest = r;
        if (smallest == i) break;
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return ret;
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    Graph* g = create_graph(n);
    int u, v;
    long long w;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %lld", &u, &v, &w);
        add_edge(g, u, v, w);
    }
    int s;
    if (scanf("%d", &s) != 1) s = 1;

    ll *dist = malloc(sizeof(ll)*(n+1));
    int *vis = calloc(n+1, sizeof(int));
    for (int i = 1; i <= n; ++i) dist[i] = INFLL;
    dist[s] = 0;

    MinHeap* heap = heap_create(m*2 + 10);
    heap_push(heap, s, 0);

    while (!heap_empty(heap)) {
        HeapNode hn = heap_pop(heap);
        int node = hn.node;
        if (vis[node]) continue;
        vis[node] = 1;
        for (Edge* e = g->adj[node]; e; e = e->next) {
            if (dist[e->to] > dist[node] + e->w) {
                dist[e->to] = dist[node] + e->w;
                heap_push(heap, e->to, dist[e->to]);
            }
        }
    }

    printf("DISTANCES FROM %d:\n", s);
    for (int i = 1; i <= n; ++i) {
        if (dist[i] >= INFLL/4) printf("%d: INF\n", i);
        else printf("%d: %lld\n", i, dist[i]);
    }

    return 0;
}
