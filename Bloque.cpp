/* 
 * File:   Bloque.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 03:04 PM
 */

#include "Bloque.h"
#include "Objeto.h"

Bloque::Bloque() {
}

Bloque::Bloque(const Bloque& orig) {
}

Bloque::Bloque(int id, int n_objs, int rad, int tam, Objeto& ob){
    id_bloque = id;
    n_objetos = n_objs;
    radio = rad;
    tamanio = tam;
    this->centro = &ob;
}

Bloque::~Bloque() {
}

