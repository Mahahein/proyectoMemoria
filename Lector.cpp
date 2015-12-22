

#include <iostream>
#include "Lector.h"
#include "Objeto.h"
#include "Vector.h"
#include "String.h"
#include <vector>
#include <set>
#include <string>
#include <stdio.h>
#include <string.h>


using namespace std;

Lector::Lector(){

}

Lector::Lector(int dimen){
	this->dimension = dimen;
}

Lector::Lector(const Lector& orig){

}

Lector::~Lector(){

}

vector<Objeto*> Lector::leerDesdeArchivo(string nombreArchivo){
	//CONVENCION MIA :P
	//-- PRIMERA LINEA: NUMERO DE CONSULTAS
	//-- SEGUNDA LINEA EN ADELANTE: VECTOR DEL OBJETO - RADIO
	cout << "Leyendo consultas desde " << nombreArchivo << endl;
	vector<Objeto*> consultas;
	char linea[100];
	FILE* archivo = fopen(nombreArchivo.c_str(), "r");
	int ncon;
	fscanf(archivo, "%d", &ncon);
	cout << "num objs " << ncon << endl;
	int cont = 0;
	int cont2, i;
	double e, r;
	char basura[30];
	while( cont < ncon ){

		Objeto* o;
		if( tipo == 1){
			o = new Vector();
			cont2 = 0;
			while( cont2 <= this->dimension ){
				fscanf(archivo, "%lf", &e);
				if(cont2 == 0){
					o->id = e;
				}
				else o->poneValor(e);
				cont2++;
			}
			//PARA EL HISTOGRAMA
			//fscanf(archivo, "%s\n", basura);
		}
		else{
			o = new String();
			fscanf( archivo, "%s", linea);
			((String*)o)->valores = (char*)malloc(sizeof(char)*dimension);
			for(i = 0; i < strlen(linea); i++) ((String*)o)->valores[i] = linea[i];
			//strncpy(((String*)o)->valores, linea, sizeof(((String*)o)->valores));
			((String*)o)->valores[strlen(linea)] = 0;
			((String*)o)->tamReal = strlen(linea);
			((String*)o)->dimension = dimension;
		}

		//fscanf(archivo, "%lf", &r);
		//o->distanciaAcumulada = r;
		consultas.push_back(o);
		cont++;
	}
	cout << "lectura completa " << consultas.size() << endl;
	return consultas;
}

Objeto* Lector::leerDesdeEE(){
	Objeto* o = new Objeto();
	int cont = 0;
	double e;
	while( cont < this->dimension ){
		printf( "Ingrese elemento %d del vector: ", cont);
		scanf( "%lf", &e );
		cont++;
		o->poneValor(e);
	}
	printf( "Ingrese el radio de consulta: ");
	scanf( "%lf", &e );
	while( e < 0 ){
		printf("Radio no válido. Ingrese el radio de consulta:\n");
		scanf( "%lf", &e );
	}
	o->distanciaAcumulada = e;
	return o;
}

void Lector::setDimension( int dim ){
	this->dimension = dim;
}