#define CATCH_CONFIG_MAIN
#include <stdio.h>
#include "catch.hpp"
#define VALUE float
//lower and upper boundary values for the x, y, z axis
int N;
float edgeX0 = 0.0;
float edgeX1 = 0.0;
float edgeY0 = 0.0;
float edgeY1 = 0.0;
float edgeZ0 = 0.0;
float edgeZ1 = 0.0;

using stencil =std::vector<std::vector<std::vector<VALUE>>>;

//stencil3 stenz3;



stencil initStencil(int n){
	N = n;
	stencil stenz;
	stenz.resize(n);
	for(int i=0; i<n; i++){
		stenz[i].resize(n);
		for(int j=0; j<n; j++){
			stenz[i][j].resize(n);
			for(int k=0; k<n;k++)
				stenz[i][j][k]=0;
		}
	}
	return stenz;
}
float transition(float a, float b, float c, float d, float e, float f){
	
	return (a+b+c+d+e+f)/6;	
}

float computeStencil(int startx, int stopx, int starty, int stopy,  int startz, int stopz, stencil &stenz, stencil &b){
	float change=0.0;
	float tmp;
	int c = 0;
	for(int i= startx; i< stopx; i++){
		for(int j = starty; j< stopy; j++){
			for(int k=startz; k< stopz; k++){
				c++;

				tmp= stenz[i][j][k];
				float lowx, highx, lowy, highy, lowz, highz;
				(i-1 < 0) ? lowx = edgeX0 : lowx= stenz[i-1][j][k];
				(i+1 >= N) ? highx= edgeX1 : highx=stenz[i+1][j][k];
				(j-1 < 0) ? lowy = edgeY0 : lowy=stenz[i][j-1][k];
				(j+1 >= N) ? highy= edgeY1 : highy=stenz[i][j+1][k];
				(k-1 < 0) ? lowz = edgeZ0 : lowz= stenz[i][j][k-1];
				(k+1 >= N) ? highz= edgeZ1 : highz= stenz[i][j][k+1];
			b[i][j][k] = transition(lowx, highx, lowy, highy, lowz, highz);
			change+= b[i][j][k]-tmp;
			}
		}
	}
	return change;	
}
void setEdges(float a, float b, float c, float d, float e, float f){
	edgeX0 = a;
	edgeX1 = b;
	edgeY0 = c;
	edgeY1 = d;
	edgeZ0 = e;
	edgeZ1 = f;
	
}
void printSlice(std::vector<std::vector<float>> slice){
	for(int n=0; n<N; n++){
		for(int j=0; j<N;j++)
			printf("%f\t", slice[n][j]);
		printf("\n");	
	}
}
void printStencil(stencil &stenz){
	for(int i=0; i<N; i++){
		printf("i: %d\n", i);
		printSlice(stenz[i]);
	}
	
}
void iterate(float change_rate, stencil &stenz){
		int n=0;
		stencil b= initStencil(N);
		stencil c;
		while(computeStencil(0, N, 0, N, 0, N, stenz, b)>change_rate){
			c= stenz;
			stenz=b;
			b=c;
		}
		printStencil(stenz);
}
TEST_CASE("WEIRD STUFF", ""){
	int n=100;

	stencil stenz = initStencil(n);
	setEdges(0.0, 0.0, 1.0, 1.0, 0.0, 1.0);
	iterate(0.000000001, stenz);
	printf("Oy\n");
	for(int i=0; i<n; i++)
		for(int j=0; j< n; j++)
			for(int k = 0; k< n; k++)
			REQUIRE((stenz[i][j][k]> 0.0 && stenz[i][j][k]< 1.0) == true);
}



