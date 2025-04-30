#include <stdio.h>

int bBinaria(int A[],int linf,int size,int x){
    
    if(A[linf+(size/2)==x]){

        return linf+(size/2);

    }else if(A[linf+(size/2)<x]){

        linf=linf+(size/2);
        size=size/2;

        return bBinaria(A,linf,size,x);

    }else if(A[size+(linf/2)>x]){
        size=size/2;
        return bBinaria(A,linf,size,x);
    }
}

int bGalopante(int A[],int size,int x){

    if(size==0){
        return -1;
    }else{
        int linf=1;
        while(linf*2<size && A[linf*2]<x){
            linf=linf*2;
        }
        return bBinaria(A,linf,size,x);
    }
}

int main(){

}