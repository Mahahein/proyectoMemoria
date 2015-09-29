/* 
 * File:   Nodo.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 03:21 PM
 */

#ifndef NODO_H
#define	NODO_H
#include "Bloque.h"

class Nodo {
public:
    Bloque *block;
    Nodo *hizq;
    Nodo *hder;
    Nodo();
    Nodo(const Nodo& orig);
    Nodo(Bloque &bloq);
    virtual ~Nodo();
private:

};

#endif	/* NODO_H */

