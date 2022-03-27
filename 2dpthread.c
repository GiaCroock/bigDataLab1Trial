
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "omp.h"
pthread_mutex_t lock;

#define max_dimensions 300

int A[max_dimensions][max_dimensions];
int B[max_dimensions][max_dimensions];
int C[max_dimensions][max_dimensions];
int counter=0;

void *rank2pthread_row(void *arg);
void initialise_matrices(int N);
void print_2D_matrix(int matrix[max_dimensions][max_dimensions], int N);
double time_for_round(double start, double stop);
void pthread_2d_multiply_sequence(int nthreads, int N);
double standard_deviation(double mean, double data[100]) ;

int main(int argc, char *argv[])
{

      for (int dimension = 10; dimension <= 30; dimension = dimension + 10)
      {

            for (int thread_request = 8; thread_request >= 2; thread_request = thread_request / 2)
            {
                  counter=0;
                  double total_time = 0;
                  int N = dimension; // this is the dimensions of the matrix for the round
                  int nthreads = thread_request;
                  double start = 0;
                  double stop = 0;
                  int samples = 100;
                  double mean = 0;
                  double time_data[samples];
                  double ind_times = 0;
                  double sdev=0;
                  
                  // repeat 1000 times so can take an average
                  for (int loops = 0; loops < samples; loops++)
                  {
                   
                        counter=0;
                        srand(time(NULL));

                        initialise_matrices(N);
                        start = omp_get_wtime(); // take start time

                        pthread_2d_multiply_sequence(nthreads, N);

                        stop = omp_get_wtime(); // take stop time
            // Displaying the result matrix
                                                // print_2D_matrix(C, N) ;
                        ind_times = time_for_round(start, stop);
                        total_time = total_time + ind_times;
                        time_data[loops] = ind_times;
                  }
                  mean = total_time / samples;
                  sdev=standard_deviation(mean, time_data);
                  
                  printf("Using Pthread, %d by %d  multiplication algorithm took %f milliseconds to execute on average (%d samples) with %f Standard deviation when %d threads requested \n", N, N, mean, samples, sdev, nthreads);
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

void print_2D_matrix(int matrix[max_dimensions][max_dimensions], int N)
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

void initialise_matrices(int N)
{
      for (int i = 0; i < N; i++)
            for (int k = 0; k < N; k++)
            {
                  A[i][k] = rand() % 20;
                  B[i][k] = rand() % 20;
                  C[i][k] = 0;
            }
}

void *rank2pthread_row(void *arg)
{ // a thread running this function will be responsible for incrementing rows till all elements in row
//
      int N = *((int *)arg);
      int i=0; //because imediately increment on line 119;
      //should be -1 but then get segmentation
      int j=0;
      pthread_mutex_lock(&lock);
      if((counter++)%N==0){j=0, i++;}
      else {j++;}
      pthread_mutex_unlock(&lock);
            for (int k = 0; k < N; k++)
                  C[i][j] += A[i][k] * B[k][j];
                  
}

void pthread_2d_multiply_sequence(int nthreads, int N)
{
      pthread_t threads[nthreads];

      int loopNum = ((N*N) / nthreads); // interger division of the total number of elements divided by the no of created threads

      for (int j = 0; j < loopNum; j++)
      {
            for (int i = 0; i < nthreads; i++)
            {
                  pthread_create(&threads[i], NULL, rank2pthread_row, (void *)&N);
            }

            // joining and waiting for all threads to complete
            for (int i = 0; i < nthreads; i++)
            {
                  pthread_join(threads[i], NULL);
            }
      }

      // remainder of elements to be computetd
      int loopNum2 = (N*N) % nthreads;

      for (int i = 0; i < loopNum2; i++)
      {
            pthread_create(&threads[i], NULL, rank2pthread_row, (void *)(&N));
      }

      // joining and waiting for all remander threads to complete
      for (int i = 0; i < loopNum2; i++)
      {
            pthread_join(threads[i], NULL);
      }
}
double standard_deviation(double mean, double data[1000]) 
{double sdev = 0;
int samples =1000;
for (int loops = 0; loops < samples; loops++)
                  {
                        sdev += sqrtf(pow((data[loops] - mean), 2));
                        sdev = sdev / samples;
                        
                  }
  return sdev;                
}