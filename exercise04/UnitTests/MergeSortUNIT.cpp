
#include "MergeSort.cpp"

using Array = std::vector<int>;
using Matrix = std::vector<std::vector<int>>;

void testMerge(){
	Array a = {0, 5, 6, 7, 9, 10, 12, 42, 55, 101, 10000};
	Array b = {7, 27, 44, 55, 77, 88};
	Array sol = {0,5,6,7,7,9,10, 12, 27,42, 44, 55, 55, 77, 88, 101, 10000};
	Array c= merge(a, b);
	bool correct= true;
	for(unsigned i=0; i< c.size(); i++){
		if(c[i] != sol[i]){
			correct= false;
			printf("TEST MERGE:\t\t\t[FAILED]\nEXPECTED: 0,5,6,7,7,9,10, 12, 27,42, 44, 55, 55, 77, 88, 101, 10000\n GOT: ");
			for(unsigned i = 0; i< c.size(); i++)
				printf("%d, ", c[i]);
			printf("\n");	
		}
	}
	if(correct)
		printf("TEST MERGE:\t\t\t[PASSED]\n");	
	
}
void testIsSorted(){
	Array a = {1,2,3,4,78,101,1000, 1001};
	Array b = {1,2,6,8,9,100,4,6,7,8};
	if(checkSorted(a))
			printf("TEST CHECKSORTED_POSITIVE:\t[PASSED]\n");
	else
			printf("TEST CHECKSORTED_POSITIVE:\t[FAILED]\n");
	if(!checkSorted(b))
			printf("TEST CHECKSORTED_NEGATIVE:\t[PASSED]\n");
	else
			printf("TEST CHECKSORTED_NEGATIVE:\t[FAILED]\n");
		
	
}
void testSort(){
	Array a = {512, 77, 99, 12, 4, 7, 88, 128, 1024, 42};
	Array b = {4, 7, 12, 42, 77, 88,99 ,128, 512, 1024};
	a = mergeSort(a);
	bool correct=true;
	for(unsigned i=0; i< a.size(); i++){
		if(a[i]!= b[i]){
			printf("TEST MERGESORT:\t\t\t[FAILED]\nEXPECTED: 4, 7, 12, 42, 77, 88,99 ,128, 512, 1024\n GOT: ");
			for(unsigned j= 0; j< a.size(); j++)
				printf("%d, ", a[j]);
			printf("\n");	
			correct=false;	
			break;	
		}
	}
	if(correct)
		printf("TEST MERGESORT:\t\t\t[PASSED]\n");
	
	
}
void testSplit(){
	Array a= {1,2,3,4,5,6,7,8,9};
	Array b= {1,2,3,5,4,3,2,8};
	Matrix am= split(a);
	Matrix bm= split(b);
	bool correct = true;
	if(am.size()!=2 || bm.size()!=2){
		printf("TEST SPLIT:\t\t\t[FAILED]\n");
		correct=false;
	}
	else{
		unsigned i;
		for(i = 0; i< am[0].size(); i++){
			if(a[i]!= am[0][i]){
				printf("TEST SPLIT:\t\t\t[FAILED]\n");
				correct=false;
				break;
			}
		}
		for(i = 0; i< am[1].size(); i++){
			if(a[i+am[0].size()]!= am[1][i]){
				printf("TEST SPLIT:\t\t\t[FAILED]\n");
				correct=false;
				break;
			}		
		}
	for( i = 0; i< bm[0].size(); i++){
			if(b[i]!= bm[0][i]){
				printf("TEST SPLIT:\t\t\t[FAILED]\n");
				correct=false;
				break;
			}
		}
		for(i = 0; i< bm[1].size(); i++){
			if(b[i+bm[0].size()]!= bm[1][i]){
				printf("TEST SPLIT:\t\t\t[FAILED]\n");
				correct=false;
				break;
			}
		}
	}
	if(correct)
		printf("TEST SPLIT:\t\t\t[PASSED]\n");
						
	
}



int main(int argc, char** argv) {
	if(argc!=2) return EXIT_FAILURE;
	char n = *argv[1];
	
	
	switch(n){
		case 'm': testMerge(); break;
		case 's': testSort(); break;
		case 'p': testSplit(); break;
		case 'b': testIsSorted(); break;
		default: return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
	
}
	
