
#define CATCH_CONFIG_MAIN
#include "mpi.h"
#include <stdio.h>
#include <vector>
#include <ctime>
#include <cmath>
#include <time.h>
#include <iostream>
//#include "catch.hpp"
#define VALUE float
//lower and upper boundary values for the x, y, z axis
int N;

float edgeX0 = 0.0;
float edgeX1 = 0.0;
float edgeY0 = 0.0;
float edgeY1 = 0.0;
float normalizer= 0.0;
using stencil =std::vector<std::vector<VALUE>>;

stencil initStencil(int n){
	N = n;
	float f= 0.0;
	stencil stenzel;
	stenzel.resize(n+1);
	for(int i=0; i<n; i++){
		stenzel[i].resize(n+1);
		for(int j=0; j<n; j++)
			stenzel[i][j]=f;
	}
	return stenzel;
}


void printStencil(stencil &stenz){
	for(int i=0; i< N; i++){
		for(int j=0; j<N; j++){
			printf("%f\t",stenz[i][j]);
		}
		printf("\n");
	}
			
	
}
float transition(float a, float b, float c, float d){
	
	return 0.25* (a+b+c+d);
		
}

void normalize(stencil &stenz){
	float min = edgeX0;
	if(edgeX1< min)
		min = edgeX1;
	if(edgeY0< min)
		min = edgeY0;
	if(edgeY1< min)
		min = edgeY1;
	if(min >= 0.0)
			return;
	else{
		normalizer = std::abs(min);
		edgeX0+= normalizer;
		edgeX1+= normalizer;
		edgeY0+= normalizer;
		edgeY1+= normalizer;
		for(int i= 0; i< N; i++){
			for(int j =0; j< N; j++){
				stenz[i][j]+= normalizer;
			}
		}
		
		
	}		
	
	
}

void denormalize(stencil &stenz){
	for(int i= 0; i< N; i++){
			for(int j =0; j< N; j++){
				stenz[i][j]-= normalizer;
			}
		}
	
}
float computeStencil(int startx, int stopx, int starty, int stopy, stencil &stenz, stencil &b, int nodenum, int id, int count){
		printf("a by %d\n", id);
		//printf("values %d, %d\n", starty, stopy);
		float left[N]; 
		float leftr[N];
		float right[N];
		float rightr[N];
		if(id>0 )
        {	
			//for(int i= 0; i< N; i++){
					//left[i]= stenz[startx][i];
			//printf("Och nö\n");
            MPI_Send(&stenz[startx],
                                 N,
                                 MPI_FLOAT,
                                 id-1,
                                 1,
                                 MPI_COMM_WORLD);
            printf("post send pre rec by %d\n", id);                     
            MPI_Recv(&leftr,
                                N,
                                MPI_FLOAT,
                                id-1,
                                1,
                                MPI_COMM_WORLD,
                                MPI_STATUS_IGNORE);
			
        //  for(int i= 0; i< N; i++)
					 stenz[startx]= leftr;  
			printf("?\n");		                
            }
		
          printf("a/2 by %d \n", id);  							 
		if(id<(nodenum-1)){	
			for(int i= 0; i< N; i++){
				//	right[i]= stenz[stopx-1][i];				 
            MPI_Send(&stenz[stopx-1][i],
                                 1,
                                 MPI_FLOAT,
                                 id+1,
                                 1,
                                 MPI_COMM_WORLD);
          printf("post send pre rec by %d\n", id); 
          MPI_Recv(&rightr[i],
                                1,
                                MPI_FLOAT,
                                id+1,
                                1,
                                MPI_COMM_WORLD,
                                MPI_STATUS_IGNORE);
		}
          printf("There\n");                      
          for(int i= 0; i< N; i++)
					 stenz[stopx-1][i]= rightr[i]; 
                             
        
     }   
    printf("b by %d\n", id);
	float change=0.0;
	float tmp;
	int c= 0;
	
	for(int i= startx; i< stopx; i++){
		for(int j = starty; j< stopy; j++){
	
			c++;
			tmp= stenz[i][j];
			float lowx, highx, lowy, highy;
			(i-1 < 0) ? lowx = edgeX0 : lowx= stenz[i-1][j];
			(i+1 >= N) ? highx= edgeX1 : highx=stenz[i+1][j];
			(j-1 < 0) ? lowy= edgeY0 : lowy=stenz[i][j-1];
			(j+1 >= N) ? highy=edgeY1 : highy=stenz[i][j+1];
			b[i][j] = transition(lowx, highx, lowy, highy);
	
			float ch;
			(b[i][j]>tmp)? ch= b[i][j]-tmp : ch=tmp-b[i][j];
			
			change+= ch;
			
		}
	}
	
	for(int i= startx; i< stopx; i++){
		for(int j = starty; j< stopy; j++){
			stenz[i][j]= b[i][j];	
		}
	}	
	
	
	return change;	

}



d setEdges(float a, float b, float c, float d){
	edgeX0 = a;
	edgeX1 = b;
	edgeY0 = c;
	edgeY1 = d;
	
}

void iterate(float change_rate, stencil &stenz, int id, int nodenum){
		stencil b= initStencil(N);
		int n=0;
		while(n++ < 500)
			computeStencil(0,N,(id*(N/nodenum)), (id+1)*(N/nodenum), stenz, b, nodenum, id, n);//> (change_rate))
		//	n++;
		//finished[id]=true;	
		printf("%d Iterations by %d\n ", n, id);
		
}

int main(int argc, char* argv[]){
	stencil result;
    stencil stenz;
//	omp_set_num_threads(1);
	int n=512;
	setEdges(1.0, 0.0, -0.5, 0.5);
	
	
	//std::clock_t c_start = std::clock();
	int id;
	int nodenum;
	   
//normalize(stenz);
    MPI_Init(&argc, &argv);                 //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &id);           //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &nodenum);
   
   
		
    int recieved[nodenum];
    long recvniter[nodenum];
 //   MPI_Barrier(MPI_COMM_WORLD); 

	stenz = initStencil(n);	
			
	iterate(10.0, stenz, id, nodenum);
	//if(normalizer != 0.0)
	//		denormalize(stenz);	

//	MPI_Barrier(MPI_COMM_WORLD);	
		
	printf("Hello\n");
	result= initStencil(n);
		for(int i=0; i< N; i++){
			MPI_Reduce( &stenz[i],&result[i], N, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);	
	if(id==0){
		/**result= initStencil(n);
		for(int i=0; i< N; i++){
			
			MPI_Reduce( &result[i],&stenz[i], N, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);	
			*/
		printf("%f %f %f\n", result[10][10], result[387][215], result[502][7]);		
		//printf("Hello2\n");
		}
	}
	
			
		//printStencil(result);
		
		
	//}

	
 	//std::clock_t c_end = std::clock();
 	
 	
	//long time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    //    std::cout << time_elapsed_ms;
    MPI_Finalize();    
	return 0;
}
