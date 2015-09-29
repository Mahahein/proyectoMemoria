/* 
 * File:   Objeto.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 12:47 PM
 */

#ifndef OBJETO_H
#define	OBJETO_H
#include <vector>
#include <iostream>
#include <list>
#include <string>
#include "MSvector.h"
using namespace std;

class Objeto {
public:
    //vector<T> valores;
    MSvector* distancias;
    double distanciaAcumulada;
    double distanciaACentro;
    int id;
    int comparando;
    int pos;
    bool esPivote;
    int posPiv;
    int largoMax;
    Objeto();
    Objeto(const Objeto& orig);
    Objeto(int peso);
    virtual ~Objeto();
    virtual void poneValor( double val);
    void poneDistancia(double d);
    void aumentaAcumulado(double k);
    virtual double metrica(Objeto* ob);
    //virtual double metrica(Vector* ob);
    //virtual double metrica(String* ob);
    virtual string getClass();
    Objeto newObjeto();
private:

};

#endif	/* OBJETO_H */

