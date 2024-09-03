/*
Computación Paralela y Distribuida - Sección 10
Pablo Andrés Zamora Vásquez - 21780
Diego Andrés Morales Aquino - 21762
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;

#define INFILE "unordered_numbers.csv"
#define OUTFILE "ordered_numbers.csv"

// Realizar QuickSort en un arreglo con OpenMP
void par_qsort(int* data, int lo, int hi) {
    if (lo >= hi) return;
    
    int l = lo;
    int h = hi;
    int p = data[(hi + lo) / 2];

    while (l <= h) {
        while (data[l] < p) l++;
        while (data[h] > p) h--;
        if (l <= h) {
            int tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            l++; h--;
        }
    }
    
    // Paralelizar la recursión del QuickSort
    if (hi - lo > 100) { // Umbral de tolerancia para usar paralelización
        #pragma omp parallel sections
        {
            #pragma omp section
            par_qsort(data, lo, h);
            #pragma omp section
            par_qsort(data, l, hi);
        }
    } else {
        par_qsort(data, lo, h);
        par_qsort(data, l, hi);
    }
}

int main() {
    srand(time(0));  // Inicialización de la semilla para números aleatorios

    int cant_num;
    cout << "Ingresar cantidad de numeros aleatorios a generar: ";
    cin >> cant_num;

    auto start = std::chrono::high_resolution_clock::now(); // Iniciar medición de tiempo

    int* Array = new int[cant_num];
    
    // Abrir el archivo de números desordenados

    ofstream outFile(INFILE);
    if (!outFile) {
        cerr << "No se pudo crear el archivo de salida '" << INFILE << "'.\n";
        delete[] Array;
        return 1;
    }

    // Generar números entre 1 y 1000 y escribirlos en el archivo separados por comas
    for (int i = 0; i < cant_num; i++) {
        Array[i] = rand() % 1000 + 1;  
        outFile << Array[i];
        if (i < cant_num - 1) {
            outFile << ",";
        }
    }
    outFile.close();

    // Leer números del archivo y almacenarlos en memoria
    ifstream inFile(INFILE);
    if (!inFile) {
        cerr << "No se pudo abrir el archivo '" << INFILE << "'.\n";
        delete[] Array; 
        return 1;
    }

    int* numbersArray = new int[cant_num];  // Números leídos del archivo
    char comma;  
    for (int i = 0; i < cant_num; i++) {
        inFile >> numbersArray[i];
        inFile >> comma;  // Ignorar la coma
    }
    inFile.close();

    // Ordenar el arreglo usando la versión paralela de QuickSort
    par_qsort(numbersArray, 0, cant_num - 1);

    // Guardar los números ordenados en el archivo de salida
    ofstream sortedFile(OUTFILE);
    if (!sortedFile) {
        cerr << "No se pudo crear el archivo de salida '" << OUTFILE << "'.\n";
        delete[] Array;
        delete[] numbersArray;
        return 1;
    }
    
    for (int i = 0; i < cant_num; i++) {
        sortedFile << numbersArray[i];
        if (i < cant_num - 1) {
            sortedFile << ",";
        }
    }
    sortedFile.close();

    delete[] Array;
    delete[] numbersArray;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " segundos\n";

    return 0;
}
