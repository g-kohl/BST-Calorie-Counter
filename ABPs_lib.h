//STRUCTURES:
typedef struct food{
    char name[50];
    int calories;
} FOOD;

typedef struct nodeAVL{
    FOOD food;
    int height;
    struct nodeAVL *left;
    struct nodeAVL *right;
} nodeAVL;

typedef struct nodeBST{
    FOOD node;
    struct nodeBST *left;
    struct nodeBST *right;
} nodeBST;

//GENERIC FUNCTIONS:
char* strlwr(char* str);

void printTree(nodeBST *root);

int maximum(int a, int b);

//AVL FUNCTIONS:
nodeAVL* createAVL();

int AVL_nodeHeight(nodeAVL* n);

int AVL_getBalance(nodeAVL* n);

nodeAVL* AVL_rightRotate(nodeAVL* y);

nodeAVL* AVL_leftRotate(nodeAVL* x);

nodeAVL* insertAVL(nodeAVL* node, FOOD newFood);

nodeAVL* consultAVL(nodeAVL* node, char* key);

//BST FUNCTIONS:
nodeBST* createBST();

nodeBST* insertBST(nodeBST *root, FOOD newFood);

nodeBST* consultBST(nodeBST *root, char *foodName);

int heightBST(nodeBST *root);

//SPLAY FUNCTIONS:
nodeBST* insertSplay(nodeBST *root, FOOD newFood);

nodeBST* splaying(nodeBST *root, char *foodName);

nodeBST* leftRotate(nodeBST *root);

nodeBST* rightRotate(nodeBST *root);

