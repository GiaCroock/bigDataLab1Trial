# bigDataLab1Trial
Each program must simply be compiled and run, and they will automate the averaging of samples and provide the standard deviation for different thread numbers and different matrix dimensions. 

To compile and run the programs, use a Unix environment. 

In the terminal, to compile the different codes:



 gcc rank2TensorMultPThread.c -o rank2TensorMultPThread -lpthread -fopenmp -lgomp -lm 
 gcc rank3TensorMultPThread.c -o rank3TensorMultPThread -lpthread -fopenmp -lgomp -lm 
 gcc rank2TensorMultOpenMP.c -o rank2TensorMultOpenMP -fopenmp -lgomp -lm 
 gcc rank3TensorMultOpenMP.c -o rank3TensorMultOpenMP -fopenmp -lgomp -lm


To run the codes:


./rank2TensorMultPThread
./rank3TensorMultPThread
 ./rank2TensorMultOpenMP
 ./rank3TensorMultOpenMP

If the user wishes to change the number of threads used or the dimensions of the arrays, the outher most for loop found in the source code can be altered.
Changing the 'samples' variable will change the number of samples used in the calculation of the mean execution times
If the user wishes to print the resultant or any other matrix, a print function has been provided (but has been commented out for convenience)
