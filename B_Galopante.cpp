#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
using namespace std;

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

int main()
{
    int tam = 10000;//tamaño del arreglo
    short Arr[tam]; //= {1,2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59};
    unsigned t0, t1;
    short val;
    for (int i = 0; i < tam; i++)
    {
        val = short(rand() % ((i + 1) * 3));//llenado de randoms en orden
        if (val > Arr[i - 1] || i == 0)
        {
            Arr[i] = val;
            // cout<<val<<" ";
        }
        else
        {
            i--;
        }
    }
    cout << endl;
    int valR = int(rand() % tam);
    cout << "posicion: " << valR << endl;
    valR = Arr[valR];
    cout << "valor: " << valR << endl;
    t0 = clock();
    int res = bGalopante(Arr, tam, valR);
    t1 = clock();
    double tiempo = double((t1 - t0)  / CLOCKS_PER_SEC);
    if (res == -1)
    {
        cout << "Valor no encontrado, tiempo: " << tiempo << endl;
    }
    else
    {
        cout << "El valor " << valR << " fue encontrado en la posicion:" << res << " tiempo: " << tiempo << " ms" << endl;
    }
    return 0;
}