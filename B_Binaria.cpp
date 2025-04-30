#include <stdio.h>

int bBinaria(int Arr[],int indice,int valor ){
    if(Arr[indice]==valor){
        return indice;
    }
    int aux[indice];
    if(valor<Arr[indice]){
        for( int i=0;i<indice;i++){
            aux[i]=Arr[i];
        }
    }
    if(valor>Arr[indice]){
        for( int i=0;i<=indice;i++){
            aux[i]=Arr[indice+i];
        }
    }
    return bBinaria(aux,(indice/2),valor);

}

int main(){
    int n;
    int arr[n];
    int valor;
    bBinaria(arr,n/2,valor);
}
