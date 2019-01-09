
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
float computeStencil(int startx, int stopx, int starty, int stopy, stencil &stenz, stencil &b, int nodenum, int id, int perRow){
	//  printf("startx: %d stopx: %d starty: %d stopy: %d id: %d\n", startx, stopx, starty, stopy, id);
	  if(id== nodenum-1){
			stopx= N;
			stopy=N;
		}
	  int idOffset;
	  nodenum <= 3 ? idOffset= 1: idOffset= nodenum/perRow;				
			//printf("Och nö\n");
	 float change=0.0;
	 float tmp;
	
	 float rlx[N];
	 float rhx[N];
	 float rhy[N];
	 float rly[N];
	 for(int i=0; i< N; i++){
		 rlx[i]=0.0;
		 rhx[i]=0.0;
		 rhy[i]=0.0;
		 rly[i]=0.0;
	 }
	 MPI_Status status_slx, status_shx, status_rlx, status_rhx, status_sly, status_shy, status_rly, status_rhy;
	 MPI_Request reqslx, reqshx, reqrlx, reqrhx, reqsly, reqshy, reqrly, reqrhy;  
	 
	   
      if(startx >0 ){	
			float buf[N];
			for(int i=0; i<N; i++)
				buf[i]= stenz[startx][i];
				
            MPI_Isend(&buf,
                                 N,
                                 MPI_FLOAT,
                                 id-1,
                                 0,
                                 MPI_COMM_WORLD, 
                                 &reqslx);
         
	   }
                                
        //    printf("post send pre rec by %d\n", id);
         if(startx >0 ){ 
			
			               
            MPI_Irecv(&rlx,
                                N,
                                MPI_FLOAT,
                                id-1,
                                0,
                                MPI_COMM_WORLD,
                                &reqrlx);
          
        }                        
								 
		if(stopx <N){	
			float buf[N];
			for(int i=0; i<N; i++)
				buf[i]= stenz[stopx-1][i];
			
            MPI_Isend(&buf,
                                 N,
                                 MPI_FLOAT,
                                 id+1,
                                 0,
                                 MPI_COMM_WORLD,
                                 &reqshx);
                                 
							 
						}
        
        if(stopx <N){ 
			
			MPI_Irecv(&rhx,
                                N,
                                MPI_FLOAT,
                                id+1,
                                0,
                                MPI_COMM_WORLD,
                                &reqrhx);
            				
		}
	
       if(starty >0 ){	
			
			float buf[N];
			for(int i=0; i<N; i++)
				buf[i]= stenz[i][starty];
            MPI_Isend(&buf,
                                 N,
                                 MPI_FLOAT,
                                 id-idOffset,
                                 0,
                                 MPI_COMM_WORLD,
                                 &reqsly);
         
	   }
                                
      
         if(starty >0 ){ 
			
			            
            MPI_Irecv(&rly,
                                N,
                                MPI_FLOAT,
                                id-idOffset,
                                0,
                                MPI_COMM_WORLD,
                                &reqrly);
                                        
			
        }                        
		 							 
		if(stopy <N){	
			float buf[N];
			for(int i=0; i<N; i++){
				buf[i]= stenz[i][stopy-1];
		   // printf("%f ", buf[i]);
		}
		//printf("\n");
			
            MPI_Isend(&buf,
                                 N,
                                 MPI_FLOAT,
                                 id+idOffset,
                                 0,
                                 MPI_COMM_WORLD,
                                 &reqshy);
							 
						 }
      
        if(stopy <N){ 
			
			
			MPI_Irecv(&rhy,
                                N,
                                MPI_FLOAT,
                                id+idOffset,
                                0,
                                MPI_COMM_WORLD,
                                &reqrhy);
         			
		}
	
		
	
	
	int c= 0;
	
	
	for(int i= startx+1; i< stopx-1; i++){
		for(int j = starty+1; j< stopy-1; j++){
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
	
			b[i][j] = transition(lowx, highx, lowy, highy);
	
			float ch;
			ch= abs(b[i][j]-tmp);
			
			change+= ch;
		}
		}
		
	
	if(startx > 0){
	
		MPI_Wait(&reqrlx, &status_rlx);
		
		for(int i =0; i< N; i++)
			b[startx-1][i]= rlx[i];
	}
	if(starty > 0){
		
	    MPI_Wait(&reqrly, &status_rly);
	  
	    for(int i =0; i< N; i++)
			b[i][starty-1]= rly[i];
	    
	}
	if(stopx < N){
	
		MPI_Wait(&reqrhx, &status_rhx);
	
		 for(int i =0; i< N; i++)
			b[stopx][i]= rhx[i];
	}
	if(stopy < N){
	   
	    MPI_Wait(&reqrhy, &status_rhy);	
	   
	     for(int i =0; i< N; i++)
			b[i][stopy]= rhy[i];
	}
	
	for(int i=0; i<N; i++){
		printf("%f rly ", rly[i]);
	}
	printf("\n");
	for(int i=0; i<N; i++){
		printf("%f rhy ", rhy[i] );
	}
	printf("\n");
	for(int i=0; i<N; i++){
		printf("%f rlx ", rlx[i] );
	}
	printf("\n");
	for(int i=0; i<N; i++){
		printf("%f rhx ", rhx[i]);
	}
	printf("\n");
		
		
	
  
	float lowx1, lowx2, highx1, highx2, lowy1, lowy2, highy1, highy2;
	float tmp1, tmp2, ch;
	for(int i=startx; i<stopx; i++){
			tmp1= stenz[i][starty];
			tmp2= stenz[i][stopy-1];
			if(i== startx){
				(i<=0) ? lowx1= edgeX0 : lowx1= b[i-1][starty];
				(i<=0) ? lowx2= edgeX0 : lowx2= b[i-1][stopy-1];
				highx1= stenz[i+1][starty];
				highx2= stenz[i+1][stopy-1];
			}else{
				lowx1= stenz[i-1][starty];
				lowx2= stenz[i-1][stopy-1];
				if(i >= stopx-1){
					(i>= N-1)? highx1= edgeX1: highx1= b[i+1][starty];
					(i>= N-1)? highx2= edgeX1: highx2= b[i+1][stopy-1];
					
				}
				else{
					highx1= stenz[i+1][starty];
					highx2= stenz[i+1][stopy-1];
				}
			}
			starty <= 0 ? lowy1= edgeY0: b[i][starty-1];
			starty <= 0 ? lowy2= edgeY0: b[i][starty-1];
			stopy >= N-1 ? highy1= edgeY1: b[i][stopy];
			stopy >= N-1 ? highy2= edgeY1: b[i][stopy];
			b[i][starty]= transition(lowx1, highx1,lowy1, highy1);
			b[i][stopy-1]= transition(lowx2, highx2,lowy2, highy2);
		
			(b[i][starty]>tmp1)? ch= b[i][starty]-tmp1 : ch=tmp1-b[i][starty];
			change+= ch;
			(b[i][stopy-1]>tmp2)? ch= b[i][stopy-1]-tmp2 : ch=tmp2-b[i][stopy-1];
			change+= ch;
			printf("change in function %f\n", change);
	}
	printf("lowx1 %f lowx2 %f highx1 %f highx2 %f lowy1 %f lowyl2 %f 1highy1 %f highy2 %f \n",lowx1, lowx2, highx1, highx2, lowy1, lowy2, highy1, highy2);
	
	for(int i=starty+1; i<stopy-1; i++){
		    tmp1= stenz[startx][i];
			tmp2= stenz[stopx-1][i];
			if(i== starty){
				(i<=0) ? lowy1= edgeY0 : lowy1= b[startx][i-1];
				(i<=0) ? lowy2= edgeY0 : lowy2= b[stopx-1][i-1];
				highy1= stenz[startx][i+1];
				highy1= stenz[stopx-1][i+1];
			}else{
				lowy1= stenz[startx][i-1];
				lowy2= stenz[stopx-1][i-1];
				if(i >= stopy-1){
					(i<= N-1)? highy1= edgeY1: highy1= b[startx][i+1];
					(i<= N-1)? highy2= edgeY1: highy2= b[stopx-1][i+1];
					
				}
				else{
					highy1= stenz[startx][i+1];
					highy2= stenz[stopx-1][i+1];
				}
			}
			startx <= 0 ? lowx1= edgeX0: b[startx-1][i];
			startx <= 0 ? lowx2= edgeX0: b[startx-1][i];
			stopx >= N-1 ? highx1= edgeX1: b[stopx][i];
			stopx >= N-1 ? highx2= edgeX1: b[stopx][i];
			b[startx][i]= transition(lowx1, highx1,lowy1, highy1);
			b[stopx-1][i]= transition(lowx2, highx2,lowy2, highy2);
			(b[startx][i]>tmp1)? ch= b[startx][i]-tmp1 : ch=tmp1-b[startx][i];
			change+= ch;
			(b[stopx-1][i]>tmp2)? ch= b[stopx-1][i]-tmp2 : ch=tmp2-b[stopx-1][i];
			change+= ch;
			
	}
			
		
	
	printf("2lowx1 %f lowx2 %f highx1 %f highx2 %f lowy1 %f lowy2 %f 2highy1 %f highy2 %f \n",lowx1, lowx2, highx1, highx2, lowy1, lowy2, highy1, highy2);	
	 
	
	for(int i= startx; i< stopx; i++){
		for(int j = starty; j< stopy; j++){
			stenz[i][j]= b[i][j];	
		}
	}	
	//if(id==0)
		//printStencil(stenz, id);
	//printf("%d", stenz[513][600]);
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
		int perRow=2;
		int rows= nodenum/perRow;
		if(nodenum < 4){
			perRow= nodenum;
			rows=1;
		}
		int x=-1;
		int y=0;
		for(int i,j,k=0; i<= id; i++, j++, k++){
				x++;
				if(j == rows){
					
					x=0;
					y++;
					j=0;
					printf("id %d x %d\n", id, x);
				}
			/**	if(k== perRow){
					k=0;
					y++;
					printf("id %d y %d\n", id,y);	
				}*/
		}
		//x-=1;
	
		printf("rows %d\n", rows);
		printf("id: %d startx: %d starty %d stopx: %d stopy %d\n", id, x,y, x+1, y+1);
		while(change > change_rate){
			n++;
			//printf("%f\n", nodenum%(id+1));
			ch= computeStencil((x)*(N/rows), ((x)+1)*(N/rows),
			y*(N/perRow), (y+1)*(N/perRow), stenz, b, nodenum, id, perRow);//> (change_rate))
			MPI_Allreduce( &ch, &change, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD );
		printf("change %f\n", ch );
			
		}
		//	n++;
		//finished[id]=true;	
		printf("%d Iterations by %d\n ", n, id);
	return change;	
}

int main(int argc, char* argv[]){
	
    stencil stenz, result;
    
//	omp_set_num_threads(1);
	int n=10;
	
	setEdges(1.0, 0.0, -0.5, 0.5);
	float begin=0.0;
	float end= 0.0;
	
	//std::clock_t c_start = std::clock();
	int id;
	int nodenum;
	   
	normalize(stenz);
    MPI_Init(&argc, &argv);                 //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &id);           //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &nodenum);
   
   
		stenz = initStencil(n);	
	
    int recieved[nodenum];
    long recvniter[nodenum];
 //   MPI_Barrier(MPI_COMM_WORLD); 

	
	float change;		
	change= iterate(1.0, stenz, id, nodenum);
	//if(normalizer != 0.0)
	//		denormalize(stenz);	

	MPI_Barrier(MPI_COMM_WORLD);	
	
		result= initStencil(n);
	//if(id==1)
		//printStencil(stenz, id);	
	MPI_Barrier(MPI_COMM_WORLD);
	//if(id==0)
		//printStencil(stenz, id);		
	printf("Hello\n");
	//MPI_Gather(&stenz, N*N, MPI_FLOAT, &result, N*N, MPI_FLOAT, 0, MPI_COMM_WORLD);	
	//result= initStencil(n);
		//for(int i=0; i< N; i++)	
			
	
		
	//	MPI_Reduce( &stenz,&result, N*N, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
		
		for(int i=0; i< N; i++){
			//printf("yikes\n");
			MPI_Allreduce( stenz[i].data(), result[i].data(), N, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
			
						
					
				}
			
		
		/**result= initStencil(n);
			
			*/
		if(id==0){	
			denormalize(result);	
			printStencil(result, id);	
		//	printf("%f %f %f\n", result[10][10], result[387][215], result[502][7]);		
			printf("Hello2\n");
		}
	
	
			
		
		
		
	//}

	
 	//std::clock_t c_end = std::clock();
 	
 	
	//long time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
    //    std::cout << time_elapsed_ms;
   // freeStencil(stenz);
  //  freeStencil(result);
    MPI_Finalize();    
	//return EXIT_SUCCESS;
}
