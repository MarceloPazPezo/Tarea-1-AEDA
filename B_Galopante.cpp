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
#include <thread>
#include <mutex>
#include <numeric>

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

void Swap(unsigned int* a, unsigned int* b)
{
    unsigned int temp = *a;
    *a = *b;
    *b = temp;
}

indice Partition(unsigned int* arr, indice low, indice high)
{
    unsigned int pivot = arr[high];
    indice i = low;

    for (indice j = low; j < high; j++) {
        if (arr[j] < pivot) {
            if (i != j) {
                Swap(&arr[i], &arr[j]);
            }
            i++;
        }
    }
    Swap(&arr[i], &arr[high]);
    return i;
}

void QuickSort(unsigned int* arr, indice low, indice high)
{
    if (low < high && high != std::numeric_limits<indice>::max()) {
        indice pi = Partition(arr, low, high);
        if (pi > low) {
            QuickSort(arr, low, pi - 1);
        }

        if (pi < high) {
            QuickSort(arr, pi + 1, high);
        }
    }
}

void parallelCountingSort(unsigned int* arr, indice N, unsigned int min_val_datos, unsigned int max_val_datos, unsigned int num_threads)
{
    if (N == 0) return;
    if (min_val_datos > max_val_datos) {
        cerr << "Error en parallelCountingSort: min_val_datos no puede ser mayor que max_val_datos." << endl;
        return;
    }

    indice range_size = static_cast<indice>(max_val_datos) - min_val_datos + 1;

    cout << "Fase de conteo paralelo con " << num_threads << " hilos..." << endl;
    std::vector<std::thread> threads_count;
    std::vector<std::vector<unsigned long long>> local_counts(num_threads, std::vector<unsigned long long>(range_size, 0));

    indice chunk_size = (N + num_threads - 1) / num_threads;

    auto count_chunk_task = [&](unsigned int thread_id, indice start_idx, indice end_idx) {
        for (indice i = start_idx; i < end_idx; ++i) {
            if (arr[i] >= min_val_datos && arr[i] <= max_val_datos) {
                local_counts[thread_id][arr[i] - min_val_datos]++;
            } else {
            }
        }
    };

    for (unsigned int i = 0; i < num_threads; ++i) {
        indice start_idx = i * chunk_size;
        indice end_idx = std::min(start_idx + chunk_size, N);
        if (start_idx < end_idx) {
            threads_count.emplace_back(count_chunk_task, i, start_idx, end_idx);
        }
    }

    for (auto& t : threads_count) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::vector<unsigned long long> global_counts(range_size, 0);
    for (unsigned int val_idx = 0; val_idx < range_size; ++val_idx) {
        for (unsigned int t = 0; t < num_threads; ++t) {
            global_counts[val_idx] += local_counts[t][val_idx];
        }
    }
    local_counts.clear();

    cout << "Calculando posiciones de inicio para reconstruccion paralela..." << endl;
    std::vector<unsigned long long> output_start_indices(range_size + 1, 0);
    for (indice i = 0; i < range_size; ++i) {
        output_start_indices[i+1] = output_start_indices[i] + global_counts[i];
    }

    cout << "Fase de reconstruccion paralela con " << num_threads << " hilos..." << endl;
    std::vector<std::thread> threads_reconstruct;

    indice values_per_thread = (range_size + num_threads - 1) / num_threads;

    auto reconstruct_chunk_task = [&](unsigned int thread_id, indice start_val_idx, indice end_val_idx) {
        for (indice val_idx = start_val_idx; val_idx < end_val_idx; ++val_idx) {
            unsigned int actual_value = min_val_datos + val_idx;
            unsigned long long start_write_pos = output_start_indices[val_idx];
            unsigned long long end_write_pos = output_start_indices[val_idx + 1];
            
            for (unsigned long long k = start_write_pos; k < end_write_pos; ++k) {
                 if (k < N) {
                    arr[k] = actual_value;
                } else {
                     cerr << "Error (hilo " << thread_id << "): k (" << k << ") excedió N (" << N << ") durante la reconstrucción." << endl;
                     return;
                }
            }
        }
    };
    
    for (unsigned int i = 0; i < num_threads; ++i) {
        indice start_val_idx = i * values_per_thread;
        indice end_val_idx = std::min(start_val_idx + values_per_thread, range_size);
        if (start_val_idx < end_val_idx) {
            threads_reconstruct.emplace_back(reconstruct_chunk_task, i, start_val_idx, end_val_idx);
        }
    }

    for (auto& t : threads_reconstruct) {
        if (t.joinable()) {
            t.join();
        }
    }

    cout << "Ordenamiento por conteo paralelo completado ";
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
        // cout << "linf: " << linf << endl;
        // cout << "size - 1:" << size - 1 << endl;
        // cout << "min: " << min(linf * 2, size - 1) << endl;
        indice lsup = min(linf * 2, size - 1);
        return BusquedaBinaria(A, linf - 1, lsup, x);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Error: Debes proporcionar el valor de N como argumento." << endl;
        cerr << "Uso: " << argv[0] << " <valor_de_N> [num_hilos (opcional)]" << endl;
        return 1;
    }

    indice N;
    try {
        N = std::stoull(argv[1]);
        if (N == 0 && argc > 0) {
             cout << "N es 0, no se realizarán operaciones de ordenamiento/búsqueda en el arreglo A." << endl;
        } else if (N <= 0) {
            cerr << "Error: N debe ser un entero positivo (o 0 si se maneja explícitamente)." << endl;
            return 1;
        }
    } catch (const std::invalid_argument& ia) {
        cerr << "Error: Argumento invalido para N: '" << argv[1] << "'. Debe ser un numero." << endl;
        return 1;
    } catch (const std::out_of_range& oor) {
        cerr << "Error: El valor de N ('" << argv[1] << "') esta fuera de rango para un unsigned long long." << endl;
        return 1;
    }

    unsigned int num_hilos = std::thread::hardware_concurrency();
    if (argc > 2) {
        try {
            int hilos_arg = std::stoi(argv[2]);
            if (hilos_arg > 0) {
                num_hilos = hilos_arg;
            } else {
                cout << "Numero de hilos invalido, usando por defecto: " << num_hilos << endl;
            }
        } catch (const std::exception& e) {
            cout << "Argumento de hilos invalido (" << e.what() << "), usando por defecto: " << num_hilos << endl;
        }
    }
    cout << "Usando " << num_hilos << " hilos para operaciones paralelas." << endl;

    const int MIN_VAL = 1, MAX_VAL = 10000000;
    unsigned int* A = nullptr;
    string output_filename = "resultados_busqueda_" + std::to_string(N) + ".txt";
    cout << "ADVERTENCIA: Intentando asignar memoria para N = " << N << " elementos." << endl;
    cout << "Esto requiere aproximadamente " << (N * sizeof(unsigned int)) / (1024.0 * 1024.0 * 1024.0) << " GiB de RAM." << endl;
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
    // cout << endl << endl;

    // cout << "Ordenando el arreglo A por MergeSort... ";
    auto inicio = chrono::high_resolution_clock::now();
    if (N > 0) {
        // MergeSort(A, 0, N);
        // QuickSort(A, 0, N - 1);
        // sort(A, A + N);
        parallelCountingSort(A, N, MIN_VAL, MAX_VAL, num_hilos);
    }
    auto termino = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> tiempoMergeSort = termino - inicio;
    cout << "y ordenado en " << fixed << setprecision(6) << tiempoMergeSort.count() << " ms." << endl;
    cout << endl;

    // cout << "Arreglo ordenado: " << endl;
    // for (indice i = 0; i < N; ++i) {
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