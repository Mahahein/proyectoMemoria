
#ifndef LECTOR_H
#define LECTOR_H
#include <iostream>
#include "Objeto.h"
#include <vector>
#include <set>
#include <string>

using namespace std;

class Lector{
	public:
		Lector();
		Lector(int dimen);
		Lector(const Lector& orig);
		~Lector();
		vector<Objeto*> leerDesdeArchivo(string nombreArchivo);
		Objeto* leerDesdeEE();	
		int dimension;
		int tipo;
		void setDimension( int dim );
	private:

};

#endif /* LECTOR_H */