/* 
 * File:   Objeto.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 12:47 PM
 */

#include "Objeto.h"
#include "MSvector.h"
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;


Objeto::Objeto() {
    distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    distancias = new MSvector();
    //distancias.reserve(100);
}

Objeto::Objeto(const Objeto& orig) {
    distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    distancias = new MSvector();
    //distancias.resize(0);
}
Objeto::Objeto(int dist){
    distanciaAcumulada = dist;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    distancias = new MSvector();
    //distancias.resize(0);
}

Objeto::~Objeto() {
    delete distancias;
}

void Objeto::poneValor( double val){
    //valores.push_back(val);
}

void Objeto::poneDistancia(double d){
    distancias->putValue(d);
}

void Objeto::aumentaAcumulado(double k){
    distanciaAcumulada += k;
}

Objeto Objeto::newObjeto(){
    return Objeto();
}

double Objeto::metrica(Objeto* otro){
    /*double dist = 0;
    vector<double>::iterator i = this->valores.begin();
    vector<double>::iterator j = ob->valores.begin();
    int m, n;
    m = 0;
    n = 0;
    for( ; i != this->valores.end() && j != ob->valores.end() ; ++i, ++j ){
        dist += pow( *i - *j, 2 );
    }
    //for( ; m < this->valores.size() && n < ob->valores.size(); m++, n++)
    dist = sqrt(dist);
    return dist;*/
}

/*double Objeto::metrica( Vector* otro ){

}

double Objeto::metrica( String* otro ){
    
}*/

string Objeto::getClass(){
    return "";
}