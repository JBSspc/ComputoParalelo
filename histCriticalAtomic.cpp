#include <iostream>
#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <cmath>

using namespace std;
using namespace chrono;

#define N 1000000000


// Definición de funciones
void initVectors();
int serialHist(vector<int> A, size_t n);
int parallelHist(vector<int> A, size_t n);
void verify();

// Definición de variables globales
vector<int> A(N);
vector<int> sHist(10,0);
vector<int> pHist(10,0);

int main() {
    // Inicializar vectores con números aleatorios
    auto start = high_resolution_clock::now();
    initVectors();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Initialization finished, elapsed time: "<<  duration.count()/1000 << " milliseconds" <<endl;

    // Realizar suma serial
    start = high_resolution_clock::now();
    serialHist(A,N);
    end = high_resolution_clock::now();
    auto serialTimer = duration_cast<microseconds>(end - start);
    cout << "Serial hist finished, elapsed time: "<<  serialTimer.count()/1000 << " milliseconds" <<endl;
    //cout << "Serial Histogram" << sHist << endl;


    // Realizar suma paralela
    start = high_resolution_clock::now();
    parallelHist(A,N);
    end = high_resolution_clock::now();
    auto parallelTimer = duration_cast<microseconds>(end - start);
    cout << "Parallel hist finished, elapsed time: "<<  parallelTimer.count()/1000 << " milliseconds" <<endl;

    // Verificar que los resultados son iguales
    verify();

    // Calcular speedUp
    cout << "SpeedUp = " << float(serialTimer.count())/float(parallelTimer.count()) << endl;



    return 0;
}


/*
* Funtion to initialize two vectors A and B randomly.
*/
void initVectors(){
    //#pragma omp parallel for
    for(int i=0; i<N; i++){
        A[i] =  rand() % 100 ;
    }
}

int serialHist(vector<int> A, size_t n){
    for(int i=0; i<N; i++){
        sHist[A[i]/10]++;
    }
}

int parallelHist(vector<int> A, size_t n){
    int temp=0;
#pragma omp parallel for firstprivate(temp) shared(pHist)
    for(int i=0; i<N; i++){
        temp=A[i]/10;
        //#pragma omp critical
        #pragma mop atomic
        pHist[temp]++;
    }
}

void verify(){
    for (int i = 0; i < 10; ++i) {
        if(sHist[i] != pHist[i]){
            cout <<"Los histogramas no son iguales"<<endl;
            return; // ya no es necesario revisar los demás
        }
    }
    cout <<"Los histogramas son iguales"<<endl;
}
