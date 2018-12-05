
#include "Queens.cpp"

using Array = std::vector<int>;
using Matrix = std::vector<std::vector<int>>;

void testSolve(){
	Array solutions = {1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596, 2279184};
	
	
	bool correct= true;
	for(unsigned i=1; i< solutions.size(); i++){
		Matrix a= initBoard(i);
		unsigned col=0;
		if(solutions[i-1] != solve(a,col,0)){
			correct= false;
			printf("TEST SOLVE %d:\t\t[FAILED]\nEXPECTED: %d\n GOT: %d\n", i, solutions[i-1], solve(a,0,0));
			
			printf("\n");	
		}else{
			printf("TEST SOLVE %d:\t\t[PASSED]\n", i);
		}	
	}
	if(correct)
		printf("TEST SOLVE:\t\t[PASSED]\n");	
	
}

void testIsSafe(){
	Matrix a = initBoard(8);
	a[0][0]= 1;
	bool correctR= true;
	bool correctC= true;
	bool correctD= true;
	for(int i=0; i<8; i++){
		if(isSafe(a,0,i)){
			printf("TEST ISSAFE ROW:\t [FAILED]\n");
			correctR=false;
		}
		

		if(isSafe(a,i,0)){
			printf("TEST ISSAFE COL:\t [FAILED]\n");
			correctC=false;
		}
			
		if(isSafe(a, i, i)){
			printf("TEST ISSAFE DIAGONAL: [FAILED]\n");
			correctD=false;
		}
	}
	if(correctC)
		printf("TEST ISSAFE COL:\t [PASSED]\n");
	if(correctR)
		printf("TEST ISSAFE ROW:\t [PASSED]\n");
	if(correctD)
		printf("TEST ISSAFE DIAGONAL:\t [PASSED]\n");
	if(!isSafe(a, 1, 2)){
			printf("TEST ISSAFE POSITIVE: [FAILED]\n");
			
		}
	else
		printf("TEST ISSAFE POSITIVE:\t [PASSED]\n");
}

int main(int argc, char** argv) {
	if(argc!=2) return EXIT_FAILURE;
	char n = *argv[1];

	
	switch(n){
		case 's': testSolve(); break;
		case 'i': testIsSafe(); break;
		default: return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
	
}
	
