#define CATCH_CONFIG_MAIN
#include <omp.h>
#include <vector>
#include <ctime>
#include <time.h>
#include <iostream>
//#include "catch.hpp"
#define VALUE double
//lower and upper boundary values for the x, y, z axis
int N;
double edgeX0 = 0.0;
double edgeX1 = 0.0;
double edgeY0 = 0.0;
double edgeY1 = 0.0;

using stencil =std::vector<std::vector<VALUE>>;

stencil initStencil(int n){
	N = n;
	stencil stenz;
	stenz.resize(n);
	for(int i=0; i<n; i++){
		stenz[i].resize(n);
		for(int j=0; j<n; j++)
			stenz[i][j]=0;
	}
	return stenz;
}
void printStencil(stencil &stenz){
	for(int i=0; i< N; i++){
		for(int j=0; j<N; j++){
			printf("%f\t",stenz[i][j]);
		}
		printf("\n");
	}
			
	
}
double transition(double a, double b, double c, double d){
	
	return 0.25* (a+b+c+d);
		
}

double computeStencil(int startx, int stopx, int starty, int stopy, stencil &stenz, stencil &b){
	double change=0.0;
	double tmp;
	int c= 0,i,j;
	//omp_set_dynamic(0);     // Explicitly disable dynamic teams
	//omp_set_num_threads(4);
	#pragma omp parallel for collapse (2) private(i,j) 
	for(i= startx; i< stopx; i++){
	for(j = starty; j< stopy; j++){
			c++;
			tmp= stenz[i][j];
			double lowx, highx, lowy, highy;
			(i-1 < 0) ? lowx = edgeX0 : lowx= stenz[i-1][j];
			(i+1 >= N) ? highx= edgeX1 : highx=stenz[i+1][j];
			(j-1 < 0) ? lowy= edgeY0 : lowy=stenz[i][j-1];
			(j+1 >= N) ? highy=edgeY1 : highy=stenz[i][j+1];
			b[i][j] = transition(lowx, highx, lowy, highy);
			change+= b[i][j]-tmp;
		}
	}
	for(int i= startx; i< stopx; i++){
		for(int j = starty; j< stopy; j++){
			stenz[i][j]= b[i][j];	
		}
	}	
	return change;	
}

void setEdges(double a, double b, double c, double d){
	edgeX0 = a;
	edgeX1 = b;
	edgeY0 = c;
	edgeY1 = d;
	
}

void iterate(double change_rate, stencil &stenz){
		stencil b= initStencil(N);
		while(computeStencil(0, N, 0, N, stenz, b) > change_rate);
			
}

int main(){

	omp_set_num_threads(1);
	int n=512;
	
	stencil stenz = initStencil(n);

	setEdges(1.0, 0.0, -0.5, 0.5);
	std::clock_t c_start = std::clock();

	iterate(10.0, stenz);
	//printStencil(stenz);
 	std::clock_t c_end = std::clock();
	long time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
        std::cout << time_elapsed_ms;
	return 0;
}
/*
TEST_CASE("WEIRD STUFF", ""){
	int n=10;
	stencil stenz = initStencil(n);
	setEdges(0.0, 0.0, 1.0, 1.0);
	iterate(0.000000001, stenz);
	printStencil(stenz);
	for(int i=0; i<n; i++)
		for(int j=0; j< n; j++)
			REQUIRE((stenz[i][j]> 0.0 && stenz[i][j]< 1.0) == true);
}
*/
