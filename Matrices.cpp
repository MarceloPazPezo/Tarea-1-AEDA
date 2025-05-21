#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <random>
#include <vector>
using namespace std;

void mTradicional(int **A, int **B, int **C, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            for (int k = 0; k < tam; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void DR1(int **A, int **B, int **C, int vsup, int vinf, int hsup, int hinf)
{   
    
}

void impMat(int **A, int tam)
{
    for (int i = 0; i < tam; ++i)
    {
        for (int j = 0; j < tam; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
        // cout << A[i] << " ";
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        cout << "ingrese la es eponente del tamaño de la matriz 1-10" << endl;
    }
    int tam = pow(2, atoi(argv[1]));
    // vector<vector<int>> A(tam, vector<int>(tam));
    // vector<vector<int>> B(tam, vector<int>(tam));
    // vector<vector<int>> C(tam, vector<int>(tam));
    /*int A[tam][tam];
    int B[tam][tam];
    int C[tam][tam];*/
    int **A = (int **)malloc(tam * sizeof(int));
    int **B = (int **)malloc(tam * sizeof(int));
    int **C = (int **)malloc(tam * sizeof(int));

    const int MIN_VAL = 1, MAX_VAL = 5;

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<> distrib(MIN_VAL, MAX_VAL);

    for (int i = 0; i < tam; ++i)
    {
        for (int j = 0; j < tam; j++)
        {
            A[i] = (int *)malloc(sizeof(int));
            B[i] = (int *)malloc(sizeof(int));
            C[i] = (int *)malloc(sizeof(int));
        }
        // cout << A[i] << " ";
    }

    for (int i = 0; i < tam; ++i)
    {
        for (int j = 0; j < tam; j++)
        {
            A[i][j] = distrib(gen);
            B[i][j] = distrib(gen);
            C[i][j] = 0;
        }
        // cout << A[i] << " ";
    }

    impMat(A, tam);
    impMat(B, tam);

    // mTradicional(A, B, C, tam);
    DR1(A, B, C, tam - 1, 0, tam - 1, 0);
    for (int i = 0; i < tam; ++i)
    {
        for (int j = 0; j < tam; j++)
        {
            cout << C[i][j] << " ";
        }
        cout << endl;
        // cout << A[i] << " ";
    }

    return 0;
}