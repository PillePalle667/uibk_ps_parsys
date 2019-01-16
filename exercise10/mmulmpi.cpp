#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <vector>
#include <ctime>
//#define N 3072
#define FROM_MASTER 1
#define FROM_WORKER 2
#define DEBUG 1 

MPI_Status status;

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


int main(int argc, char **argv)
{

int myrank, nproc;
int rows;   
int mtype;   
int dest, src, offseta, offsetb;
double start_time, end_time;
int i, j, k, l;
int n = 3072;
    auto a = id(n);
    auto b = id(n);
    auto c = id(n);
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &nproc);
MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

rows = n/nproc;  //compute the block size
mtype = FROM_MASTER; //  =1

   if (myrank == 0) {
    /*Initialization*/
    printf("SIZE = %d, number of nodes = %d\n", n, nproc);
     a = id(n);
     b = id(n);
   
    start_time = MPI_Wtime();

    if(nproc == 1) { 
        for(i = 0; i < n; i++) {
            for(j = 0; j < n; j++) {
                for(k = 0; k < n; k++)
                    c[i][j] += a[i][k]*b[j][k];
            } //end for i
        }  //end for j
        end_time = MPI_Wtime();
       // print_matrix();//---------------------------------
        printf("Execution time on %2d nodes: %f\n", nproc, end_time-
                    start_time);
    } // end  if(nproc == 1)

    else {

          for(l = 0; l < nproc; l++){
            offsetb = rows*l;  //start from (block size * processor id)
            offseta = rows; 
            mtype = FROM_MASTER; // tag =1

              for(dest = 1; dest < nproc; dest++){
                MPI_Send(&offseta, 1, MPI_INT, dest, mtype,
                            MPI_COMM_WORLD);
                MPI_Send(&offsetb, 1, MPI_INT, dest, mtype, 
                            MPI_COMM_WORLD);
                MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
                MPI_Send(&a[offseta][0], rows*n, MPI_DOUBLE, dest,
                             mtype, MPI_COMM_WORLD);
                MPI_Send(&b[0][offsetb], rows*n, MPI_DOUBLE, dest, 
                          mtype, MPI_COMM_WORLD);

                offseta += rows;
                offsetb = (offsetb+rows)%n;

            } // end for dest

            offseta = rows;
            offsetb = rows*l;

     //--mult the final local and print final global mult
            for(i = 0; i < offseta; i++) {
                for(j = offsetb; j < offsetb+rows; j++) {
                        for(k = 0; k < n; k++){
                            c[i][j] += a[i][k]*b[k][j];
                    }//end for k
                } //end for j
            }// end for i
               /*- wait for results from all worker tasks */
            mtype = FROM_WORKER;
            for(src = 1; src < nproc; src++){
                MPI_Recv(&offseta, 1, MPI_INT, src, mtype, MPI_COMM_WORLD,
                           &status);
                MPI_Recv(&offsetb, 1, MPI_INT, src, mtype, MPI_COMM_WORLD,
                            &status);
                MPI_Recv(&rows, 1, MPI_INT, src, mtype, MPI_COMM_WORLD, 
                           &status);
                for(i = 0; i < rows; i++) {
                    MPI_Recv(&c[offseta+i][offsetb], offseta, MPI_DOUBLE,
           src, mtype, MPI_COMM_WORLD, &status);
                } //end for scr
            }//end for i
        } //end for l
       end_time = MPI_Wtime();          
        //print_matrix();
       printf("Execution time on %2d nodes: %f\n", nproc, end_time-
             start_time);
       }//end else
   } //end if (myrank == 0) 

   else{  
          /*---------------------------- worker----------------------*/
      if(nproc > 1) {
            for(l = 0; l < nproc; l++){
               mtype = FROM_MASTER;
               MPI_Recv(&offseta, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD,
                       &status);
               MPI_Recv(&offsetb, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD,
                       &status);
            MPI_Recv(&rows, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD, 
                      &status);

            MPI_Recv(&a[offseta][0], rows*n, MPI_DOUBLE, 0, mtype, 
            MPI_COMM_WORLD, &status);
            MPI_Recv(&b[0][offsetb], rows*n, MPI_DOUBLE, 0, mtype, 
            MPI_COMM_WORLD, &status);

            for(i = offseta; i < offseta+rows; i++) {
                for(j = offsetb; j < offsetb+rows; j++) {
                    for(k = 0; k < n; k++){
                        c[i][j] += a[i][k]*b[k][j];  
                    } //end for j
                } //end for i    

            } //end for l

            mtype = FROM_WORKER;
            MPI_Send(&offseta, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
            MPI_Send(&offsetb, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, 0, mtype, MPI_COMM_WORLD);
                for(i = 0; i < rows; i++){
                MPI_Send(&c[offseta+i][offsetb], offseta, MPI_DOUBLE, 0, 
                 mtype, MPI_COMM_WORLD);

               } //end for i
            }//end for l

        } //end if (nproc > 1)
    } // end else

    MPI_Finalize();
    return 0;
} //end main()
