
#include "Objeto.h"
#include "String.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <float.h>

using namespace std;

String::String(){
	distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    distancias = new MSvector();
};

String::String(const String& orig){

}

String::~String(){
	delete valores;
};

double String::metrica( Objeto* otro ){

	if( this->getClass() != otro->getClass() ) return - DBL_MAX;

	int i, j;
	vector<int> T(((String*)otro)->tamReal+1);
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
	return (double)T[((String*)otro)->tamReal];
};

string String::getClass(){
	return "String";
};

void String::setTamReal( int tam ){
	this->tamReal = tam;
};

void String::poneValor(double val){

}