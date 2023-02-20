#include <iostream>
#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <cmath>

using namespace std;
using namespace chrono;

// Definición de funciones
void parallelMultiply();
void serialMultiply();
void initMat();

// Definición de variables globales
//int A[2][2];
//int B[2][2];
int serialM[3][3];
int parallelM[3][3];

int main() {

    // Multiplicación serial
    auto start = high_resolution_clock::now();
    serialMultiply();
    auto end = high_resolution_clock::now();
    auto serialTimer = duration_cast<microseconds>(end - start);
    cout << "Serial matrix mult finished, elapsed time: "<<  serialTimer.count()/1000 << " milliseconds" <<endl;

    // Multiplicación paralela
    start = high_resolution_clock::now();
    parallelMultiply();
    end = high_resolution_clock::now();
    auto parallelTimer = duration_cast<microseconds>(end - start);
    cout << "Parallel matrix mul finished, elapsed time: "<<  parallelTimer.count()/1000 << " milliseconds" <<endl;

    // Calcular speedUp
    cout << "SpeedUp = " << float(serialTimer.count())/float(parallelTimer.count()) << endl;

    //MultiplyWithOutAMP();
    getchar();
}

void serialMultiply(){
    int A[3][2] = {{1, 4}, {2, 5}, {3, 6}};
    int B[2][3] = {{7, 8, 9}, {10, 11, 12}};
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < 2; inner++) {
                serialM[row][col] += A[row][inner] * B[inner][col];
            }
            cout << serialM[row][col] << "  ";
        }
        cout << "\n";
    }
}

void parallelMultiply(){
    int A[3][2] = {{1, 4}, {2, 5}, {3, 6}};
    int B[2][3] = {{7, 8, 9}, {10, 11, 12}};
#pragma omp parallel for
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < 2; inner++) {
                parallelM[row][col] += A[row][inner] * B[inner][col];
            }
            cout << parallelM[row][col] << "  ";
        }
        cout << "\n";
    }
}
