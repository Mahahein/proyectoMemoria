
#include "Objeto.h"
#include "String.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <float.h>
#include <stdlib.h>

using namespace std;

String::String(){
	distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    distancias = NULL;
    valores = NULL;
    tipo = 2;
    sizeDistancias = 0;
    capDistancias = 0;
};

String::String(int cantPivs, int dim) : Objeto(cantPivs, dim){
	distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    tipo = 2;
    valores = (new char[dim]);
}

String::String(const String& orig){

}

String::~String(){
	if(valores)
		delete[] valores;
	//delete distancias;
};

double String::metrica( Objeto* otro ){

	if( this->tipo != otro->tipo ) return - DBL_MAX;

	int i, j;
	int *T(new int[((String*)otro)->tamReal+1]);
	for( i = 0; i <= ((String*)otro)->tamReal; i++ ) T[i] = i;

	for( i = 0; i < this->tamReal; i++ ){
		T[0] = i+1;
		int corner = i;
		for( j = 0; j < ((String*)otro)->tamReal; j++ ){
			int upper = T[j+1];
			if( this->valores[i] ==  ((String*)otro)->valores[j] )
				T[j+1] = corner;
			else
				T[j+1] = min( T[j], min(upper, corner)) + 1;
			corner = upper;
		}
	}
	double val = (double)T[((String*)otro)->tamReal];
	//vector<int>().swap(T);
	//T.resize(0);
	//T.shrink_to_fit();
	delete[] T;
	return val;
};

string String::getClass(){
	return "String";
};

void String::setTamReal( int tam ){
	this->tamReal = tam;
};

void String::poneValor(double val){

}

void String::eliminaValores(){
	if(valores)
		delete valores;
}