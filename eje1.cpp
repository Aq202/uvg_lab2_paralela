#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

#define INFILE "unordered_numbers.csv"
#define OUTFILE "ordered_numbers.csv"

// Realizar QuickSort en un arreglo
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
    
    par_qsort(data, lo, h);
    par_qsort(data, l, hi);
}

int main() {
    srand(time(0));  // Inicialización de la semilla para números aleatorios

    int cant_num;
    cout << "Ingresar cantidad de numeros aleatorios a generar: ";
    cin >> cant_num;

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

    // Ordenar el arreglo de numeros
    par_qsort(numbersArray, 0, cant_num - 1);

    // abrir archivo de salida para nums ordenados
    ofstream sortedFile(OUTFILE);
    if (!sortedFile) {
        cerr << "No se pudo crear el archivo de salida.\n";
        delete[] Array;
        delete[] numbersArray;
        return 1;
    }

    // Escribir numeros ordenados en en el archivo
    for (int i = 0; i < cant_num; i++) {
        sortedFile << numbersArray[i];
        if (i < cant_num - 1) {
            sortedFile << ",";
        }
    }
    sortedFile.close();

    // Liberar memoria
    delete[] Array;
    delete[] numbersArray;

    return 0;
}
