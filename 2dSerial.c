#include "omp.h"
#include <stdio.h>
#include <stdlib.h>
int N = 10; // default
void initialise_varaiables();
void print_2D_matrix(int matrix[N][N], int N);
void initialise_matrices(int A[N][N], int B[N][N], int C[N][N], int N);
double time_for_round(double start, double stop);
void multiply_2D(int A[N][N], int B[N][N], int C[N][N], int N);

int main(int argc, char *argv[])
{
      // run the code with all desired combinations of threads and matrix sizes
      for (int dimension = 4; dimension <= 6; dimension = dimension + 1)
      {


                  double total_time = 0;
                  N = dimension; // overwrite
                  int tid, i, j, k;
                  int A[N][N];
                  int B[N][N];
                  int C[N][N];
                  double start = 0;
                  double stop = 0;
                  int samples = 1;

                  // repeat 1000 times so can take an average
                  for (int loops = 0; loops < samples; loops++)
                  {

                        initialise_matrices(A, B, C, N);
                        start = omp_get_wtime(); // take start time
                        multiply_2D(A, B, C, N);
                        stop = omp_get_wtime(); // take stop time
                        if (loops == 0)
                        {
                              printf("A\n");
                              print_2D_matrix(A, N);
                              printf("B\n");
                              print_2D_matrix(B, N);
                              printf("C\n");
                              print_2D_matrix(C, N);
                        }

                        total_time = total_time + time_for_round(start, stop);
                  }
                  // double mean =calculate_mean(total_time, samples);
                  // calculate_standard_deviation(data,mean,samples)
                  printf(" %d by %d openMp multiplication algorithm took %f milliseconds to execute on average when using serial code \n", N, N, total_time / samples);
            
            printf("\n");
      }
      return 0;
}

double time_for_round(double start, double stop)
{
      double time_taken = (stop - start) * 1000.0;
      return time_taken; // returns time in milliseconds
}

void print_2D_matrix(int matrix[N][N], int N)
{

      for (int i = 0; i < N; i++)
      {
            for (int j = 0; j < N; j++)
            {

                  {
                        printf("%d ", matrix[i][j]);
                  }
                  if (j == N - 1)
                        printf("\n");
            }
      }
}

void initialise_matrices(int A[N][N], int B[N][N], int C[N][N], int N)
{
      for (int i = 0; i < N; i++)
            for (int k = 0; k < N; k++)
            {
                  A[i][k] = rand() % 20;
                  B[i][k] = rand() % 20;
                  C[i][k] = 0;
            }
}

void multiply_2D(int A[N][N], int B[N][N], int C[N][N], int N)
{
      int tid, i, j, k;

// combines the total number of iterations in the two loops below and devide them by the total number of threads allocated by the enviroment

            for (i = 0; i < N; i++) // for all the rows
            {
                  for (j = 0; j < N; j++) // for all the columns

                  {
                        for (k = 0; k < N; k++) // dot product of  row i from A with column j of B to caculate element ij of C
                        {
                              C[i][j] += A[i][k] * B[k][j];
                        }
                  }
            }
}
