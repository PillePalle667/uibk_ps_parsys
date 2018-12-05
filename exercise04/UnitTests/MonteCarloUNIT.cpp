#include "MonteCarlo.cpp"

using Array = std::vector<unsigned>;

void testMonty(){
	const double PI = 3.14159265359;
	Array testSizes={10, 100, 1000, 10000, 100000, 1000000, 100000000};
	double precisions[]={ 1, 0.5, 0.1, 0.01, 0.01, 0.001, 0.0001};
	for(unsigned i=0; i< testSizes.size(); i++){
		double result= estimatePI(testSizes[i]);
		if(PI-precisions[i]<=result && result<=PI+precisions[i]){
			printf("TEST MC PI N%d:\t\t [PASSED]\n", testSizes[i]);
			//printf("%f \n %f\n", result, PI);
		}
		else{
			printf("TEST MC PI N%d:\t\t [FAILED]\n", testSizes[i]);
			//printf("%f \n %f\n", result, PI);
}
		}		
		
		
}
		
int main(int argc, char** argv) {
	
	testMonty();
	return EXIT_SUCCESS;	
}
