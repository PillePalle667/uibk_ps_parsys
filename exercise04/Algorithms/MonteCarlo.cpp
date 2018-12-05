#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstdlib>
#include <time.h>

using Matrix = std::vector<std::vector<double>>;

double estimatePI(double n){
	srand(time(NULL));
	double p=0;
	unsigned i;
	double x ,y,z;
	#pragma omp parallel \
	private(i,x,y,z) shared(p)
	#pragma omp for
	for( i = 0; i < (unsigned) n; i++){

		x = (double)rand()/(double)(RAND_MAX);
		y = (double)rand()/(double)(RAND_MAX);
		z = (x*x)+(y*y);
		if(z <= 1){
			p++;
		}
	}

	return (4*p)/n;
	
}

int main(int argc, char** argv) {
	
	if(argc!=2) return EXIT_FAILURE;
	unsigned n = atoi(argv[1]);
	if(n==0) return EXIT_FAILURE;
	
	double result= estimatePI(n);
	printf("Pi is %f\n", result);
}
