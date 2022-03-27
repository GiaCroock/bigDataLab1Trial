#include "omp.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int N = 10; // default
void print_2D_matrix(int matrix[N][N], int N);
void initialise_matrices(int A[N][N], int B[N][N], int C[N][N], int N);
double time_for_round(double start, double stop);
void multiply_2D(int A[N][N], int B[N][N], int C[N][N], int N, int nthreads);
double standard_deviation(double mean, double data[1000]);

int main(int argc, char *argv[])
{
      // run the code with all desired combinations of threads and matrix sizes
      for (int dimension = 10; dimension <= 50; dimension = dimension + 10)
      {
            for (int thread_request = 8; thread_request >= 2; thread_request = thread_request / 2)
            {

                  double total_time = 0;
                  N = dimension; // overwrite
                  int nthreads = thread_request;
                  int tid, i, j, k;
                  int A[N][N];
                  int B[N][N];
                  int C[N][N];
                  double start = 0;
                  double stop = 0;
                  int samples = 1000;

                  double mean = 0;
                  double mean2 = 0;
                  double time_data[samples];
                  double ind_times = 0;
                  double sdev = 0;
                  // repeat 1000 times so can take an average
                  for (int loops = 0; loops < samples; loops++)
                  {

                        initialise_matrices(A, B, C, N);
                        start = omp_get_wtime(); // take start time
                        multiply_2D(A, B, C, N, nthreads);
                        stop = omp_get_wtime(); // take stop time
                        // print_2D_matrix( C,   N);
                        ind_times = time_for_round(start, stop);
                        total_time = total_time + ind_times;
                        time_data[loops] = ind_times;
                  }
                  mean = total_time / samples;
                  mean2 = (mean / 1000.0);
                  sdev = standard_deviation(mean2, time_data);
                  printf("Using openMP, %d by %d  multiplication algorithm took %f milliseconds to execute on average (%d samples) with %f Standard deviation when %d threads requested \n", N, N, mean, samples, sdev, nthreads);
            }
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

void multiply_2D(int A[N][N], int B[N][N], int C[N][N], int N, int nthreads)
{
      int tid, i, j, k;
#pragma omp parallel shared(A, B, C) private(i, j, k) num_threads(nthreads)
      {
#pragma omp for // collapse(2)

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
}

double standard_deviation(double mean, double data[1000])
{
      double sdev = 0;
      int samples = 1000;
      for (int loops = 0; loops < samples; loops++)
      {
            sdev += sqrtf(pow((data[loops] - mean), 2));
            sdev = sdev / samples;
      }
      return sdev;
}