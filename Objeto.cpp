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
    //distancias = new MSvector();
    sizeDistancias = 0;
    capDistancias = 0;
    distancias = NULL;
    //distancias.reserve(100);
}

Objeto::Objeto(const Objeto& orig) {
    distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    distancias = NULL;
    //distancias.resize(0);
}
Objeto::Objeto(int cantPivs, int dim){
    distanciaAcumulada = 0;
    esPivote = false;
    comparando = 0;
    pos = 0;
    posPiv = -1;
    distancias = (new double[cantPivs]);
    sizeDistancias = cantPivs;
    capDistancias = cantPivs;
    dimension = dim;
    //distancias.resize(0);
}

Objeto::~Objeto() {
    if(distancias)
        delete[] distancias;
}

void Objeto::poneValor( double val){
    //valores.push_back(val);
}

void Objeto::poneDistancia(double d){
    //distancias->putValue(d);
    if(sizeDistancias == 0){
        sizeDistancias = 1;
        capDistancias = 1;
        distancias = (new double[capDistancias]);
        distancias[0] = d;
        return;
    }
    if(sizeDistancias < capDistancias){
        distancias[sizeDistancias] = d;
    }
    else{
        capDistancias = sizeDistancias*2;
        double *dist = (new double[capDistancias]);

        for(int i = 0; i < sizeDistancias; i++){
            dist[i] = distancias[i];
        }
        delete[] distancias;
        distancias = dist;
        distancias[sizeDistancias] = d;
    }
    sizeDistancias+=1;
    return;
}

void Objeto::aumentaAcumulado(double k){
    distanciaAcumulada += k;
}

void Objeto::eliminaDistancia(int pos){
    if(pos > sizeDistancias) return;
    else{
        for(int i = pos; i < sizeDistancias-1; i++){
            distancias[i] = distancias[i+1];
        }
        distancias[sizeDistancias] = -1;
        sizeDistancias-=1;
    }
}

Objeto Objeto::newObjeto(){
    return Objeto();
}

double Objeto::metrica(Objeto* otro){
}

string Objeto::getClass(){
    return "";
}

void Objeto::eliminaValores(){
}