#include <iostream>
#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <cmath>


using namespace std;
using namespace chrono;

#define N 90000000

// Definición de funciones
float serialSum(vector <int> A, size_t n);
float parallelSum(vector <int> A, size_t n);
void initVectors();
void verify();

// Definición de variables globales
vector<int> A(N);
float totalS;
float totalP;



int main() {
    // Inicializar vectores con números aleatorios
    auto start = high_resolution_clock::now();
    initVectors();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Initialization finished, elapsed time: "<<  duration.count()/1000 << " milliseconds" <<endl;

    // Realizar suma serial
    start = high_resolution_clock::now();
    serialSum(A,N);
    end = high_resolution_clock::now();
    auto serialTimer = duration_cast<microseconds>(end - start);
    cout << "Serial sum finished, elapsed time: "<<  serialTimer.count()/1000 << " milliseconds" <<endl;


    // Realizar suma paralela
    start = high_resolution_clock::now();
    parallelSum(A,N);
    end = high_resolution_clock::now();
    auto parallelTimer = duration_cast<microseconds>(end - start);
    cout << "Parallel sum finished, elapsed time: "<<  parallelTimer.count()/1000 << " milliseconds" <<endl;


    // Verificar que los resultados son iguales
    verify();
    
    // Número de elementos
    cout << "N = " << N <<endl;
    // Calcular speedUp
    cout << "SpeedUp = " << float(serialTimer.count())/float(parallelTimer.count()) << endl;


    return 0;
}


/*
* FunCtion to initialize two vectors A and B randomly.
*/
void initVectors(){
    //#pragma omp parallel for
    for(int i=0; i<N; i++){
        A[i] =  i * 2; // Rnd Nums 1 - 100
    }
}

float serialSum(vector <int> A, size_t n){
    float totalS=0.;
    for(size_t i= 0; i<N; i++){
        totalS += A[i];
    }
    return totalS;
}

float parallelSum(vector <int> A, size_t n){
    float totalP=0.;
#pragma omp parallel for reduction(+:totalP)
    for(size_t i= 0; i<N; i++){
        totalP += A[i];
    }
    return totalP;

}

void verify(){
    if(totalS != totalP)
        cout<< 0 << endl;
    else
        cout <<1<<endl;

}
