#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* atgv[])
{
  //Incio de la región paralela
  #pragma omp parallel
  {
      printf("Hello World... from thread =%d\n",
            omp_get_thread_num());
  }
  // fin de la región paralela
}
