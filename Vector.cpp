
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
    distancias = new MSvector();
};

Vector::Vector(const Vector& orig){

}


Vector::~Vector(){
	//delete distancias;
};

double Vector::metrica( Objeto* otro ){
	
	if( this->getClass() != otro->getClass() ) return - DBL_MAX;

	double dist = 0;
    vector<double>::iterator i = this->valores.begin();
    vector<double>::iterator j = ((Vector*)otro)->valores.begin();
    int m, n;
    m = 0;
    n = 0;
    for( ; i != this->valores.end() && j != ((Vector*)otro)->valores.end() ; ++i, ++j ){
        dist += pow( *i - *j, 2 );
    }
    //for( ; m < this->valores.size() && n < ob->valores.size(); m++, n++)
    dist = sqrt(dist);
    return dist;
};

void Vector::poneValor( double val ){
	this->valores.push_back( val );
};

string Vector::getClass() {
	return "Vector";
};
