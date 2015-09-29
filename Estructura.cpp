
#include "Estructura.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>



Estructura::Estructura(){

}

Estructura::Estructura( const Estructura& orig ){

}

bool Estructura::cargarEstructura(string nombreArchivo, Consultor& consultor){
	/*ESTRUCTURA DEL ARCHIVO
	dimension cantObjetosEnBolsa cantPivotesProvisorios cantCentros
	lineas de objetos en bolsa
	 - id + valores + distancia a pivotes (con eso obtengo la distancia acumulada)
	lineas de id de pivotes provisorios
	lineas de centros : idCentro + valores centro + radios de cobertura + numero de cluster
	*/

	if( verificaArchivo() ){
		FILE* archivo = fopen("structure.txt", "r");
		int dim, nObjBolsa, pivotes, centros;
		fscanf( archivo, "%d %d %d %d", &dim, &nObjBolsa, &pivotes, &centros );
		int i, j, k, id, nCluster;
		double val, d, dAcum, r;
		//cargando objetos
		consultor->saco.resize(0);
		for( i = 0; i < nObjBolsa; i++){
			Objeto* ob = new Objeto();
			ob->valores.resize(0);
			ob->distancias.resize(0);
			fscanf( archivo, "%d", &id );
			ob->id = id;
			//leer valores
			for( j = 0; j < dim; j++){
				fscanf( archivo, "%f", &val);
				ob->valores.push_back( val );
			}
			//leer distancias a pivotes
			dAcum = 0;
			for( j = 0; j < pivotes; j++ ){
				fscanf( archivo, "%f", &d );
				ob->distancias.push_back( d );
				dAcum += d;
			}
			ob->distanciaAcumulada = dAcum;
			ob->pos = i;
			ob->id
		}
		//cargando pivotes
		consultor->pivotes.resize(0);
		for( i = 0; i < pivotes; i++ ){
			fscanf( archivo, " %d", &id );
			Pivote* piv = new Pivote();
			piv->centro = buscaCentro( id, consultor->saco );
			piv->pos = i;
			obtieneCercanos( piv, consultor->saco );
			piv->actualizaRadio();
			consultor->pivotes.push_back( piv );
		}

		//cargando centros
		consultor->centros.resize(0);
		for( i = 0; i < centros; i++ ){
			fscanf( archivo, "%f %d", &r, &nCluster );
			Pivote* piv = new Pivote();
			piv->cercanos.resize(0);
			piv->radio = r;
			piv->numCluster = nCluster;
		}

	}
	return false;

}

void Estructura::guardarEstructura( int dimension, vector<Objeto*> bolsa, vector<Pivote*> provisorios, vector<Pivote*> centros){

}

bool Estructura::verificaArchivo(){
	struct stat buffer;   
    return (stat ("structure.txt", &buffer) == 0);
}

Objeto* Estructura::buscaCentro(int id, vector<Objeto*> objetos){
	for( int i = 0; i < objetos.size(); i++ ){
		if( objetos[i]->id == id ){
			return objetos[i];
		}
	}
	return NULL;
}

void Estructura::obtieneCercanos(Pivote* p, vector<Objeto*> objetos){
    p->cercanos.clear();
    int k = 0;
    for( vector<Objeto*>::iterator i = objetos.begin(); i != objetos.end(); ++i){
        //cout << "aqui" << endl;
        (*i)->comparando = p->pos;
        (*i)->pos = k;
        //cout << "aca" << endl;
        if( (*i)->id != p->centro->id ){
            //cout << "aculla" << endl;
            if(p->cercanos.size()<(n-1)){
                //cout << "metiendo a la mala" << endl;
                p->cercanos.push_back((*i));
                //cout << "actualizando el mas lejano" << endl;
                p->actualizaMasLejano();
                //cout << "sacando a la wena" << endl;
            }
            else{
                //cout << "aquino" << endl;
                if( (*i)->distancias[p->pos] < p->radio ){
                    p->cercanos.erase( p->posMasLejano );
                    p->cercanos.push_back((*i));
                    p->actualizaMasLejano();
                }
                //cout << "acalla" << endl;
            }
            //cout << "per cua" << endl;
        }
        k++;
    }
    //cout << "saliendo de buscar los cercanos" << endl;
}
