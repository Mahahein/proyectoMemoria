/* 
 * File:   Bloque.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 03:03 PM
 */

#ifndef BLOQUE_H
#define	BLOQUE_H
#include <vector>
#include "Objeto.h"
using namespace std;

class Bloque {
public:
    int id_bloque;
    int tamanio; //en alguna unidad
    int n_objetos; //cantidad de objetos en el bloque
    int radio; //radio del cluster.
    vector<Objeto> objetos;
    Objeto *centro;
    Bloque();
    Bloque(const Bloque& orig);
    Bloque(int id, int n_objs, int rad, int tam, Objeto& ob);
    virtual ~Bloque();
private:

};

#endif	/* BLOQUE_H */

