
#include <iostream>
#include <string>
#include <math.h>
#include <float.h>
#include "Vector.h"
#include "Objeto.h"
#include "MSvector.h"

using namespace std;

Vector::Vector(){
	distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    distancias = NULL;
    valores = NULL;
    tipo = 1;
    sizeDistancias = 0;
    capDistancias = 0;
    sizeValores = 0;
    capValores = 0;
};

Vector::Vector(const Vector& orig){

}

Vector::Vector(int cantPivs, int dim) : Objeto(cantPivs, dim){
    distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    //distancias = NULL;
    valores = (new double[dim]);
    tipo = 1;
}


Vector::~Vector(){
	//delete distancias;
    if(valores)
        delete[] valores;
};

double Vector::metrica( Objeto* otro ){
	
	if( this->tipo != otro->tipo ) return - DBL_MAX;

	double dist = 0;
    int m, n;
    m = 0;
    n = 0;
    //cout << "en origen: " << this->valores->getValueAt(0) << " en otro:" << ((Vector*)otro)->valores->getValueAt(0) << endl;
    for( ; m < this->sizeValores && n < ((Vector*)otro)->sizeValores; m++, n++){
        dist += pow( this->valores[m] - ((Vector*)otro)->valores[n] , 2 );
    }
    dist = sqrt(dist);
    return dist;
};

void Vector::poneValor( double d ){
	//this->valores->putValue( val );
    if(sizeValores == 0){
        sizeValores = 1;
        capValores = 1;
        valores = (new double[capValores]);
        valores[0] = d;
        return;
    }
    if(sizeValores < capValores){
        valores[sizeValores] = d;
    }
    else{
        capValores = sizeValores*2;
        double *dist = (new double[capValores]);

        for(int i = 0; i < sizeValores; i++){
            dist[i] = valores[i];
        }
        delete[] valores;
        valores = dist;
        valores[sizeValores] = d;
    }
    sizeValores += 1;
    return;
};

string Vector::getClass() {
	return "Vector";
};

void Vector::eliminaValores(){
    delete valores;
}
