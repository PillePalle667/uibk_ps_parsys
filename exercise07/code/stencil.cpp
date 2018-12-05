#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#define VALUE float


//lower and upper boundary values for the x, y, z axis
int N;
float edgeX0 = 0.0;
float edgeX1 = 0.0;

using stencil =std::vector<VALUE>;

//stencil1 stenz1; 


stencil initStencil(int n){
	N = n;
	//der ewige Stenz(il)
	stencil monaco;
	monaco.resize(n);
	for(int i=0; i<n; i++)
		monaco[i]=0;
	return monaco;
}

float transition(float a, float b){
	
	return 0.5*(a+b);
		
}

float computeStencil(int start, int stop, stencil &stenz, stencil &b){
	float change=0.0;
	float tmp;
	float c=0.0;
	for(int i= start; i<stop; i++){
		c++;
		tmp= stenz[i];
		int low=  i-1;
		int high= i+1;
		if(low<0){
			if(high>=N)
				b[i]= transition(edgeX0,edgeX1);
			else
				b[i]=transition(edgeX0, stenz[high]);
		}
		else if(high>=N)
			 b[i]=transition(stenz[low], edgeX1);
		else
			b[i]=transition(stenz[low], stenz[high]);	
		change+= b[i]-tmp;
				
	}
	for(int i=0; i<N; i++)
		stenz[i]=b[i];
	return change;	
}
void setEdges(float a, float b){
	edgeX0 = a;
	edgeX1 = b;

}
void printStencil(stencil &stenz){
	printf("Stencil: \n");
	for(int i=0; i< N; i++)
		printf("%f\t", stenz[i]);
	
	printf("\n");
}
void iterate(float change_rate, stencil &stenz){
		stencil b= initStencil(N);
		while(computeStencil(0, N, stenz) > change_rate){
			printStencil(stenz);
		}
		printStencil(stenz);
}

TEST_CASE("WEIRD STUFF", ""){
	int n=15;
	stencil stenz = initStencil(n);
	setEdges(0.0, 1.0);
	iterate(0.000001, stenz);
	for(int i=0; i<n; i++){
		printf("There %f\n",stenz[i]);
		REQUIRE((stenz[i]> 0.0000) == true);
		REQUIRE((stenz[i]< 1.0000) == true);
		if((i-1)>0)
			REQUIRE((stenz[i]> stenz[i-1]) == true);
		if((i+1)<n)
			REQUIRE((stenz[i]<stenz[i+1]) == true);	
	}
}



