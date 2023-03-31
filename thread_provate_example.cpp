#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static int x = 0;
#pragma omp threadprivate(x)

#pragma omp parallel default(none) shared (A,B,C) num_threads(4)
{
  int tid = omp_get_thred_num();
  C[tid] = A[tid] + B[tid];
  
  if(tid == 0)
    x = 0;
}
