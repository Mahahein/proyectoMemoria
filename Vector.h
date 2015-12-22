
#ifndef VECTOR_H
#define	VECTOR_H

#include "Objeto.h"
#include "MSvector.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;


class Vector : public Objeto{
	public:
		double* valores;
		int sizeValores;
		int capValores;
		double metrica(Objeto* otro);
		void poneValor(double d);
		string getClass();
		void eliminaValores();
		Vector();
		Vector(const Vector& orig);
		Vector(int cantPivs, int dim);
		virtual ~Vector();
	private:
};

#endif /* VECTOR_H */
