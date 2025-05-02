#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <random>
#include <limits>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

using indice = unsigned long long;

void Merge(unsigned int* A, indice left, indice mid, indice right) {
    indice sizeLeft = mid - left;
    indice sizeRight = right - mid;

    vector<unsigned int> tempLeft(A + left, A + mid);
    vector<unsigned int> tempRight(A + mid, A + right);

    indice i = 0;
    indice j = 0;
    indice k = left;

    while (i < sizeLeft && j < sizeRight) {
        if (tempLeft[i] <= tempRight[j]) {
            A[k] = tempLeft[i];
            i++;
        } else {
            A[k] = tempRight[j];
            j++;
        }
        k++;
    }

    while (i < sizeLeft) {
        A[k] = tempLeft[i];
        i++;
        k++;
    }

    while (j < sizeRight) {
        A[k] = tempRight[j];
        j++;
        k++;
    }
}

void MergeSort(unsigned int* A, indice left, indice right) {
    if (right - left <= 1) {
        return;
    }

    indice mid = left + (right - left) / 2;

    MergeSort(A, left, mid);
    MergeSort(A, mid, right);

    Merge(A, left, mid, right);
}

int bBinaria(short A[], int linf, int size, int x)
{
    // cout<<linf + (size / 2)<<endl;
    if (size == 0)
    {
        return -1;
    }
    if (A[linf + (size / 2)] == x)
    {
        return linf + (size / 2);
    }
    if (A[linf + (size / 2)] < x)
    {
        return bBinaria(A, linf + (size / 2) + 1, size / 2, x);
    }
    else
    {
        return bBinaria(A, linf, size / 2, x);
    }
}

int bGalopante(short A[], int size, int x)
{
    if (size == 0)
    {
        return -1;
    }
    else
    {
        int linf = 1;
        while (linf * 2 < size && A[linf * 2] < x)
        {
            linf = linf * 2;
        }
        return bBinaria(A, linf - 1, size - linf, x);
    }
}

/**
 * @brief 
 * 
 * @param A: Puntero al arreglo
 * @param i: Indice inicial 
 * @param f: Indice final
 * @param x: Valor a buscar
 * @return: Posicion del valor buscado o -1 si no se encuentra
 */
indice BusquedaBinaria(const unsigned int* A, indice i, indice f, unsigned int x)
{
    if (i > f) {
        return std::numeric_limits<indice>::max();
    }

    indice mid = i + (f - i) / 2;

    if (A[mid] == x) {
        return mid;
    }
    else if (A[mid] > x) {
         if (mid == 0) {
            return std::numeric_limits<indice>::max();
         }
        return BusquedaBinaria(A, i, mid - 1, x);
    }
    else {
        return BusquedaBinaria(A, mid + 1, f, x);
    }
}

/**
 * @brief 
 * 
 * @param A: Puntero al arreglo 
 * @param size: Tamaño del arreglo 
 * @param x: Valor a buscar
 * @return : Posicion del valor buscado o -1 si no se encuentra
 */
indice BusquedaGalopante(const unsigned int* A, indice size, unsigned int x)
{
    if (size == 0)
    {
        return numeric_limits<indice>::max();
    }
    else
    {
        indice linf = 1;
        while (linf * 2 < size && A[linf * 2] < x)
        {
            linf = linf * 2;
        }
        // return BusquedaBinaria(A, linf, size, x);
        // return BusquedaBinaria(A, linf - 1, size - linf, x);
        return BusquedaBinaria(A, linf - 1, size - 1, x);
    }
}

// int main()
// {
//     int tam = 10000;//tamaño del arreglo
//     short Arr[tam]; //= {1,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59};
//     unsigned t0, t1;
//     short val;
//     for (int i = 0; i < tam; i++)
//     {
//         val = short(rand() % ((i + 1) * 3));//llenado de randoms en orden
//         if (val > Arr[i - 1] || i == 0)
//         {
//             Arr[i] = val;
//             // cout<<val<<" ";
//         }
//         else
//         {
//             i--;
//         }
//     }
//     cout << endl;
//     int valR = int(rand() % tam);
//     cout << "posicion: " << valR << endl;
//     valR = Arr[valR];
//     cout << "valor: " << valR << endl;
//     t0 = clock();
//     int res = bGalopante(Arr, tam, valR);
//     t1 = clock();
//     double tiempo = double((t1 - t0)  / CLOCKS_PER_SEC);
//     if (res == -1)
//     {
//         cout << "Valor no encontrado, tiempo: " << tiempo << endl;
//     }
//     else
//     {
//         cout << "El valor " << valR << " fue encontrado en la posicion:" << res << " tiempo: " << tiempo << " ms" << endl;
//     }
//     return 0;
// }

int main()
{
    const indice N = 5000000000;
    const int MIN_VAL = 1, MAX_VAL = 10000000;
    unsigned int* A = nullptr;
    string output_filename = "resultados_busqueda_" + std::to_string(N) + ".txt";
    cout << "ADVERTENCIA: Intentando asignar memoria para N = " << N << " elementos." << endl;
    cout << "Esto requiere aproximadamente " << (N * sizeof(unsigned int)) / (1024.0 * 1024.0 * 1024.0) << " GiB de RAM." << endl;
    cout << "La operacion podria fallar o ser muy lenta debido a restricciones de memoria." << endl;
    try {
        A = new unsigned int[N];
        cout << "Asignacion de memoria exitosa." << endl;
    } catch (const bad_alloc& e) {
        cerr << "Error al asignar memoria para A (size " << N << "): " << e.what() << endl;
        cerr << "Intenta reducir N o asegúrate de tener suficiente RAM y espacio de intercambio." << endl;
        return 1;
    }

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<> distrib(MIN_VAL, MAX_VAL);

    cout << "Generando datos para el arreglo A["<< N << "]" << endl;
    for (indice i = 0; i < N; ++i) {
        A[i] = distrib(gen);
        // cout << A[i] << " ";
    }
    cout << endl << endl;

    cout << "Ordenando el arreglo A por MergeSort... ";
    auto inicio = chrono::high_resolution_clock::now();
    MergeSort(A, 0, N);
    auto termino = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tiempoMergeSort = termino - inicio;
    cout << "ordenado en " << fixed << setprecision(6) << tiempoMergeSort.count() << " ms." << endl;
    cout << endl;

    // cout << "Arreglo ordenado: " << endl;
    // for (int i = 0; i < N; ++i) {
    //     cout << A[i] << " ";
    // }
    // cout << endl << endl;
    
    ofstream outputFile;
    outputFile.open(output_filename);
    if (!outputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de salida: " << output_filename << endl;
        delete[] A;
        return 1;
    }
    cout << "Los resultados se guardarán en: " << output_filename << endl << endl;

    vector<int> claves = {10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000};
    uniform_int_distribution<> key_distrib(MIN_VAL, MAX_VAL);

    ostringstream oss_header;
    oss_header << setw(15) << "N de claves"
               << setw(25) << "std::binary_search (ms)"
               << setw(35) << "Valores encontrados (std::bs)"
               << setw(25) << "Busqueda Binaria (ms)"
               << setw(35) << "Valores encontrados (Binaria)"
               << setw(25) << "Busqueda Galopante (ms)"
               << setw(35) << "Valores encontrados (Galopante)";
    string header = oss_header.str();

    string separator = string(195, '-');

    outputFile << header << endl;
    outputFile << separator << endl;
    cout << header << endl;
    cout << separator << endl;

    for (int n : claves) {
        outputFile << setw(15) << n;
        cout << setw(15) << n;

        vector<unsigned int> keys(n);
        for (int i = 0; i < n; ++i) {
            keys[i] = key_distrib(gen);
            // cout << keys[i] << " ";
        }
        // cout << endl;


        int valoresEncontradosBS = 0;
        inicio = chrono::high_resolution_clock::now();
        for (unsigned int x : keys) {
            if (binary_search(A, A + N, x) == true) {
                valoresEncontradosBS++;
            }
        }
        termino = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tiempoBS = termino - inicio;
        outputFile << fixed << setprecision(6) << setw(25) << tiempoBS.count();
        outputFile << fixed << setprecision(0) << setw(35) << valoresEncontradosBS;
        cout << fixed << setprecision(6) << setw(25) << tiempoBS.count();
        cout << fixed << setprecision(0) << setw(35) << valoresEncontradosBS;

        
        int valoresEncontradosBusquedaBinaria = 0;
        inicio = chrono::high_resolution_clock::now();
        for (unsigned int x : keys) {
            if (BusquedaBinaria(A, 0, N-1, x) != numeric_limits<indice>::max()) {
                valoresEncontradosBusquedaBinaria++;
            }
        }
        termino = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tiempoBusquedaBinaria = termino - inicio;
        outputFile << fixed << setprecision(6) << setw(25) << tiempoBusquedaBinaria.count();
        outputFile << fixed << setprecision(0) << setw(35) << valoresEncontradosBusquedaBinaria;
        cout << fixed << setprecision(6) << setw(25) << tiempoBusquedaBinaria.count();
        cout << fixed << setprecision(0) << setw(35) << valoresEncontradosBusquedaBinaria;

        int valoresEncontradosBusquedaGalopante = 0;
        inicio = chrono::high_resolution_clock::now();
        for (unsigned int x : keys) {
            if (BusquedaGalopante(A, N, x) != numeric_limits<indice>::max()) {
                valoresEncontradosBusquedaGalopante++;
            }
        }
        auto termino = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tiempoBusquedaGalopante = termino - inicio;
        outputFile << fixed << setprecision(6) << setw(25) << tiempoBusquedaGalopante.count();
        outputFile << fixed << setprecision(0) << setw(35) << valoresEncontradosBusquedaGalopante << endl;
        cout << fixed << setprecision(6) << setw(25) << tiempoBusquedaGalopante.count();
        cout << fixed << setprecision(0) << setw(35) << valoresEncontradosBusquedaGalopante << endl;
    }
    outputFile.close();
    cout << endl << "Resultados guardados en " << output_filename << endl;

    cout << "\nLiberando memoria del arreglo A..." << endl;
    delete[] A;
    A = nullptr;
    cout << "Memoria liberada." << endl;

    return 0;
}