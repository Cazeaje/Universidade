#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef long long ll;

typedef struct Edge {
    int to;
    ll w;
    int idx;
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

void add_edge(Graph* g, int u, int v, ll w, int idx) {
    Edge* e = malloc(sizeof(Edge));
    e->to = v; e->w = w; e->idx = idx;
    e->next = g->adj[u];
    g->adj[u] = e;
}

typedef struct {
    int node;
    ll key;
    int from;
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
void heap_push(MinHeap* h, int node, ll key, int from) {
    int i = ++h->size;
    h->data[i].node = node; h->data[i].key = key; h->data[i].from = from;
    while (i>1) {
        int p = i/2;
        if (h->data[p].key <= h->data[i].key) break;
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
        if (l <= h->size && h->data[l].key < h->data[smallest].key) smallest = l;
        if (r <= h->size && h->data[r].key < h->data[smallest].key) smallest = r;
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
        add_edge(g, u, v, w, i);
        add_edge(g, v, u, w, i);
    }

    int *vis = calloc(n+1, sizeof(int));
    ll total = 0;
    // store edges of MST
    typedef struct { int u,v; ll w; } OutE;
    OutE *out = malloc(sizeof(OutE)*(n-1+5));
    int outcnt = 0;

    MinHeap* heap = heap_create(m*2 + 10);

    for (int start = 1; start <= n; ++start) {
        if (vis[start]) continue;
        // start Prim from this component
        heap_push(heap, start, 0, -1);
        while (!heap_empty(heap)) {
            HeapNode hn = heap_pop(heap);
            int node = hn.node;
            if (vis[node]) continue;
            vis[node] = 1;
            if (hn.from != -1) {
                total += hn.key;
                out[outcnt].u = hn.from;
                out[outcnt].v = node;
                out[outcnt].w = hn.key;
                outcnt++;
            }
            for (Edge* e = g->adj[node]; e; e = e->next) {
                if (!vis[e->to]) {
                    heap_push(heap, e->to, e->w, node);
                }
            }
        }
    }

    printf("TOTAL MST WEIGHT: %lld\n", total);
    printf("EDGES IN MST:\n");
    for (int i = 0; i < outcnt; ++i) {
        printf("%d %d %lld\n", out[i].u, out[i].v, out[i].w);
    }

    return 0;
}
