#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstdlib>

#define VALUE int

using Matrix = std::vector<std::vector<VALUE>>;

// initializes a square matrix of size n x n
Matrix initBoard(unsigned n) {
	Matrix res;
	res.resize(n);
	for(unsigned i=0; i<n; i++) {
		res[i].resize(n);
		for(unsigned j=0; j<n; j++) {
			res[i][j] =  0;
		}
	}
	return res;
}

//check if field is safe
bool isSafe(const Matrix&a, unsigned row, unsigned col){
	 int i,j;
	 for (i = 0; i < col; i++){ 
        if (a[row][i]!=0) 
            return false; 
		}
	
    /* Check upper diagonal on left side */
    for (i=row, j=col; i>=0 && j>=0; i--, j--){ 
        if (a[i][j]!=0) 
            return false; 
  }
	
    /* Check lower diagonal on left side */
    for (i=row, j=col; j>=0 && i<a.size(); i++, j--){ 
        if (a[i][j]!=0) 
            return false; 
  }
    return true; 
} 

// compute number of solutions
int solve( Matrix& a, unsigned col, int solutions) {
	unsigned n = a.size();

	if(col == a.size()){
		#pragma omp atomic
		++solutions;
		return solutions;
	}
	//bool result= false;
	#pragma omp parallel for
	for (unsigned i = 0; i < n; i++)
    {
        if (isSafe(a, i, col))
        {

            a[i][col] = 1;
            solutions = solve(a,col+1, solutions);
            a[i][col] = 0;
        }

    }
    return solutions;
}
	
/**/
int main(int argc, char** argv) {
	if(argc!=2) return EXIT_FAILURE;
	unsigned n = atoi(argv[1]);
	if(n==0) return EXIT_FAILURE;

	Matrix a = initBoard(n);
	unsigned col= 0;
	int result = solve(a,col,0);
	printf("%d Solutions\n", result);
	return EXIT_SUCCESS;
	
}
