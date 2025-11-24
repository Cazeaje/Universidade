#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

Node* newNode(int key) {
    Node* n = (Node*) malloc(sizeof(Node));
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

int height(Node* r) {
    if (!r) return 0;
    int hl = height(r->left);
    int hr = height(r->right);
    return (hl > hr ? hl : hr) + 1;
}

int fb(Node* r) {
    if (!r) return 0;
    return height(r->right) - height(r->left);
}

void inorder_with_fb(Node* r, char* buf, size_t bufsize) {
    if (!r) return;
    inorder_with_fb(r->left, buf, bufsize);
    char part[64];
    int f = fb(r);
    if (f > 0) snprintf(part, sizeof(part), "%d(+%d) ", r->key, f);
    else if (f == 0) snprintf(part, sizeof(part), "%d(0) ", r->key);
    else snprintf(part, sizeof(part), "%d(%d) ", r->key, f);
    if (strlen(buf) + strlen(part) + 1 < bufsize) strcat(buf, part);
    inorder_with_fb(r->right, buf, bufsize);
}

Node* bst_insert(Node* root, int key) {
    if (!root) return newNode(key);
    if (key < root->key) root->left = bst_insert(root->left, key);
    else if (key > root->key) root->right = bst_insert(root->right, key);
    return root;
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
}
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
}

Node* find_responsible(Node* root, int key) {
    Node* cur = root;
    Node* responsible = NULL;
    while (cur) {
        int f = fb(cur);
        if (abs(f) > 1 && responsible == NULL) responsible = cur;
        if (key < cur->key) cur = cur->left;
        else if (key > cur->key) cur = cur->right;
        else break;
    }
    return responsible;
}

int decide_rotation_type(Node* resp) {
    int rfb = fb(resp);
    if (rfb < -1) {
        Node* L = resp->left;
        if (!L) return 1;
        int lfb = fb(L);
        if (lfb <= 0) return 1;
        else return 3;
    } else if (rfb > 1) {
        Node* R = resp->right;
        if (!R) return 2;
        int rchildfb = fb(R);
        if (rchildfb >= 0) return 2;
        else return 4;
    }
    return 0;
}

Node* apply_rotation_at(Node* root, int targetKey, int rotationType) {
    if (!root) return NULL;
    if (targetKey < root->key) {
        root->left = apply_rotation_at(root->left, targetKey, rotationType);
        return root;
    } else if (targetKey > root->key) {
        root->right = apply_rotation_at(root->right, targetKey, rotationType);
        return root;
    } else {
        if (rotationType == 1) return rotateRight(root);
        else if (rotationType == 2) return rotateLeft(root);
        else if (rotationType == 3) { if (root->left) root->left = rotateLeft(root->left); return rotateRight(root);}
        else if (rotationType == 4) { if (root->right) root->right = rotateRight(root->right); return rotateLeft(root);}
    }
    return root;
}

void print_inorder_fb(Node* root) {
    char buf[20000];
    buf[0] = '\0';
    inorder_with_fb(root, buf, sizeof(buf));
    printf("%s", buf);
}

void process_dataset(int keys[], int m) {
    Node* root = NULL;
    for (int i = 0; i < m; ++i) {
        int k = keys[i];
        root = bst_insert(root, k);
        Node* resp = find_responsible(root, k);
        if (!resp) {
            printf("arvore ja balanceada. ");
            print_inorder_fb(root);
            printf("%d\n", height(root));
        } else {
            int respKey = resp->key;
            printf("no responsavel: %d\n", respKey);
            print_inorder_fb(root);
            printf("\n");
            int rtype = decide_rotation_type(resp);
            if (rtype == 1) printf("rotacao direita.\n");
            else if (rtype == 2) printf("rotacao esquerda.\n");
            else if (rtype == 3) printf("rotacao direita dupla.\n");
            else if (rtype == 4) printf("rotacao esquerda dupla.\n");
            root = apply_rotation_at(root, respKey, rtype);
            print_inorder_fb(root);
            printf("\n");
            printf("%d\n", height(root));
        }
    }
}

int parse_line_ints(char* line, int arr[]) {
    int cnt = 0;
    char* tok = strtok(line, " \t\r\n");
    while (tok) {
        int v = atoi(tok);
        arr[cnt++] = v;
        tok = strtok(NULL, " \t\r\n");
    }
    return cnt;
}

int main() {
    char line[10000];
    int arr[2000];
    while (fgets(line, sizeof(line), stdin)) {
        char *p = line;
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
        if (*p == '\0') continue;
        char tmp[10000];
        strncpy(tmp, line, sizeof(tmp)-1);
        tmp[sizeof(tmp)-1] = '\0';
        int n = parse_line_ints(tmp, arr);
        if (n > 0) {
            process_dataset(arr, n);
            printf("\n");
        }
    }
    printf("\n");
    return 0;
}
