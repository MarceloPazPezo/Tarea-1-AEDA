#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;


int bBinaria(short A[], int linf, int size, int x)
{
    if(size==0){
        return -1;
    }
    if (A[linf + (size / 2)]==x)
    {
        return linf + (size / 2);
    }
    if (A[linf + (size / 2)]<x)
    {
        linf = linf + (size / 2);
        size = size / 2;
        return bBinaria(A, linf+1, size, x);
    }else{
        size = size / 2;
        return bBinaria(A, linf, size, x);
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
        return bBinaria(A, linf-1, size, x);
    }
}

int main()
{
    int tam = 10;
    short Arr[tam] = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23};
    int valR =5;
    int res = bGalopante(Arr, tam, valR);
    cout << "Valor encontrado en la posicion:" <<res;
    return 0;
}