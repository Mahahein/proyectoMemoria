

#ifndef ESTRUCTURA_H
#define	ESTRUCTURA_H
#include <vector>
#include <iostream>
#include <string>
#include "Objeto.h"
#include "Pivote.h"

using namespace std;


class Estructura {

public:
	Estructura();
	Estructura(const Estructura& orig);
	bool cargarEstructura(string nombreArchivo, Consultor& consultor);
	void guardarEstructura( int dimension, vector<Objeto*> bolsa, vector<Pivote*> provisorios, vector<Pivote*> centros);
	bool verificaArchivo();
	Objeto* buscaCentro(int id, vector<Objeto*> objetos);
	void obtieneCercanos(Pivote* piv, vector<Objeto*> objetos);
};




#endif	/* ESTRUCTURA_H */