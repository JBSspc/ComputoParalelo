#include <>

srand (time(NULL));
const int n = 10000;
const int bins = 10;

// int array de apuntadores
int *A = new int[n];
int *hist = new int [bins];

//Lenar el array 'A' con números aleatorios del 1 al 100
for (int i=0; i<n; i++){
  A[i] = rand() % 100 + 1;
} 
 
