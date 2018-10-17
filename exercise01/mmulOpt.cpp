#include <vector>
#include <cstdlib>
#include <iostream>
using Matrix = std::vector<std::vector<double>>;

// initializes a square identity matrix of size n x n
Matrix id(unsigned n) {
	Matrix res;
	res.resize(n);		
	for(unsigned i=0; i<n; i++) {
		res[i].resize(n);
		for(unsigned j=0; j<n; j++) {
			res[i][j] = (i == j) ? 1 : 0;
		}
	}
	return res;
}


void Transpose(int size, Matrix m)
{
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            std::swap(m[i][j], m[j][i]);
        }
    }
}

Matrix SeqMatrixMult3(int size, Matrix& m1, Matrix& m2, Matrix& result)
{
    Transpose(n,m2);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int c = 0;
            for (int k = 0; k < size; k++) {
                c += m1[i][k] * m2[j][k];
            }
            result[i][j] = c;
        }
    }
    Transpose(n,m2);
    return result;
}


int main(int argc, char** argv) {
	
	if(argc!=2) return EXIT_FAILURE;
	int n = atoi(argv[1]);
	if(n==0) return EXIT_FAILURE;

	// create two matrices
	
	auto a = id(n);
	a[0][0] = 42;
	auto b = id(n);

	auto c = id(n);
	// compute the product
	c = SeqMatrixMult3(n,a,b,c);
	
	// check that the result is correct
	return (c == a) ? EXIT_SUCCESS : EXIT_FAILURE;
}
