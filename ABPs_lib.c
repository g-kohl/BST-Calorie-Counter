#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include "ABPs_lib.h"

// GLOBAL VARIABLES:
int comp_AVL=0, comp_BST=0, comp_SPLAY=0; // contador de comparacoes em AVL, ABP e Splay
int rotations_AVL=0, rotations_SPLAY=0; // contador de rotacoes em AVL e Splay

// GENERIC FUNTIONS:
char* strlwr(char* str){
    if(!str)
        return NULL;

    for (char* ptr = str; *ptr; ptr++){ // percorre os caracteres tornando cada um minusculo
        *ptr = tolower(*ptr);
    }
    return str;
}

void printTree(nodeBST *root){
    if(root != NULL){ // printa a sub-arvore esquerda, depois a raiz e por fim, a sub-arvore direita
        printTree(root->left);
        printf("%s\n", root->node.name);
        printTree(root->right);
    }
}

// funcao auxiliar que retorna o maximo de dois inteiros
int maximum(int a, int b){
    return a > b ? a : b;
}

//AVL FUNCTIONS:
nodeAVL* createAVL(){
    return NULL;
}


// retorna a altura do nodo dado
int AVL_nodeHeight(nodeAVL* n){
  if (!n) return 0;
  return n->height;
}


// retorna o fator de balanceamento do nodo dado
int AVL_getBalance(nodeAVL* n){
  if (!n) return 0;
  return AVL_nodeHeight(n->left) - AVL_nodeHeight(n->right);
}


// rotacao para direita da avl
nodeAVL* AVL_rightRotate(nodeAVL* y){
    nodeAVL* x = y->left;
    nodeAVL* T2 = x->right;

    // altera os ponteiros para fazer a rotacao
    x->right = y;
    y->left = T2;

    // atualiza as novas alturas
    y->height = 1 + maximum(AVL_nodeHeight(y->left),
                            AVL_nodeHeight(y->right));
    x->height = 1 + maximum(AVL_nodeHeight(x->left),
                            AVL_nodeHeight(x->right));

    // rotations_AVL++;
    // retorna a nova raiz
    return x;
}


// rotacao para esquerda da avl
nodeAVL* AVL_leftRotate(nodeAVL* x){
    nodeAVL* y = x->right;
    nodeAVL* T2 = y->left;

    // altera os ponteiros para fazer a rotacao
    y->left = x;
    x->right = T2;

    // atualiza as novas alturas
    x->height = 1 + maximum(AVL_nodeHeight(x->left),
                            AVL_nodeHeight(x->right));
    y->height = 1 + maximum(AVL_nodeHeight(y->left),
                            AVL_nodeHeight(y->right));

    // rotations_AVL++;
    // retorna a nova raiz
    return y;
}



nodeAVL* insertAVL(nodeAVL* node, FOOD newFood){
    // se a arvore for vazia, adiciona o nodo
    if (!node){
        nodeAVL* newNode = malloc(sizeof(nodeAVL));
        newNode->right = NULL;
        newNode->left = NULL;
        newNode->height = 1;
        strcpy(newNode->food.name, newFood.name);
        newNode->food.calories = newFood.calories;

        return newNode;
    }

    // ordem lexicografica da raiz em relacao ao nodo a ser adicionado
    int order = strcmp(newFood.name, node->food.name);

    // insercao em bst
    if (order < 0){
        node->left = insertAVL(node->left, newFood);
    }
    else if (order > 0){
        node->right = insertAVL(node->right, newFood);
    }
    else return node;

    // atualizar altura apos a insercao
    node->height = 1 + maximum(AVL_nodeHeight(node->left),
                               AVL_nodeHeight(node->right));

    // fator de balanceamento do nodo
    int balance = AVL_getBalance(node);

    // quatro casos de desbalanceamento
    // caso rotacao simples para direita
    if (balance > 1 && strcmp(newFood.name, node->left->food.name) < 0){
        rotations_AVL++;
        return AVL_rightRotate(node);
    }

    // caso rotacao simples para esquerda
    if (balance < -1 && strcmp(newFood.name, node->right->food.name) > 0){
        rotations_AVL++;
        return AVL_leftRotate(node);
    }

    // caso rotacao dupla direita
    if (balance > 1 && strcmp(newFood.name, node->left->food.name) > 0){
        rotations_AVL++;
        node->left = AVL_leftRotate(node->left);
        return AVL_rightRotate(node);
    }

    // caso rotacao dupla esquerda
    if (balance < -1 && strcmp(newFood.name, node->right->food.name) < 0){
        rotations_AVL++;
        node->right = AVL_rightRotate(node->right);
        return AVL_leftRotate(node);
    }

    return node;
}


nodeAVL* consultAVL(nodeAVL* node, char* key){
    if (!key) return NULL;

    while (node){
        comp_AVL++;

        if (strcmp(node->food.name, key) == 0){
            return node;
        }
        if (strcmp(node->food.name, key) > 0){
            node = node->left;
        }
        else{
            node = node->right;
        }
    }
    return NULL;
}


//BST FUNCTIONS:
nodeBST* createBST(){
    return NULL;
}

nodeBST* insertBST(nodeBST *root, FOOD newFood){
    if(root == NULL){ // se a arvore esta vazia, aloca memoria para o novo nodo e o torna a raiz da arvore
        root = (nodeBST*)malloc(sizeof(nodeBST));
        root->right = NULL;
        root->left = NULL;
        root->node = newFood;
    }
    else if(strcmp(newFood.name, root->node.name) > 0) // se o nome do alimento suceder lexicograficamente o nome da raiz, insere o nodo na direita
        root->right = insertBST(root->right, newFood);
    else // caso o contrario, insere na esquerda
        root->left = insertBST(root->left, newFood);

    return root;
}

nodeBST* consultBST(nodeBST *root, char *foodName){
    if(foodName == NULL)
        return NULL;

    while(root != NULL){
        comp_BST ++;

        if(strcmp(root->node.name, foodName) == 0) // se o nome do alimento for igual ao da raiz, retorna a raiz
            return root;
        else{
            if(strcmp(root->node.name, foodName) > 0) // se o nome do alimento antecer lexicograficamente o nome da raiz, procura na sub-arvore direita
                root = root->left;
            else // caso o contrario, procura na sub-arvore direita
                root = root->right;
        }
    }
    return NULL;
}

int heightBST(nodeBST *root){
    int leftHeight, rightHeight;

    if(root){
        leftHeight = heightBST(root->left);
        rightHeight = heightBST(root->right);

        return (leftHeight > rightHeight ? leftHeight+1 : rightHeight+1); // se a altura esquerda for maior que a direita, retorna a altura esquerda. caso o contrario, retorna a altura direita
    }

    return 0;
}

 // SPLAY FUNCTIONS:
nodeBST* insertSplay(nodeBST *root, FOOD newFood){
    nodeBST *newNode;

    if(root == NULL){ // se a arvore esta vazia, aloca memoria para o novo nodo e o torna a raiz da arvore
        root = (nodeBST*)malloc(sizeof(nodeBST));
        root->right = NULL;
        root->left = NULL;
        root->node = newFood;

        return root;
    }

    root = splaying(root, newFood.name); // efetua o splaying do nodo a ser inserido, isso retornara o nodo mais proximo do novo nodo como raiz

    if(strcmp(root->node.name, newFood.name) == 0) // se a nova raiz for igual ao novo nodo, retorna a raiz
        return root;

    newNode = (nodeBST*)malloc(sizeof(nodeBST)); // aloca memoria para o novo nodo
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->node = newFood;

    if(strcmp(root->node.name, newFood.name) > 0){ // se o nome do alimento suceder lexicograficamente o nome da raiz, torna a raiz seu filho direito
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    }
    else{ // caso o contraio, torna a raiz seu filho esquerdo
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    return newNode;
}

nodeBST* splaying(nodeBST *root, char *foodName){
    if(foodName == NULL)
        return NULL;

    if(root == NULL || strcmp(root->node.name, foodName) == 0) // se a arvore esta vazia ou se o nome da raiz e igual a chave, retorna a raiz
        return root;

    comp_SPLAY ++;

    if(strcmp(foodName, root->node.name) > 0){ // se o nome do alimento suceder lexicograficamente o nome da raiz...
        if(root->right == NULL) // se a raiz nao possui filho direito, retorna a raiz
            return root;

        if(strcmp(foodName, root->right->node.name) > 0){ // se o nome do alimento suceder lexicograficamente o nome do filho direito da raiz...
            root->right->right = splaying(root->right->right, foodName); // faz o splaying agora com a sub-arvore direita-direita
            root = leftRotate(root); // efetua uma rotacao a esquerda
        }
        else{ // caso o contrario...
            root->right->left = splaying(root->right->left, foodName); // faz o splaying agora com a sub-arvore direita-esquerda

            if(root->right->left != NULL) // se existir o filho direito-esquerdo, efetua uma rotacao a direita
                root->right = rightRotate(root->right);
        }

        return (root->right == NULL ? root : leftRotate(root)); // se o filho direito nao existir, retorna a raiz. caso o contrario, efetua maiz uma rotacao a esquerda e retorna a raiz
    }
    else{ // caso o contrario...
        if(root->left == NULL) // se a raiz nao possui filho esquerdo, retorna a raiz
            return root;

        if(strcmp(foodName, root->left->node.name) > 0){ // se o nome do alimento suceder lexicograficamente o nome do filho direito da raiz...
            root->left->right = splaying(root->left->right, foodName);

            if(root->left->right != NULL) // se existir o filho esquerdo-direito, efetua uma rotacao a esquerda
                root->left = leftRotate(root->left);
        }
        else{ // caso o contrario...
            root->left->left = splaying(root->left->left, foodName); // faz o splaying agora com a sub-arvore esquerda-esquerda
            root = rightRotate(root); // efetua uma rotacao a direita
        }

        return (root->left == NULL ? root : rightRotate(root)); // se o filho esquerdo nao existir, retorna a raiz. caso o contrario, efetua maiz uma rotacao a direita e retorna a raiz
    }
}

nodeBST* leftRotate(nodeBST *root){
    nodeBST *rightSon=root->right;

    root->right = rightSon->left; // filho direito recebe filho direito-esquerdo
    rightSon->left = root; // filho direito-esquerdo recebe raiz

    rotations_SPLAY ++;
    return rightSon; // retorna filho direito como nova raiz
}

nodeBST* rightRotate(nodeBST *root){
    nodeBST *leftSon=root->left;

    root->left = leftSon->right; // filho esquerdo recebe filho esquerdo-direito
    leftSon->right = root; // filho esquerdo-direito recebe raiz

    rotations_SPLAY ++;
    return leftSon; // retorna filho esquerdo como nova raiz
}

