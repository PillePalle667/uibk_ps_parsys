#include <iostream>
#include <ctime>
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<omp.h>

struct Node
{
    int size;
    int key;
    struct Node *left;
    struct Node *right;
    int height;
};

int height( Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

void fixheight(Node* p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl>hr ? hl : hr) + 1;
}

int max(int a, int b)
{
    return (a > b)? a : b;
}

struct Node* newNode(int key)
{
    struct Node* node = (Node*) malloc(sizeof(Node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    node->size = node->size +1;
    return(node);
}

struct Node *rightRotate(Node *p)
{
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

struct Node *leftRotate( Node *p)
{
    Node* q = p->right;
    p->right = q->left;
    q->left = p;
    fixheight(p);
    fixheight(q);
    return q;
}

int getBalance(struct Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct Node* insertroot(struct Node* node, int key) // inserting a new node with  key in tree
{
    if( node == NULL) return(newNode(key)); ;
    if( key<node->key )
    {
        node->left = insertroot(node->left,key);
        node->size = node->size +1;
        return rightRotate(node);
    }
    else
    {
        node->right = insertroot(node->right,key);
        node->size = node->size +1;
        return leftRotate(node);
    }
}

struct Node* insert(struct Node* node, int key)
{

    /* Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));

     if(node ->size==0)
         return  insertroot(node, key);

    if (key < node->key) {
        #pragma omp task private(key) untied
        node->left = insert(node->left, key);
    }
    else if (key > node->key) {
        #pragma omp task private(key) untied
        node->right = insert(node->right, key);
    }
    else if(key == node->key) {
        #pragma omp taskwait
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    node->size = node->size +1;

    /* Get the balance factor of this ancestor node to check whether this node became unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    //1 Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    //2 Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    //3 Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }
    //4 Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    #pragma omp taskwait
    return node;
}

void preOrder(struct Node *root)
{
    if(root != NULL)
    {
        std::cout<<root->key<<" ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main(int argc, char** argv)
{
    struct Node *root;
    if(argc!=2)
        return EXIT_FAILURE;
    int n= atoi(argv[1]);
    if(n<=8)
        return EXIT_FAILURE;
   unsigned int N;
    std::clock_t c_start = std::clock();
  //  #pragma omp for
    for(int i= 0; i<n; i++){
       N = rand()%(n/8) +1;
       #pragma omp critical
       root = insert(root, N);
    }
    std::clock_t c_end = std::clock();
   // printf("Preorder traversal of the AVL tree is \n");
   // preOrder(root);

    long time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    std::cout << time_elapsed_ms;

  return 0;
}

