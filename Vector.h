
#ifndef VECTOR_H
#define	VECTOR_H

#include "Objeto.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;


class Vector : public Objeto{
	public:
		vector<double> valores;
		double metrica(Objeto* otro);
		void poneValor(double val);
		string getClass();
		Vector();
		Vector(const Vector& orig);
		virtual ~Vector();
	private:
};

#endif /* VECTOR_H */
