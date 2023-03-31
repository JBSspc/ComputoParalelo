// AUTOR: Sofia Palacios Cuevas
// FECHA: 31 MAR 2023
// COMPUTO PARALELO - PARCIAL 1

// nuestras librerías
#include <iostream>
#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <cmath>


using namespace std;
using namespace chrono;

#define N 100000000

// Definición de funciones
float parallelSum(vector <int> A, size_t n);
void initVectors();
float serialMinCent(vector <int> X1, vector <int> X2, vector <int> Y1, vector <int> Y2 , size_t n);
float parallelMinCent(vector <int> X1, vector <int> X2, vector <int> Y1, vector <int> Y2 , size_t n);
float serialMinArea(vector <int> X1, vector <int> X2, vector <int> Y1, vector <int> Y2 , size_t n);
float parallelMinArea(vector <int> X1, vector <int> X2, vector <int> Y1, vector <int> Y2 , size_t n);

// Definición de variables globales
vector<int> X1(N);
vector<int> X2(N);
vector<int> Y1(N);
vector<int> Y2(N);
vector<float> As(N);
vector<float> Ap(N);


float MINL;
float MINAL;
float MINs;
float MINp;
float MINAs;
float MINAp;
vector<float> d(N);
vector<float> Cx(N);
vector<float> Cy(N);



int main() {
    // Inicializar vectores con números aleatorios
    auto start = high_resolution_clock::now();
    initVectors();
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Initialization finished, elapsed time: "<<  duration.count()/1000 << " milliseconds" <<endl;

    //primer problema Serial
    start = high_resolution_clock::now();
    serialMinCent(X1,X2,Y1,Y2,N);
    end = high_resolution_clock::now();
    auto serialTimer = duration_cast<microseconds>(end - start);
    cout << "Serial min distance finished, elapsed time: "<<  serialTimer.count()/1000 << " milliseconds" <<endl;
    cout << "Min distancia al origen (Serial) = " << MINs << endl;

    // primer problema Paralelo
    start = high_resolution_clock::now();
    parallelMinCent(X1,X2,Y1,Y2,N);
    end = high_resolution_clock::now();
    auto parallelTimer = duration_cast<microseconds>(end - start);
    cout << "Parallel min distance finished, elapsed time: "<<  parallelTimer.count()/1000 << " milliseconds" <<endl;
    cout << "Min distancia al origen (Paralelo) = " << MINp << endl;


    // Número de elementos
    cout << "N = " << N <<endl;
    // Calcular speedUp
    cout << "SpeedUp Problema 1 (min dis cent al origen) = " << float(serialTimer.count())/float(parallelTimer.count()) << endl;
    cout << "Eficiencia Problema 1 (min area) =" << float(serialTimer.count())/float(parallelTimer.count())/8<< endl;

    //segundo problema Serial
    start = high_resolution_clock::now();
    serialMinArea(X1,X2,Y1,Y2,N);
    end = high_resolution_clock::now();
    auto serialTimer2 = duration_cast<microseconds>(end - start);
    cout << "\nSerial min area finished, elapsed time: "<<  serialTimer2.count()/1000 << " milliseconds" <<endl;
    cout << "Min area (Serial) = " << MINAs << endl;

    // segundo problema Paralelo
    start = high_resolution_clock::now();
    parallelMinArea(X1,X2,Y1,Y2,N);
    end = high_resolution_clock::now();
    auto parallelTimer2 = duration_cast<microseconds>(end - start);
    cout << "Parallel min area finished, elapsed time: "<<  parallelTimer2.count()/1000 << " milliseconds" <<endl;
    cout << "Min area (Paralelo) = " << MINAp << endl;

    // Número de elementos
    cout << "N = " << N <<endl;
    // Calcular speedUp
    cout << "SpeedUp Problema 2 (min area) = " << float(serialTimer2.count())/float(parallelTimer2.count()) << endl;
    cout << "Eficiencia Problema 2 (min area) =" << float(serialTimer2.count())/float(parallelTimer2.count())/8<< endl;
    return 0;
}


/*
* FunCtion to initialize two vectors A and B randomly.
*/
void initVectors(){
    //#pragma omp parallel for
    for(int i=0; i<N; i++){
        X1[i] =  rand() % 50 +1;
        X2[i] =  rand() % 50 +1;
        Y1[i] =  rand() % 50 + 1;
        Y2[i] =  rand() % 50 +1;
    }
}

// PROBLEMA 1 MINIMA DISTANCIA DEL CENTROIDE AL ORIGEN
float serialMinCent(vector <int> X1, vector <int> X2, vector <int> Y1, vector <int> Y2 , size_t n){
    float MINL=999999.0;
    float MINs =999999.0;
    vector<float> d(N);
    vector<float> Cx(N);
    vector<float> Cy(N);

    for(size_t i= 0; i<N; i++){
        Cx[i] = (X1[i] + 2*X2[i])/3;
        Cy[i] = (2*Y1[i] + Y2[i])/3;
        d[i]= (pow((Cx[i]-0),2) + pow((Cy[i]-0),2),(1/2));
        if (d[i]<MINL) MINL = d[i];
    }

    if (MINL < MINs) MINs = MINL;

    return MINs;
}


float parallelMinCent(vector <int> X1, vector <int> X2, vector <int> Y1, vector <int> Y2 , size_t n){
    float MINL=999999.0;
    float MINp =999999.0;
    vector<float> d(N);
    vector<float> Cx(N);
    vector<float> Cy(N);
#pragma omp for
    for(size_t i= 0; i<N; i++){
        Cx[i] = (X1[i] + 2*X2[i])/3;
        Cy[i] = (2*Y1[i] + Y2[i])/3;
        d[i]= (pow((Cx[i]-0),2) + pow((Cy[i]-0),2),(1/2));
        if (d[i]<MINL) MINL = d[i];
    }
#pragma omp critical
    if (MINL < MINp) MINp = MINL;

    return MINp;
}


// PROBLEMA 2 AREA
float serialMinArea(vector <int> X1, vector <int> X2, vector <int> Y1, vector <int> Y2 , size_t n){
    float MINAL=999999.0;
    float MINAs =999999.0;
    vector<float> As(N);

    for(size_t i= 0; i<N; i++){
        As[i]= ((X2[i]-X1[i])*(Y2[i]-Y1[i]))/2;
        if (As[i]<MINAL) MINAL = As[i];
    }

    if (MINAL < MINAs) MINAs = MINAL;

    return MINAs;
}


float parallelMinArea(vector <int> X1, vector <int> X2, vector <int> Y1, vector <int> Y2 , size_t n){
    float MINAL=999999.0;
    float MINAp =999999.0;
    vector<float> Ap(N);
#pragma omp for
    for(size_t i= 0; i<N; i++){
        Ap[i]= ((X2[i]-X1[i])*(Y2[i]-Y1[i]))/2;
        if (Ap[i]<MINL) MINL = Ap[i];
    }
#pragma omp critical
    if (MINAL < MINAp) MINAp = MINAL;

    return MINAp;
}