/* 
 * File:   Pivote.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 05:43 PM
 */

#include "Pivote.h"
#include "Objeto.h"
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>

Pivote::Pivote() {
    numCluster = -1;
    posArch = 0;
}

Pivote::Pivote(const Pivote& orig) {
    numCluster = -1;
    posArch = 0;
}

Pivote::~Pivote() {
}

Pivote::Pivote(Objeto* ob, double r){
    this->centro = ob;
    this->radio = r;
    numCluster = -1;
    posArch = 0;
}

Pivote::Pivote(Objeto* c){
    this->centro = c;
    numCluster = -1;
    posArch = 0;
}

bool Pivote::operator <(const Pivote& piv) const{
    return (numCluster < piv.numCluster);
}

bool Pivote::operator >(const Pivote& piv) const{
    return (numCluster > piv.numCluster);
}

void Pivote::actualizaRadio(){
    int mayor = 0;
    double dist = this->centro->metrica(this->cercanos[0]);
    double dist2;
    for( vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i ){
        dist2 = this->centro->metrica( (*i) );
    	if( dist2 > dist){
            dist = dist2;
        }
    }
    this->radio = dist;
}

void Pivote::actualizaRadioCentro(){
    int mayor = 0;
    double dist = -1;
    double dist2;
    for( vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i ){
        dist2 = (*i)->metrica(this->centro);
        (*i)->distanciaACentro = dist2;
        if( dist < dist2) dist = dist2;

    }
    this->radio = dist;
}

void Pivote::ordenaCercanos(int pos){
    for(vector<Objeto*>::iterator i = cercanos.begin(); i != cercanos.end(); ++i){
	   (*i)->comparando = pos;
    }
    //sort( cercanos.begin(), cercanos.end());
}

void Pivote::actualizaMasLejano(){
    int lejano = 0;
    int i;
    //cout << "EN PIVOTE.CPP ";
    double distMayCentro = 0;
    double dist2;
    for( i = 0; i < this->cercanos.size(); i++ ){
        //cout << "cercanos[i]->distancias.size(): " << cercanos[i]->distancias.size() << "  this->pos: " << this->pos << endl;
        dist2 = this->cercanos[i]->metrica(this->centro);
        if( dist2 >= distMayCentro ){
            lejano = i;
            distMayCentro = dist2;
        }
    }
    this->posMasLejano = lejano;
}

int Pivote::masCercanoACentro(){
    int cercano = 0;
    for( int i = 0; i < cercanos.size(); i++ ){
        if( cercanos[cercano]->distanciaACentro > cercanos[i]->distanciaACentro )
            cercano = i;
    }
    return cercano;
}