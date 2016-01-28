#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <CL/cl.h>
#include "simple.h"

#define DATA_SIZE 1024

const char *KernelSource =                 "\n"

// FILL IN HERE!

  "\n";


struct timespec start, stop;

void printTimeElapsed( char *text)
{
  double elapsed = (stop.tv_sec -start.tv_sec)*1000.0
                  + (double)(stop.tv_nsec -start.tv_nsec)/1000000.0;
  printf( "%s: %f msec\n", text, elapsed);
}

void timeDirectImplementation( int count, float* in_a, float* in_b, float *out)
{
  float sum;

  clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &start);
  for (int i = 0; i < count; i++) {
    for (int j = 0; j < count; j++) {
      sum = 0.0;
      for (int k = 0; k < count; k++) {
        sum += in_a[i*count+k] * in_b[k*count+j];
      }
      out[i*count+j] =sum;
    }
  }
  clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &stop);
  printTimeElapsed( "kernel equivalent on host");
}


int main (int argc, char * argv[])
{
  cl_int err;
  cl_kernel kernel;

// define and set work set here:

  size_t global[?];
  size_t local[?];

  if( argc < ?) {
    ???
  } else {
    ???
  }

  printf( "work group size: ???\n", ??? );

  clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &start);

  /* Create data for the run.  */
  float *in_a = NULL;                /* Original data set given to device.  */
  float *in_b = NULL;                /* Original data set given to device.  */
  float *out = NULL;             /* Results returned from device.  */
  int correct;                       /* Number of correct results returned.  */

  int count = DATA_SIZE;
  float sum;
  global[0] = count;
  global[1] = count;

  in_a = (float *) malloc (count * count * sizeof (float));
  in_b = (float *) malloc (count * count * sizeof (float));
  out = (float *) malloc (count * count * sizeof (float));

  /* Fill the vector with random float values.  */
  for (int i = 0; i < count*count; i++) {
    in_a[i] = rand () / (float) RAND_MAX;
    in_b[i] = rand () / (float) RAND_MAX;
  }


  if( argc > 3) {
    printf( "using openCL on host!\n");
    err = initCPU();
  } else  {
    printf( "using openCL on GPU!\n");
    err = initGPU();
  }
  
  if( err == CL_SUCCESS) {
    // Fill in here:
    kernel = setupKernel( KernelSource, "matmul", ???);

    // Fill in here:
    runKernel( kernel, ???, global, local);
  
    clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &stop);

    printKernelTime();
    printTimeElapsed( "CPU time spent");

    /* Validate our results.  */
    correct = 0;
    for (int i = 0; i < count; i++) {
      for (int j = 0; j < count; j++) {
        sum = 0.0;
        for (int k = 0; k < count; k++) {
          sum += in_a[i*count+k] * in_b[k*count+j];
        }
        if ( abs(out[i*count+j] - sum) < 0.0001)
          correct++;
      }
    }

    /* Print a brief summary detailing the results.  */
    printf ("Computed %d/%d %2.0f%% correct values\n", correct, count*count,
            (float)(count*count)/correct*100.f);

    err = clReleaseKernel (kernel);
    err = freeDevice();

    timeDirectImplementation( count, in_a, in_b, out);
    
  }


  return 0;
}


