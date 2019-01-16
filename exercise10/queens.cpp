/** 
 *  Taken from source code of Paper 
 * 	'A Specimen MPI Application:  N-Queens in Parallel'
 *	by Timothy J Rolfe
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <sys/times.h>  // Defines the struct tms
#include <time.h>
#include <stdio.h>
#include <iostream>

long int Ntotal  = 0;  // Accumulate results here
         

enum{FALSE,TRUE} ;// Enable/disable tracing execution

// These need to be modified outside of Nqueens
short *Diag = NULL, *AntiD = NULL;

inline void Mark (
     int R, int C, int size,
     short Diag[], short AntiD[], int Flag)
{
   int Idx;

// Diagonal:  Row-Col == constant
   Idx = R - C + size-1;
   Diag[Idx] = Flag;
// AntiDiagonal:  Row+Col == constant
   Idx = R + C;
   AntiD[Idx] = Flag;
}

#define  INIT   1  // Message to client:  size and [0]
#define  DATA   2  // Message from client with results
#define  EXIT   4  // Message from client with CPU time
                   // Also to client, giving permission to exit

void StartQueens (int size)
{  int   col, k,
         commBuffer[2], // Communication buffer -- size, [0]
         Count[2],      // Counts back from the clients
         limit = (size+1) / 2,  // Mirror images done automatically
         nProc,         // size of the communicator
         proc,          // For loop [1..nProc-1] within initial message
         nActive;       // Number of active processes
   MPI_Status Status;

   MPI_Comm_size (MPI_COMM_WORLD, &nProc);
   commBuffer[0] = size;
// Send initial configurations to all client processes
   for ( col = 0, proc = 1; proc < nProc && col < limit; proc++, col++ )
   {  commBuffer[1] = col;
      MPI_Send (commBuffer, 2, MPI_INT, proc, INIT, MPI_COMM_WORLD);
   }
   nActive = proc-1;    // Since rank==0 is not used
   if (proc < nProc)    // More processes than jobs
   {  int dmy[2] = {0, 0};   // Termination message to unused processes
      while (proc < nProc)
         MPI_Send (dmy, 2, MPI_INT, proc++, INIT, MPI_COMM_WORLD);
   }
//Master waits on results
// Matser receives results and send out new problems
   while ( col < limit )
   {  MPI_Recv(Count, 2, MPI_INT, MPI_ANY_SOURCE , DATA,
               MPI_COMM_WORLD, &Status);
      proc = Status.MPI_SOURCE;
      Ntotal  += Count[1];
      commBuffer[1] = col++;
      MPI_Send (commBuffer, 2, MPI_INT, proc, INIT, MPI_COMM_WORLD);
   }
// Finally, receive back pending results and send termination
// indication (message with size of zero).
   commBuffer[0] = 0;
   while (nActive > 0)
   {
      MPI_Recv(Count, 2, MPI_INT, MPI_ANY_SOURCE , DATA,
               MPI_COMM_WORLD, &Status);
      --nActive;
      proc = Status.MPI_SOURCE;
      Ntotal  += Count[1];
      MPI_Send (commBuffer, 2, MPI_INT, proc, INIT, MPI_COMM_WORLD);
   }
   
   for (proc = 1; proc < nProc; proc++)
   {  MPI_Send(&proc, 0, MPI_INT, proc, EXIT, MPI_COMM_WORLD);

   }
}

#define  swap(x,y,z) { x temp = y; y = z; z = temp;  }

// Prototype for forward referencing
void Nqueens (int*, int*, int, int);

// Client processes receive problems to process from the
// server and then return their results to the server.
void ProcessQueens(int myPos)
{
   int  nCells = 0, size, k, col,
        buffer[2];
   int *board = NULL, *trial = NULL;   // Allow for realloc use

   MPI_Status Status;

//Receives problem from master
   MPI_Recv(buffer, 2, MPI_INT, 0, INIT, MPI_COMM_WORLD, &Status);
   fflush(stdout);
   size = buffer[0];
   col  = buffer[1];
// As long as a valid problem is in hand, do the processing.
// The server sends a size of zero as a termination message
   while (size > 0)
   {  int Count[2];

   //    Generate the arrays
      if ( size > nCells )
      {  int idx;

         board = (int*) calloc (size, sizeof *board);
         trial = (int*) calloc (size*2, sizeof *trial);
        // Allocate the boolean arrays Diag and AntiD
    //    Note that calloc automatically fills with FALSE (0)
         Diag  = (short*) calloc (2*(size-1), sizeof *Diag);
         AntiD = (short*) calloc (2*(size-1), sizeof *AntiD);
         if ( !AntiD )
         {
            puts ("realloc failed --- out of memory!");
            exit (11);
         }
   //    Initial permutation generated
      // Since trial is scratch space, it is filled by Nqueens.
         for (idx = 0; idx < size; idx++)
            board[idx] = idx;

         nCells = size;
      }
   // Zero out the counters for THIS problem start.
      Ntotal  = 0;
      swap (int, board[0], board[col]);
 //   CRITICAL:  mark [0] as used, and then as unused
      Mark (0, board[0], size, Diag, AntiD, TRUE);
//Calling Nqueens
      Nqueens (board, trial, size, 1);
      Mark (0, board[0], size, Diag, AntiD, FALSE);
      swap (int, board[0], board[col]);     // Undo the swap
//Put the data into the communication vector
  	 Count[1] = Ntotal;
//Client send results
      MPI_Send(Count,  2, MPI_INT, 0, DATA, MPI_COMM_WORLD);
//Get the next job -- or the termination message.
      MPI_Recv(buffer, 2, MPI_INT, 0, INIT, MPI_COMM_WORLD, &Status);
      size = buffer[0];
      col  = buffer[1];
   }
   MPI_Recv(buffer, 0, MPI_INT, 0, EXIT, MPI_COMM_WORLD, &Status);
}

/* Check two vectors for equality; return first inequality (a la strncmp) */
int intncmp (int L[], int R[], int N)
{
   int Idx;

   for (Idx = 0; Idx < N; Idx++)
      if ( L[Idx] - R[Idx] )
         return L[Idx]-R[Idx];
   return 0;
}

/* Rotate +90 or -90: */
void Rotate(int R[], int C[], int N, int Neg)
{
   int Idx, Jdx;

   Jdx = Neg ? 0 : N-1;
   for (Idx = 0; Idx < N; Neg ? Jdx++ : Jdx--)
      C[Idx++] = R[Jdx];
   Jdx = Neg ? N-1 : 0;
   for (Idx = 0; Idx < N; Neg ? Jdx-- : Jdx++)
      R[C[Idx++]] = Jdx;
}

/* Vertical mirror:  reflect each row across the middle */
void Vmirror(int R[], int N)
{
   int Idx;

   for (Idx = 0; Idx < N; Idx++)
      R[Idx] = (N-1) - R[Idx];
   return;
}

/* Check the symmetries.  Return 0 if this is not the 1st */
/* solution in the set of equivalent solutions; otherwise */
/* return the number of equivalent solutions.             */
int SymmetryOps(
    int Board[],     /* The fully-populated board         */
    int Trial[],     /* Used for symmetry checks          */
                     /* Holds its own scratch space too!  */
    int Size)        /* Number of cells in a row/column   */
{  int  Idx;         /* Loop variable; intncmp result     */
   int  Nequiv;      /* Number equivalent boards          */
   int *Scratch=&Trial[Size];   /* Scratch space          */

/* Copy; Trial will be subjected to the transformations   */
   for (Idx = 0; Idx < Size; Idx++)
      Trial[Idx] = Board[Idx];

/* 90 degrees --- clockwise (4th parameter of Rotate is FALSE)*/
   Rotate (Trial, Scratch, Size, 0);
   Idx = intncmp (Board, Trial, Size);
   if (Idx > 0) return 0;
   if ( Idx == 0 )  /* No change on 90 degree rotation        */
      Nequiv = 1;
   else                                       /*  180 degrees */
   {  Rotate (Trial, Scratch, Size, 0);
      Idx = intncmp (Board, Trial, Size);
      if (Idx > 0) return 0;
      if ( Idx == 0 ) /* No change on 180 degree rotation     */
         Nequiv = 2;
      else                                    /* 270 degrees  */
      {  Rotate (Trial, Scratch, Size, 0);
         Idx = intncmp (Board, Trial, Size);
         if (Idx > 0) return 0;
         Nequiv = 4;
      }
   }
/* Copy the board into Trial for mirror checks */
   for (Idx = 0; Idx < Size; Idx++)
      Trial[Idx] = Board[Idx];
/* Reflect -- vertical mirror */
   Vmirror (Trial, Size);
   Idx = intncmp (Board, Trial, Size);
   if (Idx > 0) return 0;
   if ( Nequiv > 1 )    // I.e., no four-fold rotational symmetry
   {
/* -90 degrees --- equiv. to diagonal mirror */
      Rotate (Trial, Scratch, Size, -1);
      Idx = intncmp (Board, Trial, Size);
      if (Idx > 0) return 0;
      if ( Nequiv > 2 ) // I.e., no two-fold rotational symmetry
      {
/* -180 degrees --- equiv. to horizontal mirror */
         Rotate (Trial, Scratch, Size, -1);
         Idx = intncmp (Board, Trial, Size);
         if (Idx > 0) return 0;
/* -270 degrees --- equiv. to anti-diagonal mirror */
         Rotate (Trial, Scratch, Size, -1);
         Idx = intncmp (Board, Trial, Size);
         if (Idx > 0) return 0;
      }
   }
/* WE HAVE A GOOD ONE! */
   return Nequiv * 2;
}

/* Test the validity of this particular partial board.            */
/* Massively used; inline to eliminate the function call overhead */

inline int Valid (int Board[], int Size, int Row,
                  short Diag[], short AntiD[] )
{
   int Idx;        // Index into Diag[] / AntiD[]
   int Chk;        // Occupied flag

// Diagonal:  Row-Col == constant
   Idx = Row - Board[Row] + Size-1;
   Chk = Diag[Idx];
// AntiDiagonal:  Row+Col == constant
   Idx = Row + Board[Row];
   Chk = Chk || AntiD[Idx];
   return !Chk;    // Valid if NOT any occupied
}

/* Process the partial (or complete) board for the indicated Row */
void Nqueens (int Board[], int Trial[], int Size, int Row)
{
   int Idx, Lim, Vtemp;

/* Check for a partial board. */
   if (Row < Size-1)
   {
      if (Valid (Board, Size, Row, Diag, AntiD))
      {
         Mark (Row, Board[Row], Size, Diag, AntiD, TRUE);
         Nqueens (Board, Trial, Size, Row+1);
         Mark (Row, Board[Row], Size, Diag, AntiD, FALSE);
      }
/*    Rejection of vertical mirror images means that row zero */
/*    only needs to be processed to the middle.               */
      Lim = Row ? Size : (Size+1)/2 ;
      for (Idx = Row+1; Idx < Lim; Idx++)
      {
         Vtemp = Board[Idx];
         Board[Idx] = Board[Row];
         Board[Row] = Vtemp;
         if (Valid (Board, Size, Row, Diag, AntiD))
         {
            Mark (Row, Board[Row], Size, Diag, AntiD, TRUE);
            Nqueens (Board, Trial, Size, Row+1);
            Mark (Row, Board[Row], Size, Diag, AntiD, FALSE);
         }
      }
/*    Regenerate original vector from Row to Size-1:  */
      Vtemp = Board[Row];
      for (Idx = Row+1; Idx < Size; Idx++)
         Board[Idx-1] = Board[Idx];
      Board[Idx-1] = Vtemp;
   }
/* This is a complete board.  Do the symmetry checks */
   else
   {
      if ( !Valid (Board, Size, Row, Diag, AntiD) )
         return;
      Idx = SymmetryOps (Board, Trial, Size);
      if (Idx)
      { 
         Ntotal += Idx;
      }
   }
   return;
}

int main(int argc, char *argv[])
{
   int    nProc,                    // Processes in the communicator
          proc;                     // loop variable
   int    rc;                       // Status  code from MPI_Xxx() call
   int    myPos,size = 15;                    // My own position
   double begin=0.0, end= 0.0;
   rc = MPI_Init(&argc, &argv);
   if (rc != MPI_SUCCESS)
   {  puts ("MPI_Init failed"); exit(-1);  }

   rc = MPI_Comm_rank (MPI_COMM_WORLD, &myPos);
   rc = MPI_Comm_size (MPI_COMM_WORLD, &nProc);

   MPI_Barrier(MPI_COMM_WORLD);
   begin = MPI_Wtime();

   if ( myPos == 0 )    // I.e., this is the server/master/host
   {
       StartQueens (size);
       end = MPI_Wtime(); 
       printf (" ==>%10ld  \n", Ntotal);

	printf("Time=%fs;   \n\n", end-begin); 
  
   }
   else  // I.e., this is the client/slave/node
      ProcessQueens(myPos);

   MPI_Finalize();

   exit(0);
}
