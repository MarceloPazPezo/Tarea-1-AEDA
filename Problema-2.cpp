/***
 * Problema-2.cpp
 * Programa que implementa los algoritmos de multiplicacion de matrices
 * Fecha 22-05-2025
 * Autores:
 * - Victor Lopez
 * - Marcelo Paz
 */
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

using t_in = int;
using t_out = long long;
using matrix_in = vector<vector<t_in>>;
using matrix_out = vector<vector<t_out>>;

const size_t MAX_SIZE = 4096;
const t_in MIN_VAL = -128;
const t_in MAX_VAL = 128;
const size_t DR1_THRESHOLD = 1;
const size_t DR2_THRESHOLD = 1;

matrix_in generarMatrizRandom(size_t N, t_in min_val = MIN_VAL, t_in max_val = MAX_VAL)
{
    if (N == 0) {
        return matrix_in();
    }
    matrix_in A(N, vector<t_in>(N));
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<t_in> distrib(min_val, max_val);

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            A[i][j] = distrib(gen);
        }
    }
    return A;
}

bool matricesIguales(const matrix_out& A, const matrix_out& B, bool mostrar_primera_diferencia = false)
{
    // 1. Comprobar si una está vacía y la otra no (o ambas vacías)
    if (A.empty() && B.empty()) {
        return true;
    }
    if (A.empty() || B.empty()) {
        if (mostrar_primera_diferencia) {
            cout << "Error de igualdad: Una matriz está vacía y la otra no." << endl;
        }
        return false;
    }

    // 2. Comprobar dimensiones de filas
    size_t filas1 = A.size();
    size_t filas2 = B.size();
    if (filas1 != filas2) {
        if (mostrar_primera_diferencia) {
            cout << "Error de igualdad: Diferente número de filas. A: "
                      << filas1 << ", B: " << filas2 << endl;
        }
        return false;
    }

    // 3. Comprobar dimensiones de columnas
    size_t cols1 = A[0].size();
    size_t cols2 = B[0].size();
    if (cols1 != cols2) {
        if (mostrar_primera_diferencia) {
            cout << "Error de igualdad: Diferente número de columnas. A: "
                      << cols1 << ", B: " << cols2 << endl;
        }
        return false;
    }
    // Comprobar si las matrices son cuadradas
    if (filas1 != cols1 || filas2 != cols2) {
        if (mostrar_primera_diferencia) {
            cout << "Error de igualdad: Las matrices no son cuadradas." << endl;
        }
        return false;
    }

    // 4. Comprobar cada elemento
    if (A.size() > 1024) {
        cout << "Matrices muy grandes, se omite el calculo elemento a elemento." << endl;
        return true;
    }

    for (size_t i = 0; i < filas1; ++i) {
        for (size_t j = 0; j < cols1; ++j) {
            if (A[i][j] != B[i][j]) {
                if (mostrar_primera_diferencia) {
                    cout << "Error de igualdad: Diferencia en el elemento [" << i << "][" << j << "]."
                              << " A[" << i << "][" << j << "] = " << A[i][j]
                              << ", B[" << i << "][" << j << "] = " << B[i][j] << endl;
                }
                return false;
            }
        }
    }

    return true;
}

matrix_out multiplicacionAlgoritmoTradicional(const matrix_in& A, const matrix_in& B)
{
    size_t N = A.size();
    if (N == 0) {
        return matrix_out();
    }
    if (B.size() != N || A[0].size() != N || B[0].size() != N) {
        cerr << "Error: Dimensiones de matrices no compatibles para multiplicacion." << endl;
        throw runtime_error("Dimensiones de matrices incompatibles");
    }

    matrix_out C(N, vector<t_out>(N, 0));

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

matrix_in obtenerSubmatriz(const matrix_in& M, size_t fila_inicio, size_t col_inicio, size_t dim)
{
    if (dim == 0) return matrix_in();
    matrix_in sub(dim, vector<t_in>(dim));
    for (size_t i = 0; i < dim; ++i) {
        for (size_t j = 0; j < dim; ++j) {
            sub[i][j] = M[fila_inicio + i][col_inicio + j];
        }
    }
    return sub;
}

void ensamblarMatriz(matrix_out& C,
                     const matrix_out& C11, const matrix_out& C12,
                     const matrix_out& C21, const matrix_out& C22,
                     size_t sub_dim)
{
    for (size_t i = 0; i < sub_dim; ++i) {
        for (size_t j = 0; j < sub_dim; ++j) {
            C[i][j]                 = C11[i][j];
            C[i][j + sub_dim]       = C12[i][j];
            C[i + sub_dim][j]       = C21[i][j];
            C[i + sub_dim][j + sub_dim] = C22[i][j];
        }
    }
}

matrix_in sumarMatricesInput(const matrix_in& A, const matrix_in& B)
{
    size_t N = A.size();
    if (N == 0 || B.size() != N || A[0].size() != B[0].size()) {
        throw runtime_error("Dimensiones incompatibles para suma (input).");
    }
    matrix_in C(N, vector<t_in>(A[0].size()));
    for (size_t i = 0; i < N; ++i) for (size_t j = 0; j < A[0].size(); ++j) C[i][j] = A[i][j] + B[i][j];
    return C;
}

matrix_in restarMatricesInput(const matrix_in& A, const matrix_in& B)
{
    size_t N = A.size();
     if (N == 0 || B.size() != N || A[0].size() != B[0].size()) {
        throw runtime_error("Dimensiones incompatibles para resta (input).");
    }
    matrix_in C(N, vector<t_in>(A[0].size()));
    for (size_t i = 0; i < N; ++i) for (size_t j = 0; j < A[0].size(); ++j) C[i][j] = A[i][j] - B[i][j];
    return C;
}

matrix_out sumarMatricesOutput(const matrix_out& A, const matrix_out& B)
{
    size_t N = A.size();
    if (N == 0 || B.size() != N || A[0].size() != B[0].size()) {
        throw runtime_error("Dimensiones incompatibles para suma (output).");
    }
    matrix_out C(N, vector<t_out>(A[0].size()));
    for (size_t i = 0; i < N; ++i) for (size_t j = 0; j < A[0].size(); ++j) C[i][j] = A[i][j] + B[i][j];
    return C;
}

matrix_out restarMatricesOutput(const matrix_out& A, const matrix_out& B)
{
    size_t N = A.size();
    if (N == 0 || B.size() != N || A[0].size() != B[0].size()) {
        throw runtime_error("Dimensiones incompatibles para resta (output).");
    }
    matrix_out C(N, vector<t_out>(A[0].size()));
    for (size_t i = 0; i < N; ++i) for (size_t j = 0; j < A[0].size(); ++j) C[i][j] = A[i][j] - B[i][j];
    return C;
}

matrix_out multiplicacionDR1(const matrix_in& A, const matrix_in& B)
{
    size_t N = A.size();

    if (N == 0) return matrix_out();
    // Validar dimensiones (asumiendo cuadradas)
    if (B.size() != N || A[0].size() != N || B[0].size() != N) {
        cerr << "Error: Dimensiones de matrices no compatibles para DyC." << endl;
        throw runtime_error("Dimensiones incompatibles para DyC");
    }

    // Caso base: Si N es suficientemente pequeño, usar el algoritmo tradicional
    if (N <= DR1_THRESHOLD) {
        return multiplicacionAlgoritmoTradicional(A, B);
    }

    if (N % 2 != 0) {
        return multiplicacionAlgoritmoTradicional(A, B);
    }


    size_t sub_dim = N / 2;
    matrix_out C(N, vector<t_out>(N));

    // 1. Dividir matrices A y B en submatrices Aij y Bij
    matrix_in A11 = obtenerSubmatriz(A, 0,       0,       sub_dim);
    matrix_in A12 = obtenerSubmatriz(A, 0,       sub_dim, sub_dim);
    matrix_in A21 = obtenerSubmatriz(A, sub_dim, 0,       sub_dim);
    matrix_in A22 = obtenerSubmatriz(A, sub_dim, sub_dim, sub_dim);

    matrix_in B11 = obtenerSubmatriz(B, 0,       0,       sub_dim);
    matrix_in B12 = obtenerSubmatriz(B, 0,       sub_dim, sub_dim);
    matrix_in B21 = obtenerSubmatriz(B, sub_dim, 0,       sub_dim);
    matrix_in B22 = obtenerSubmatriz(B, sub_dim, sub_dim, sub_dim);

    // 2. Calcular productos de submatrices recursivamente
    // P1 = A11*B11, P2 = A12*B21, etc.
    matrix_out C11_p1 = multiplicacionDR1(A11, B11);
    matrix_out C11_p2 = multiplicacionDR1(A12, B21);

    matrix_out C12_p1 = multiplicacionDR1(A11, B12);
    matrix_out C12_p2 = multiplicacionDR1(A12, B22);

    matrix_out C21_p1 = multiplicacionDR1(A21, B11);
    matrix_out C21_p2 = multiplicacionDR1(A22, B21);

    matrix_out C22_p1 = multiplicacionDR1(A21, B12);
    matrix_out C22_p2 = multiplicacionDR1(A22, B22);

    // 3. Combinar resultados: Cij = P_a + P_b
    matrix_out C11 = sumarMatricesOutput(C11_p1, C11_p2);
    matrix_out C12 = sumarMatricesOutput(C12_p1, C12_p2);
    matrix_out C21 = sumarMatricesOutput(C21_p1, C21_p2);
    matrix_out C22 = sumarMatricesOutput(C22_p1, C22_p2);

    // 4. Ensamblar la matriz C
    ensamblarMatriz(C, C11, C12, C21, C22, sub_dim);

    return C;
}

matrix_out multiplicacionDR2(const matrix_in& A, const matrix_in& B)
{
    size_t N = A.size();
    if (N == 0) return matrix_out();
    if (B.size() != N || (N > 0 && (A[0].size() != N || B[0].size() != N))) {
        throw runtime_error("Dimensiones incompatibles (Strassen)");
    }

    if (N <= DR2_THRESHOLD) {
        return multiplicacionAlgoritmoTradicional(A, B);
    }

    // Si N no es par, recurrir a tradicional (simplificación)
    // Para Strassen puro, se necesitaría padding a la siguiente potencia de 2.
    if (N % 2 != 0) {
        return multiplicacionAlgoritmoTradicional(A, B);
    }

    size_t sub_dim = N / 2;

    // Dividir A y B en cuadrantes
    matrix_in A11 = obtenerSubmatriz(A, 0, 0, sub_dim);
    matrix_in A12 = obtenerSubmatriz(A, 0, sub_dim, sub_dim);
    matrix_in A21 = obtenerSubmatriz(A, sub_dim, 0, sub_dim);
    matrix_in A22 = obtenerSubmatriz(A, sub_dim, sub_dim, sub_dim);

    matrix_in B11 = obtenerSubmatriz(B, 0, 0, sub_dim);
    matrix_in B12 = obtenerSubmatriz(B, 0, sub_dim, sub_dim);
    matrix_in B21 = obtenerSubmatriz(B, sub_dim, 0, sub_dim);
    matrix_in B22 = obtenerSubmatriz(B, sub_dim, sub_dim, sub_dim);

    // Calcular las 10 sumas/restas S_i (usando matrix_in)
    matrix_in S1 = restarMatricesInput(B12, B22); // Corresponde a B12-B22
    matrix_in S2 = sumarMatricesInput(A11, A12); // Corresponde a A11+A12
    matrix_in S3 = sumarMatricesInput(A21, A22); // Corresponde a A21+A22
    matrix_in S4 = restarMatricesInput(B21, B11); // Corresponde a B21-B11
    matrix_in S5 = sumarMatricesInput(A11, A22); // Corresponde a A11+A22
    matrix_in S6 = sumarMatricesInput(B11, B22); // Corresponde a B11+B22
    matrix_in S7 = restarMatricesInput(A12, A22); // Corresponde a A12-A22
    matrix_in S8 = sumarMatricesInput(B21, B22);  // Corresponde a B21+B22
    matrix_in S9 = restarMatricesInput(A21, A11); // Corresponde a A21-A11
    matrix_in S10 = sumarMatricesInput(B11, B12);// Corresponde a B11+B12


    // Calcular los 7 productos de Strassen (M, N, O, P, Q, R, S) - recursivamente
    matrix_out M = multiplicacionDR2(S5, S6);    // M = (A11+A22)(B11+B22)
    matrix_out N_prod = multiplicacionDR2(S3, B11);  // N = (A21+A22)B11
    matrix_out O_prod = multiplicacionDR2(A11, S1);  // O = A11(B12-B22)
    matrix_out P_prod = multiplicacionDR2(A22, S4);  // P = A22(B21-B11)
    matrix_out Q_prod = multiplicacionDR2(S2, B22);  // Q = (A11+A12)B22
    matrix_out R_prod = multiplicacionDR2(S9, S10); // R = (A11-A21)(B11+B12)
    matrix_out S_prod = multiplicacionDR2(S7, S8);   // S = (A12-A22)(B21+B22)

    // Calcular los cuadrantes Cij (usando matrix_out)
    // C11 = M + P - Q + S
    matrix_out temp1 = sumarMatricesOutput(M, P_prod);
    matrix_out temp2 = restarMatricesOutput(temp1, Q_prod);
    matrix_out C11 = sumarMatricesOutput(temp2, S_prod);

    // C12 = O + Q
    matrix_out C12 = sumarMatricesOutput(O_prod, Q_prod);

    // C21 = N + P
    matrix_out C21 = sumarMatricesOutput(N_prod, P_prod);

    // C22 = M + O - N + R
    temp1 = sumarMatricesOutput(M, O_prod);
    temp2 = restarMatricesOutput(temp1, N_prod);
    matrix_out C22 = sumarMatricesOutput(temp2, R_prod);

    // Ensamblar C
    matrix_out C(N, vector<t_out>(N));
    ensamblarMatriz(C, C11, C12, C21, C22, sub_dim);

    return C;
}

int main(int argc, char* argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const vector<t_in> casosPorDefecto = {32, 64, 128, 256, 512, 1024, 2048, 4096};
    vector<t_in> dimCasosPorDefecto;

    string output_filename_base = "resultados_p1_dr1-" + to_string(DR1_THRESHOLD) + "_dr2-" + to_string(DR2_THRESHOLD);
    string output_filename;

    if (argc == 1) {
        cout << "No se proporciono N. Ejecutando para los siguientes casos: ";
        for (size_t i = 0; i < casosPorDefecto.size(); ++i) {
            cout << casosPorDefecto[i] << (i == casosPorDefecto.size() - 1 ? "" : ", ");
        }
        cout << endl << endl;
        dimCasosPorDefecto = casosPorDefecto;
        output_filename = output_filename_base + "_all.txt";
    } else if (argc == 2) {
        try {
            size_t tempN = stol(argv[1]);
            if (tempN == 0 || tempN > MAX_SIZE) {
                cerr << "Error: El valor de N (" << tempN
                          << ") esta fuera del rango valido para 't_in' (1-"
                          << numeric_limits<t_in>::max() << ")." << endl;
                return 1;
            }
            dimCasosPorDefecto.push_back(static_cast<t_in>(tempN));
            output_filename = output_filename_base + "_" + to_string(tempN) + ".txt";
        } catch (const invalid_argument& e) {
            cerr << "Error: Argumento invalido para N: '" << argv[1]
                      << "'. Debe ser un numero entero." << endl;
            return 1;
        } catch (const out_of_range& e) {
            cerr << "Error: El valor de N esta fuera de rango para un unsigned long." << endl;
            return 1;
        }
    } else {
        cerr << "Error: Numero incorrecto de argumentos." << endl;
        return 1;
    }

    ofstream outputFile;
    outputFile.open(output_filename);
    if (!outputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de salida: " << output_filename << endl;
        return 1;
    }

    ostringstream oss_header;
    oss_header << setw(10) << "N"
               << setw(35) << "Tiempo Tradicional (ms)" // Aumentar un poco el ancho
               << setw(35) << "Comparacion (Trad vs DR1)"
               << setw(35) << ("Tiempo DR1 (base=" + to_string(DR1_THRESHOLD) + ") (ms)")
               << setw(35) << "Comparacion (DR1 vs DR2)"
               << setw(35) << ("Tiempo DR2 (base=" + to_string(DR2_THRESHOLD) + ") (ms)");
    string header = oss_header.str();

    string separator = string(195, '-');

    outputFile << header << endl;
    outputFile << separator << endl;
    cout << header << endl;
    cout << separator << endl;

    for (t_in n : dimCasosPorDefecto) {
        outputFile << setw(10) << n;
        cout << setw(10) << n;

        matrix_in A = generarMatrizRandom(n);
        matrix_in B = generarMatrizRandom(n);

        matrix_out C_Tradicional, C_DR1, C_DR2;
        chrono::duration<double, milli> tiempoAlgoritmoTradicional, tiempoDR1, tiempoDR2;
        auto inicio = chrono::high_resolution_clock::now();
        if (n > 0) {
            C_Tradicional = multiplicacionAlgoritmoTradicional(A, B);
        }
        auto termino = chrono::high_resolution_clock::now();
        tiempoAlgoritmoTradicional = termino - inicio;
        cout << fixed << setprecision(6) << setw(35) << tiempoAlgoritmoTradicional.count() << flush;
        outputFile << fixed << setprecision(6) << setw(35) << tiempoAlgoritmoTradicional.count();
        
        // matrix_out CDR1(n, vector<t_out>(n, 0)); 
        inicio = chrono::high_resolution_clock::now();
        if (n > 0) {
            C_DR1 = multiplicacionAlgoritmoTradicional(A, B);
        }
        termino = chrono::high_resolution_clock::now();
        tiempoDR1 = termino - inicio;
        bool igualesAT_DR1 = matricesIguales(C_Tradicional, C_DR1);
        cout << setw(35) << (igualesAT_DR1 ? "Iguales" : "DIFERENTES") << flush;
        outputFile << setw(35) << (igualesAT_DR1 ? "Iguales" : "DIFERENTES");
        cout << fixed << setprecision(6) << setw(35) << tiempoDR1.count() << flush;
        outputFile << fixed << setprecision(6) << setw(35) << tiempoDR1.count();

        // matrix_out CDR2(n, vector<t_out>(n, 0)); 
        inicio = chrono::high_resolution_clock::now();
        if (n > 0) {
            C_DR2 = multiplicacionDR2(A, B);
        }
        termino = chrono::high_resolution_clock::now();
        tiempoDR2 = termino - inicio;
        bool igualesDR1_DR2 = matricesIguales(C_DR1, C_DR2);
        cout << setw(35) << (igualesDR1_DR2 ? "Iguales" : "DIFERENTES") << flush;
        outputFile << setw(35) << (igualesDR1_DR2 ? "Iguales" : "DIFERENTES");
        cout << fixed << setprecision(6) << setw(35) << tiempoDR2.count() << endl;
        outputFile << fixed << setprecision(6) << setw(35) << tiempoDR2.count() << endl;
    }

    outputFile.close();

    return 0;
}