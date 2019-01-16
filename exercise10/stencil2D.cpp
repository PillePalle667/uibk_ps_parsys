
#define CATCH_CONFIG_MAIN
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
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
	stenzel.resize(n);
	for(int i=0; i<n; i++){
		stenzel[i].resize(n);
		for(int j=0; j<n; j++)
			stenzel[i][j]=f;
	}
	return stenzel;
}


void printStencil(stencil &stenz, int id){
	for(int i=0; i< N; i++){
		for(int j=0; j<N; j++){
			printf("%f %d\t",stenz[i][j], id);
		}
		printf("\n");
	}
	printf("\n\n");		
	
}
/**
void freeStencil(stencil &stenz){
	for(int i=0; i< stenz.size(); i++)
		delete stenz[i];
	delete stenz;	
	
	
}*/
float transition(float a, float b, float c, float d, float e){
	
	return (a+b+c+d+e)/5;
		
}
/**
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
	
}*/
float computeStencil(int startx, int stopx, int starty, int stopy, stencil &stenz, stencil &b, int nodenum, int id, int perRow){
	//  printf("startx: %d stopx: %d starty: %d stopy: %d id: %d\n", startx, stopx, starty, stopy, id);
	  if(id== nodenum-1){
			stopx= N;
			stopy=N;
		}
	  int idOffset;
	  nodenum <= 3 ? idOffset= 1: idOffset= (nodenum/perRow);				
			//printf("Och nö\n");
      if(startx >0 ){	
			float buf[N];
			for(int i=0; i<N; i++)
				buf[i]= stenz[startx][i];
            MPI_Send(&buf,
                                 N,
                                 MPI_FLOAT,
                                 id-1,
                                 0,
                                 MPI_COMM_WORLD);
         
	   }
                                
        //    printf("post send pre rec by %d\n", id);
         if(startx >0 ){ 
			float buf[N];                  
            MPI_Recv(&buf,
                                N,
                                MPI_FLOAT,
                                id-1,
                                0,
                                MPI_COMM_WORLD,
                                MPI_STATUS_IGNORE);
			for(int i =0; i< N; i++)
				b[startx-1][i]= buf[i];
        }                        
		//	printf("?\n");	*/	                
            	
     //    printf("a/2 by %d \n", id);  							 
		if(stopx <N){	
			float buf[N];
			for(int i=0; i<N; i++)
				buf[i]= stenz[stopx-1][i];
            MPI_Send(&buf,
                                 N,
                                 MPI_FLOAT,
                                 id+1,
                                 0,
                                 MPI_COMM_WORLD);
							 
						 }
         // printf("post send pre rec by %d\n", id); 
        if(stopx <N){ 
			float buf[N]; 
			MPI_Recv(&buf,
                                N,
                                MPI_FLOAT,
                                id+1,
                                0,
                                MPI_COMM_WORLD,
                                MPI_STATUS_IGNORE);
            for(int i=0; i< N; i++)
				b[stopx][i]=buf[i];					
		}
	//   printf("checkpoint by %d\n", id);
       if(starty >0 ){	
			float buf[N];
			for(int i=0; i<N; i++)
				buf[i]= stenz[i][starty];
            MPI_Send(&buf,
                                 N,
                                 MPI_FLOAT,
                                 id-idOffset,
                                 0,
                                 MPI_COMM_WORLD);
         
	   }
                                
        //    printf("post send pre rec by %d\n", id);
         if(starty >0 ){ 
			float buf[N];                  
            MPI_Recv(&buf,
                                N,
                                MPI_FLOAT,
                                id-idOffset,
                                0,
                                MPI_COMM_WORLD,
                                MPI_STATUS_IGNORE);
			for(int i =0; i< N; i++)
				b[i][starty-1]= buf[i];
        }                        
		//	printf("?\n");	*/	                
            	
        //  printf("a/2 by %d \n", id);  							 
		if(stopy <N){	
			float buf[N];
			for(int i=0; i<N; i++)
				buf[i]= stenz[i][stopy-1];
		//	printf("in send %d\n", id);	
            MPI_Send(&buf,
                                 N,
                                 MPI_FLOAT,
                                 id+idOffset,
                                 0,
                                 MPI_COMM_WORLD);
							 
						 }
       //  printf("post send pre rec y by %d\n", id); 
        if(stopy <N){ 
			float buf[N]; 
			MPI_Recv(&buf,
                                N,
                                MPI_FLOAT,
                                id+idOffset,
                                0,
                                MPI_COMM_WORLD,
                                MPI_STATUS_IGNORE);
            for(int i=0; i< N; i++)
				b[i][stopy]=buf[i];					
		}
        
        
   // printf("b by %d\n", id);
	float change=0.0;
	float tmp;
	int c= 0;
	
	//printf("id: %d edgeX0: %f edgeX1: %f\n", id, edgeX0, edgeX1);
	for(int i= startx; i< stopx; i++){
		for(int j = starty; j< stopy; j++){
			//printf("id: %d i: %d j: %d val: %f\n", id, i, j, stenz[i][j]);
			c++;
			tmp= stenz[i][j];
			float lowx, highx, lowy, highy;
			if(i-1 < 0)
				lowx= edgeX0;
			else if(i-1 < startx)
				lowx= b[i-1][j];
			else
				lowx= stenz[i-1][j];
			if(i+1 >= N)
				highx= edgeX1;
			else if(i+1 >= stopx)
				highx= b[i+1][j];
			else
				highx= stenz[i+1][j];
			if(j-1 < 0)
			    lowy= edgeY0;
			else if(j-1 < starty)
			    lowy= b[i][j-1];
			else
			    lowy= stenz[i][j-1];
			if(j+1 >= N)
			    highy= edgeY1;
			else if(j+1 >= stopy)
			    highy= b[i][j+1];
			else
			    highy= stenz[i][j+1];
	
			b[i][j] = transition(lowx, highx, lowy, highy, tmp);
	
			float ch;
			//(b[i][j]>tmp)? ch= b[i][j]-tmp : ch=tmp-b[i][j];
			ch= abs(tmp-b[i][j]);
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



void setEdges(float a, float b, float c, float d){
	edgeX0 = a;
	edgeX1 = b;
	edgeY0 = c;
	edgeY1 = d;
	
}

float iterate(float change_rate, stencil &stenz, int id, int nodenum){
		
		stencil b= initStencil(N);
		float change = change_rate+1.0;
		float ch;
		int n=0;
		
		int perRow= 2;
		//if (nodenum > 4)
		//	perRow= nodenum/4;
		int rows= nodenum/perRow;
		if(nodenum < 4){
			perRow= nodenum;
			rows=1;
		}
		int x=-1;
		int y=0;
		for(int i,j=0; i<= id; i++, j++){
				x++;
				if(j == rows){
					x=0;
					y++;
					j=0;
				//	printf("id %d x %d\n", id, x);
				}
			/**	if(k== perRow){
					k=0;
					y++;
					printf("id %d y %d\n", id,y);	
				}*/
		}
		
		while(change > change_rate){
			n++;
			//printf("%f\n", nodenum%(id+1));
			ch= computeStencil((x)*(N/rows), ((x)+1)*(N/rows),
			y*(N/perRow), (y+1)*(N/perRow), stenz, b, nodenum, id, perRow);//> (change_rate))
			MPI_Allreduce( &ch, &change, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD );
			//printf("change %f\n", change);
		}
		//	n++;
		//finished[id]=true;	
		printf("%d Iterations by %d\n ", n, id);
	return change;	
}

int main(int argc, char* argv[]){
	
    stencil stenz, result;
    double begin=0.0;
	double end= 0.0;
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
   
   
	stenz = initStencil(n);	
	
    int recieved[nodenum];
    long recvniter[nodenum];
 

	
	float change;	
	MPI_Barrier(MPI_COMM_WORLD);
	begin = MPI_Wtime();	
	change= iterate(1.0, stenz, id, nodenum);
	

	//MPI_Barrier(MPI_COMM_WORLD);	
	
		result= initStencil(n);
	
	//MPI_Barrier(MPI_COMM_WORLD);

		
		for(int i=0; i< N; i++){
			
			MPI_Allreduce( stenz[i].data(), result[i].data(), N, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
			
						
					
		}
		//MPI_Barrier(MPI_COMM_WORLD);	
		end = MPI_Wtime();	
		if(id==0){	
			//denormalize(result);	
		//	printStencil(result, id);	
			printf("%f %f %f\n", result[10][10], result[387][215], result[502][7]);		
			printf("Time=%fs;   \n",  end-begin);	
		
		}

	
    MPI_Finalize();    
	return EXIT_SUCCESS;
}
