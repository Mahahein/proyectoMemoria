
#ifndef STRING_H
#define	STRING_H

#include "Objeto.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;


class String : public Objeto{
	public:
		char *valores;
		int tamReal;
		double metrica(Objeto* otro);
		string getClass();
		void setTamReal( int tam );
		void poneValor(double val);
		void eliminaValores();
		String();
		String(const String& orig);
		String(int cantPivs, int dim);
		~String();
	private:

};

#endif /* STRING_H */