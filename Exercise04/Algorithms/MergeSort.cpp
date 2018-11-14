
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstdlib>
#include <time.h>

using Array = std::vector<int>;
using Matrix = std::vector<std::vector<int>>;
Array fillArray(int size){
	Array arr;
	arr.resize(size);
	srand(time(NULL));
	for(int i= 0; i<size; i++)
		arr[i]= rand()%100;
	return arr;
}



Matrix split(Array&a ){
	unsigned p = a.size()/2;
	unsigned rest= a.size()-p;
	Matrix result;
	result.resize(2);
	result[0].resize(p);
	result[1].resize(rest);
	for(unsigned i =0; i<a.size(); i++){
		if(i<p)
			result[0][i]= a[i];
		else
			result[1][i-p]= a[i];
	}
	return result;	
	
}
Array merge(Array &l, Array &r){
	Array result;
	result.resize(l.size()+r.size());
	unsigned i,j,x;
	i=j=x=0;
//	#pragma omp parallel for
	while(i< l.size()&& j< r.size()){
		if(l[i]<= r[j])
			result[x++]= l[i++];
		else if(l[i]>r[j])
			result[x++]= r[j++];
	}
	while(x < l.size()+r.size()){
		while(i< l.size())
			result[x++]= l[i++];
		while(j< r.size())
			result[x++]= r[j++];	
	}
	return result;
}


bool checkSorted(Array &a){
	for(unsigned i= 0; i< a.size()-1; i++){
		if(a[i]>a[i+1])
			return false;
	}
	return true;
	
}
Array mergeSort(Array &a){
	if(a.size()<= 1)
		return a;
	
	Matrix splitlist= split(a);
	Array lists[2];			
	/**
	for(int i=0; i<2; i++)
		lists[i]= mergeSort(splitlist[i]);
	Array res= merge(lists[0], lists[1]);
	*/
	Array l, r;
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			l= mergeSort(splitlist[0]);
		}
		#pragma omp section
		{
			r= mergeSort(splitlist[1]);
		}
	}
	Array res= merge(l, r);
	
	return res;
	
	
	
	
	
}
void printResult(Array&a){
	for(unsigned i= 0; i< a.size(); i++)
		printf("%d\t", a[i]);
	printf("\n");	
}

int main(int argc, char** argv) {
	if(argc!=2) return EXIT_FAILURE;
	unsigned n = atoi(argv[1]);
	if(n==0) return EXIT_FAILURE;
	Array a= fillArray(n);
	//bool pre= checkSorted(a);
	//printResult(a);
	a = mergeSort(a);
	bool post= checkSorted(a);
	//printResult(a);
	if(post)
		return EXIT_SUCCESS;
	return EXIT_FAILURE;	

	
}

	
