/* 
 * File:   Nodo.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 03:21 PM
 */

#include "Nodo.h"
#include "Bloque.h"

Nodo::Nodo() {
}

Nodo::Nodo(const Nodo& orig) {
}

Nodo::Nodo(Bloque& bloq){
    this->block = &bloq;
}

Nodo::~Nodo() {
}

