#include <iostream>
#include <ctime>
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//#define CATCH_CONFIG_MAIN
//#include "catch.hpp"
omp_lock_t globalLock;
int uglycounter=0;
struct AVL{
	
	unsigned value;
	//struct AVL *parent;
	struct AVL *l;
	struct AVL *r;
	int depth;
	omp_lock_t lock;
	};

void freeNodes(AVL* avl){
	if(avl==NULL)
		return;
	freeNodes(avl->l);
	freeNodes(avl->r);
	omp_destroy_lock(&avl->lock);
	free(avl);	
	}
	
struct AVL *newNode( unsigned val){
	printf("New Node\n");
	struct AVL* avl = (struct AVL*) 
                        malloc(sizeof(struct AVL)); 
    avl->value   = val; 
    avl->l   = NULL; 
    avl->r  = NULL; 
	avl->depth = 1; 
	omp_init_lock(&avl->lock);
    return(avl); 
}

int depth(struct AVL *avl) { 
    if (avl == NULL) 
        return 0; 
    return avl->depth; 
} 
  
// A utility function to get maximum of two integers 
int max(int a, int b) { 
    return (a > b)? a : b; 
} 

struct AVL *rotateRight(struct AVL *avl){

    struct AVL *tmp = avl->l; 
    struct AVL *tmp2 = tmp->r; 
  
    // Perform rotation 
    tmp->r = avl; 
    avl->l = tmp2; 
  
    // Update depths 
    avl->depth = max(depth(avl->l), depth(avl->r))+1; 
    tmp->depth = max(depth(tmp->l), depth(tmp->r))+1; 
  
    // Return new root 
    return tmp; 
} 
struct AVL *rotateLeft(struct AVL *avl) 
{ 
    struct AVL *tmp = avl->r; 
    struct AVL *tmp2 = tmp->l; 
  
    // Perform rotation 
    tmp->l = avl; 
    avl->r = tmp2; 
  
    //  Update depths 
    avl->depth = max(depth(avl->l), depth(avl->r))+1; 
    tmp->depth = max(depth(tmp->l), depth(tmp->r))+1; 
  
    // Return new root 
    return tmp; 
} 

int getBalance(struct AVL *avl){ 
    if (avl == NULL) 
        return 0; 
    return depth(avl->l) - depth(avl->r); 
} 
void printTree(AVL *avl){
	printf("TREE\n");
	if(avl == NULL)
		;
	else{
		printf("TreeNode %u\n", avl->value);
		if(avl->l != NULL && avl->r != NULL){
			
			printTree(avl->l);
			printTree(avl->r);
		}
		else if(avl->l != NULL){
			printTree(avl->l);
		}
		else if(avl->r != NULL){
			printTree(avl->r);
		}
	}
}

AVL* insertAVL(struct AVL* avl, unsigned val) 
{ 
	bool isnew= false;
    /* 1.  Perform the normal BST insertion */
    #pragma omp critical
    if (avl == NULL){ 
        //printf("creating new node %u\n", val);
        isnew= true;
        avl= newNode(val); 
	}
	if(isnew)
		return avl;
	omp_set_lock(&avl->lock);
    if (val < avl->value){ 
       
		avl->l= insertAVL(avl->l, val);
		omp_unset_lock(&avl->lock);
	} 
    else if (val > avl->value){
		 
        avl->r= insertAVL(avl->r, val);
		omp_unset_lock(&avl->lock);
	
		
	}
    else {

		omp_unset_lock(&avl->lock);
        return avl; 
	}
	
    /* 2. Update depth of this ancestor node */
    avl->depth = 1 + max(depth(avl->l), 
                           depth(avl->r)); 
  
	
    omp_set_lock(&globalLock);
    int balance = getBalance(avl); 
  
    // If this node becomes unbalanced, then 
    // there are 4 cases 
	
    // Left Left Case 
    if (balance > 1 && val < avl->l->value){ 
			avl= rotateRight(avl); 
			
  }
    // Right Right Case 
    if (balance < -1 && val > avl->r->value){ 
			avl= rotateLeft(avl);
			 
  }
    // Left Right Case 
    if (balance > 1 && val > avl->l->value) 
    { 
        avl->l= rotateLeft(avl->l); 
        avl= rotateRight(avl); 
        
    } 
  
    // Right Left Case 
    if (balance < -1 && val < avl->r->value) 
    { 
        avl->r=rotateRight(avl->r); 
        
        avl= rotateLeft(avl);
        
        
    } 

    omp_unset_lock(&globalLock); 

    return avl; 
} 


bool find(unsigned value, AVL *avl){
	
	if(avl == NULL)
		return false;
	if(avl->value== value)
		return true;
	
	if(avl->value<value)
		return find(value, avl->r);		
	if(avl->value>value)
		return find(value, avl->l);	
	return false;
}
void testTree(int n){
	omp_init_lock(&globalLock);
	struct AVL *avl= NULL;
	unsigned N;
	std::clock_t c_start = std::clock();
	#pragma omp parallel for shared (avl)
    for(int i= 0; i<n; i++){
       N = rand()%(n/8) +1;
       if(!find(N, avl))
			//#pragma omp critical
			insertAVL(avl, N);
    }
    std::clock_t c_end = std::clock();
   

    long time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    std::cout << time_elapsed_ms;
	omp_destroy_lock(&globalLock);
	freeNodes(avl);
}
void getTreeValues(AVL *avl, unsigned *values){
	
	if(avl== NULL)
		return;
	values[uglycounter++]= avl->value;
	getTreeValues(avl->l, values);
	getTreeValues(avl->r, values);
		
	
}
void getTreeNodes(AVL *avl, AVL **avllist, int c){
	if(avl==NULL)
		return;
	avllist[c++]=avl;
	getTreeNodes(avl->l, avllist, c);
	getTreeNodes(avl->r, avllist, c);	
	
}
/*
TEST_CASE( "TreeStructure is tested", "öhm" ) {
	int n= 100;
	omp_init_lock(&globalLock);
	unsigned values[n];
	struct AVL *avl = NULL;
	srand(time(NULL));
	for(int i =0; i<n; i++){
		values[i]= (rand())%(n/8);
		printf("%u\n", values[i]);
	}
	//avl=newNode(values[0]);
	#pragma omp parallel for shared (avl, values)	
	for(int i =0; i<n; i++){
		//if(!find(values[i], avl)){
			avl= insertAVL(avl, values[i]);
		}
		
	printTree(avl);
	for(int i=0; i< n; i++)
		REQUIRE( find(values[i], avl) == true );
	if(avl->l != NULL)
		REQUIRE(avl->l->value < avl->value);
	if(avl->r != NULL)
		REQUIRE(avl->r->value > avl->value);
	unsigned TreeValues[n];
	for(int i=0; i<n; i++)
		TreeValues[i]= 42; 
	getTreeValues(avl, TreeValues);
	for(int i=0; i<n; i++)
		for(int j=0; j<n; j++)
			if(i != j && TreeValues[i] != 42)
				REQUIRE(TreeValues[i]!= TreeValues[j]);
	AVL* nodes[n];
	
	getTreeNodes(avl, nodes, 0);
	
	for(int i=0; i<n; i++){
		if(nodes[i]==NULL)
			break;
		if(nodes[i]->l != NULL){
			REQUIRE(nodes[i]->value > nodes[i]->l->value);
		}
		if(nodes[i]->r != NULL){
			REQUIRE(nodes[i]->value < nodes[i]->r->value);
		}
		REQUIRE((1>=getBalance(nodes[i])&&getBalance(nodes[i])>=-1) == true);
			
	}
	freeNodes(avl);
	omp_destroy_lock(&globalLock);
}

*/
int main(int argc, char** argv) {
	if(argc!=2) 
		return EXIT_FAILURE;
	int n= atoi(argv[1]);
	if(n<=0)
			return EXIT_FAILURE;
	
	testTree(n);
	
		
	
	return EXIT_SUCCESS;
		
}

