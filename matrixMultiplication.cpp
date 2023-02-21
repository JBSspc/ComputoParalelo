#include <iostream>
#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <cmath>

using namespace std;
using namespace chrono;
#define N 500
// Definición de funciones
void parallelMultiply1();
void parallelMultiply2();
void parallelMultiply3();
void serialMultiply();
void initMat();
void verify();

// Definición de variables globales
vector<vector<int>> A(N,vector<int>(N));
vector<vector<int>> B(N,vector<int>(N));
vector<vector<int>> serialM(N,vector<int>(N));
vector<vector<int>> parallelM(N,vector<int>(N));

int main() {

    // Multiplicación serial
    auto start = high_resolution_clock::now();
    initMat();
    auto end = high_resolution_clock::now();
    auto initTimer = duration_cast<microseconds>(end - start);
    cout << "Initialization finished, elapsed time: "<<  initTimer.count()/1000 << " milliseconds" <<endl;

    // Multiplicación serial
    start = high_resolution_clock::now();
    serialMultiply();
    end = high_resolution_clock::now();
    auto serialTimer = duration_cast<microseconds>(end - start);
    cout << "Serial matrix mult finished, elapsed time: "<<  serialTimer.count()/1000 << " milliseconds" <<endl;

    // Multiplicación paralela
    start = high_resolution_clock::now();
    parallelMultiply1(); //Nivel 1
    //parallelMultiply2(); // Nivel 2
    //parallelMultiply3(); // Nivel 3
    end = high_resolution_clock::now();
    auto parallelTimer = duration_cast<microseconds>(end - start);
    cout << "Parallel matrix mul finished, elapsed time: "<<  parallelTimer.count()/1000 << " milliseconds" <<endl;

    // Calcular speedUp
    cout << "SpeedUp = " << float(serialTimer.count())/float(parallelTimer.count()) << endl;
    verify();
}

void initMat(){
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j]= rand() % 10 + 1;
            B[i][j] = rand() % 10 + 1;
        }
    }
}

void serialMultiply(){
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < N; inner++) {
                serialM[row][col] += A[row][inner] * B[inner][col];
            }
            //cout << serialM[row][col] << "  ";
        }
        //cout << "\n";
    }
}

void parallelMultiply1(){
#pragma omp parallel for
    for (int row = 0; row < N; row++) { // Efecto sobre las filas
        for (int col = 0; col < N; col++) { // Efecto sobre las columnas
            for (int inner = 0; inner < N; inner++) { // Efecto sobre filas y columnas
                parallelM[row][col] += A[row][inner] * B[inner][col];
            }
            //cout << parallelM[row][col] << "  "; // descomentar para imprimir mat
        }
        //cout << "\n"; // descomentar para imprimir
    }
}

void parallelMultiply2(){
    for (int row = 0; row < N; row++) { // Efecto sobre las filas
        #pragma omp parallel for
        for (int col = 0; col < N; col++) { // Efecto sobre las columnas
            for (int inner = 0; inner < N; inner++) { // Efecto sobre filas y columnas
                parallelM[row][col] += A[row][inner] * B[inner][col];
            }
            //cout << parallelM[row][col] << "  "; // descomentar para imprimir mat
        }
        //cout << "\n"; // descomentar para imprimir
    }
}

void parallelMultiply3(){
//#pragma omp parallel for
    int temporal;
    for (int row = 0; row < N; row++) { // Efecto sobre las filas
        //#pragma omp parallel for
        for (int col = 0; col < N; col++) { // Efecto sobre las columnas
            temporal=0; // para resetear la suma
#pragma omp parallel for reduction(+:temporal)
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < N; inner++) { // Efecto sobre filas y columnas
                temporal+= A[row][inner] * B[inner][col];
            }
            parallelM[row][col]= temporal;
            //cout << parallelM[row][col] << "  "; // descomentar para imprimir mat
        }
        //cout << "\n"; // descomentar para imprimir
    }
}

void verify(){
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if(serialM[i][j] != parallelM[i][j]){
                cout <<"Las matrices no son iguales"<<endl;
                return; // ya no es necesario revisar los demás
            }
        }
    }
    cout <<"Las matrices son iguales"<<endl;
}
