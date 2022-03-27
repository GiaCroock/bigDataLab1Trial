#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "omp.h"

#define max_dimensions 300
pthread_mutex_t lock;

int A[max_dimensions][max_dimensions][max_dimensions];
int B[max_dimensions][max_dimensions][max_dimensions];
int C[max_dimensions][max_dimensions][max_dimensions];
int counter = 0;

double time_for_round(double start, double stop);
void *rank3pthread(void *arg);
double standard_deviation(double mean, double data[1000]);
void initialise_matrices(int N);
void pthread_3d_multiply_sequence(int nthreads, int N);
void print_3D_matrix(int matrix[max_dimensions][max_dimensions][max_dimensions], int N);

int main(int argc, char *argv[])
{
      for (int dimension = 10; dimension <= 50; dimension = dimension + 10)
      {

            for (int thread_request = 8; thread_request >= 2; thread_request = thread_request / 2)
            {
                  /* char *a = argv[1];
                   int N = atoi(a);
                   char *b = argv[2];
                   int nthreads = atoi(b);*/
                  counter = 0;
                  double total_time = 0;
                  int N = dimension; // overwrite
                  int nthreads = thread_request;
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
                        counter = 0;
                        srand(time(NULL));
                        initialise_matrices(N);
                        start = omp_get_wtime();
                        pthread_3d_multiply_sequence(nthreads, N);
                        // print_3D_matrix(C, N) ;
                        stop = omp_get_wtime(); // take stop time
                        ind_times = time_for_round(start, stop);
                        total_time = total_time + ind_times;
                        time_data[loops] = ind_times;
                  }

                  mean = total_time / samples;
                  mean2 = (mean / 1000.0);
                  sdev = standard_deviation(mean2, time_data);

                  printf("Using Pthread, %d by %d by %d multiplication algorithm took %f milliseconds to execute on average (%d samples) with %f Standard deviation when %d threads requested \n", N, N, N, mean, samples, sdev, nthreads);
            }
            printf("\n");
      }

      return 0;
}

void *rank3pthread(void *arg)
{
      int N = *((int *)arg);
      pthread_mutex_lock(&lock);
      int i = counter++; // i denotes row number of resultant matC
      pthread_mutex_unlock(&lock);
      for (int d = 0; d < N; d++)
            for (int j = 0; j < N; j++)
                  for (int k = 0; k < N; k++)

                        C[d][i][j] += A[d][i][k] * B[d][k][j];
}

double time_for_round(double start, double stop)
{
      double time_taken = (stop - start) * 1000.0;
      return time_taken; // returns time in milliseconds
}

void initialise_matrices(int N)
{
      for (int i = 0; i < N; i++)
      {
            for (int j = 0; j < N; j++)
            {
                  for (int k = 0; k < N; k++)
                  {
                        A[i][j][k] = rand() % 20;
                        B[i][j][k] = rand() % 20;
                        C[i][j][k] = 0;
                  }
            }
      }
}

void pthread_3d_multiply_sequence(int nthreads, int N)
{
      pthread_t threads[nthreads];

      int loopNum = (N / nthreads); // interger division of the total number of rows divided by the no of created threads

      for (int j = 0; j < loopNum; j++)
      {
            for (int i = 0; i < nthreads; i++)
            {
                  pthread_create(&threads[i], NULL, rank3pthread, (void *)&N);
            }

            // joining and waiting for all threads to complete
            for (int i = 0; i < nthreads; i++)
            {
                  pthread_join(threads[i], NULL);
            }
      }

      // remainder of rows to be computetd
      int loopNum2 = N % nthreads;

      for (int i = 0; i < loopNum2; i++)
      {
            pthread_create(&threads[i], NULL, rank3pthread, (void *)(&N));
      }

      // joining and waiting for all remander threads to complete
      for (int i = 0; i < loopNum2; i++)
      {
            pthread_join(threads[i], NULL);
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

void print_3D_matrix(int matrix[max_dimensions][max_dimensions][max_dimensions], int N)
{
      printf("Resultant Matrix C");
      printf("\n");
      for (int i = 0; i < N; i++)
      {
            for (int j = 0; j < N; j++)
            {
                  for (int k = 0; k < N; k++)
                  {
                        printf("%d ", C[i][j][k]);
                  }
                  printf("\n");
            }
            printf("\n");
      }
}