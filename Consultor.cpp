
#include "Consultor.h"
#include "Objeto.h"
#include "Pivote.h"
#include "Vector.h"
#include "String.h"
#include <vector>
#include <set>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <algorithm>

using namespace std;

Consultor::Consultor(){
	this->archivo = fopen("clusters.bin", "r+b" );
	lecturas = 0; 
	escrituras = 0;
	movCabezal = 0;
	calcMetrica = 0;
	cantPags = 1;
};

Consultor::Consultor(int pages){
	this->archivo = fopen("clusters.bin", "r+b" );
	lecturas = 0; 
	escrituras = 0;
	movCabezal = 0;
	calcMetrica = 0;
	cantPags = pages;
};

Consultor::Consultor(const Consultor& orig){
	this->archivo = fopen("clusters.bin", "r+b" );
};

Consultor::~Consultor(){
	fclose(this->archivo);
};

vector<Objeto*> Consultor::consultarBusquedaRango(Objeto* ob, double r){
	vector<Objeto*> cercanos;
	vector<Pivote*> candidatos;
	vector<Pivote*>::iterator l;
	vector<Objeto*>::iterator m;
	int i, j, k;
	double dist;
	bool esta;

	//BUSCANDO EN LOS CLUSTER
	//cout << "Buscando clusters con posibles vecinos" << endl;
	for( l = clusters.begin(); l != clusters.end(); ++l ){
		calcMetrica++;
		if( ob->metrica((*l)->centro) <= (r + (*l)->radio )){
			candidatos.push_back((*l));
		}
	}

	//cout << "sacando los posibles vecinos de los clusters" << endl;
	if(!candidatos.empty()){

		for( l = candidatos.begin(); l != candidatos.end(); ++l ){

			vector<Objeto*> objetosBloque = extraerCluster( (*l)->numCluster, (*l)->posArch );
			lecturas++;

			for( m = objetosBloque.begin(); m != objetosBloque.end(); ++m ){
				calcMetrica++;
				if( ob->metrica((*m)) <= r ){
					cercanos.push_back((*m));
				}
				else{
					delete (*m);
				}
			}
			/*for( m = objetosBloque.begin(); m != objetosBloque.end(); ++m ){
				delete (*m);
			}*/
			objetosBloque.clear();
		}
	}

	//cout << "buscando en los pivotes en ram, calculando distancias a los pivotes" << endl;


	vector<double> distPivs;
	for( l = pivotes.begin(); l != pivotes.end(); ++l ){
		calcMetrica++;
		distPivs.push_back( ob->metrica((*l)->centro) );
	}

	//ELIGIENDO POR DESIGUALDAD TRIANGULAR
	//cout << "buscando por distancia triangular y luego directo a los posibles vecinos" << endl;
	for( m = saco.begin(); m != saco.end(); ++m ){
		for( j = 0; j < (*m)->distancias->getSize(); j++ ){
			if( fabs( distPivs[j] - (*m)->distancias->getValueAt(j) ) <= r ){ // DESIGUALDAD TRIANGULAR
				calcMetrica++;
				if( ob->metrica((*m)) <= r){
					cercanos.push_back( (*m) );
					break;
				}

			}
		}
	}
	//cout << "total encontrados " << cercanos.size() << endl;
	
	return cercanos;

};

bool Consultor::eliminarObjeto(Objeto* ob){
	bool eliminado = false;
	bool igual;

	vector<Pivote*>::iterator k;
	vector<Objeto*>::iterator i;
	vector<Objeto*>::iterator m;

	vector<Objeto*> bloque;
	bloque.resize(0);
	bloque.shrink_to_fit();

	double dist;
	int j;
	//Buscando en RAM
	//cout << "Buscando el objeto en ram: ";
	if(tipo == 2) printf("%s", ((String*)ob)->valores);
	cout << endl;

	for( i = saco.begin(); i != saco.end() && !eliminado; ++i ){
		//if(tipo == 2) printf("comparando con %s\n", ((String*)(*i))->valores);
		dist = ob->metrica( (*i) );
		calcMetrica++;
		if( dist == 0 ){
			//cout << "encontrado en RAM" << endl;
			if( (*i)->esPivote ){
				//cout <<"es pivote" << endl;
				eliminaPivote( (*i)->id );
				elegirPivote();
			}
			//cout << "Borrando del saco" << endl;
			saco.erase(saco.begin()+(*i)->pos);
			//cout << "Borrando objeto" << endl;
			//delete (*i);
			//cout << "Ajustando posiciones" << endl;
			ajustaPosicionesSaco();
			//cout <<"nuevo tamaño: " << saco.size() << endl;
			eliminado = true;
			return eliminado;
		}
	}

	//BUSCAR EN CLUSTERS
	//cout << "Buscando el objeto en los clusters:::" << clusters.size() << endl;
	//for( k = clusters.begin(); k != clusters.end(); ++k ){
	int l;
	for( l = 0; l < clusters.size(); l++){
		calcMetrica++;
		if( ob->metrica(clusters[l]->centro) <= clusters[l]->radio ){
			//cout << "esta en el rango, extrayendo cluster" << endl;
			bloque = extraerCluster( clusters[l]->numCluster,  clusters[l]->posArch);
			lecturas++;
			//for(i = bloque.begin(); i != bloque.end(); ++i ){
			//cout << "cluster extraido" << endl;
			clusters[l]->cercanos.clear();
			for( int w = 0; w < bloque.size(); w++){
				clusters[l]->cercanos.push_back(bloque[w]);
			}
			
			//cout << "comienza a recorrer cluster" << endl;
			for( j = 0; j < clusters[l]->cercanos.size(); j++){
				//cout << "-> objeto " << j << " del cluster" << endl;
				calcMetrica++;
				if( ob->metrica( clusters[l]->cercanos[j] ) == 0 ){
					//cout << "objeto encontrado en cluster " << l << endl;
					
					/*for(int t = 0; t < clusters[l]->cercanos[j]->valores.size(); t++)
						cout << clusters[l]->cercanos[j]->valores[t] << " ";
					cout << endl;
					*/

					if( j == 0 ){
						//cout << "ES EL CENTROOOOO!!!! AAAAAHHHHH!!!" << endl;
						
						clusters[l]->actualizaRadioCentro();
						//busco el mas cercano al centro para reemplazarlo
						int nj = clusters[l]->masCercanoACentro();
						//Objeto* centroNuevo = *(nj);
						//cambio el centro
						clusters[l]->centro = clusters[l]->cercanos[nj];//centroNuevo;
						clusters[l]->cercanos[j] = clusters[l]->cercanos[nj];
						//elimino el nuevo de la lista de cercanos
						//clusters[l]->cercanos.erase( clusters[l]->cercanos.begin()+nj );
						//cout << "->elemento es centro" << endl;
						j = nj;
						
					}
					//cout << "BUSCA UNO EN LOS DE LA RAM" << endl;
					Objeto* nuevo = cercanoA( clusters[l]->centro );
					if( nuevo->esPivote ){
						//cout << "Nuevo es pivote" << endl;
						eliminaPivote( nuevo->id );
						//AGREGAR CONDICION DE TAMAÑO DEL SACO
						if( (saco.size()-1)%tamCluster == 1 )
							elegirPivote();
					}
					//cout << "Borra el encontrado" << endl;
					delete ( clusters[l]->cercanos[j] );
					clusters[l]->cercanos.erase( clusters[l]->cercanos.begin() + j );
					
					//cout << "Agrega el nuevo " << nuevo->id << " " << nuevo->pos << endl;
					clusters[l]->cercanos.push_back(nuevo);
					//cout << "actualiza radio: " <<  clusters[l]->cercanos.size() << endl;
					clusters[l]->actualizaRadio();
					//cout << "quita nuevo del saco" << endl;
					saco.erase( saco.begin() + nuevo->pos );
					//cout << "ajustando posiciones" << endl;
					ajustaPosicionesSaco();
				
					//cout << "reescribiendo cluster" << endl;
					reescribeCluster(clusters[l]);
					escrituras++;
					eliminado = true;
					//cout << "borrando elementos" << endl;
					//BORRANDO ELEMENTOS
					for( m = clusters[l]->cercanos.begin(); m != clusters[l]->cercanos.end(); ++m ){
						delete (*m);
					}
					clusters[l]->cercanos.resize(0);
					clusters[l]->cercanos.shrink_to_fit();
					return eliminado;
				}
			}
			//BORRANDO ELEMENTOS
			for( i = bloque.begin(); i != bloque.end(); ++i ){
				delete (*i);
			}
			bloque.resize(0);
			bloque.shrink_to_fit();
		}

	}
	return eliminado;

};

vector<double> Consultor::distanciaACentros(Objeto* ob){
	vector<double> distancias;
	double dist;
	for( vector<Pivote*>::iterator i = clusters.begin(); i != clusters.end(); ++i ){
		calcMetrica++;
		distancias.push_back(ob->metrica((*i)->centro));
	}

	return distancias;

};

vector<double> Consultor::distanciaAPivotes(Objeto* ob){
	vector<double> distancias;
	double dist;
	for( vector<Pivote*>::iterator i = pivotes.begin(); i != pivotes.end(); ++i ){
		calcMetrica++;
		distancias.push_back(ob->metrica((*i)->centro));
	}

	return distancias;
};

vector<Objeto*> Consultor::extraerCluster(int numCluster, long position){
	vector<Objeto*> objetos;
	int j, k, objs;
	
	if(tipo == 1){
		if( (4096*cantPags)*numCluster != ftell(archivo) ) movCabezal++;
		fseek( archivo, (4096*cantPags)*numCluster, SEEK_SET );
		double block[512];
		fread( block, sizeof(double), 512, archivo );
		for( j = 0; j < (dimension*tamCluster); j+=dimension ){
			Vector* objMem = new Vector();
			for( k = 0; k < dimension; k++ ){
				//cout << block[j+k] << " ";
				objMem->poneValor( block[j+k] );
			}
			//cout << endl;
			objetos.push_back(objMem);
		}
	}
	else{
		if(position != ftell(archivo) ) movCabezal++;
		fseek( archivo, position, SEEK_SET );
		char linea[dimension];
		for(k = 0; k < tamCluster; k++){
			fscanf( archivo, "%s", linea);
			//printf("Extraido: %s\n", linea);
			String* objMem = new String();
			objMem->valores = (char*)malloc(sizeof(char)*dimension);
			for(j = 0; j < strlen(linea); j++){
				objMem->valores[j] = linea[j];
			}
			objMem->valores[strlen(linea)] = 0;
			objMem->tamReal = strlen(linea);
			objMem->largoMax = dimension;
			objetos.push_back(objMem);
		}			
	}

	return objetos;
};

void Consultor::eliminaPivote(int id){
    int k = 0;
    for( vector<Pivote*>::iterator i = pivotes.begin(); i != pivotes.end(); ++i ){
        if( (*i)->centro->id == id ){
            pivotes.erase( i );
            ajustaPosicionesPivotes(k);
            return;
        }
        k++;
    }
};

void Consultor::elegirPivote(){

    vector<Objeto*>::iterator mayObj;
    vector<Objeto*>::iterator i;
    double disAc = -DBL_MAX;
    for( i = saco.begin(); i != saco.end(); ++i ){
        if( (*i)->distanciaAcumulada >= disAc && !(*i)->esPivote ){
            mayObj = i;
            disAc = (*i)->distanciaAcumulada;
        }
    }

    Pivote* piv = new Pivote();
    piv->centro = (*mayObj);
    piv->centro->id = (*mayObj)->id;
    piv->pos = pivotes.size();
    (*mayObj)->esPivote = true;
    pivotes.push_back(piv);
    
    double distancia;
    for( i = saco.begin(); i != saco.end(); ++i ){
        distancia = (*i)->metrica(piv->centro);
        calcMetrica++;
        (*i)->poneDistancia(distancia);
        (*i)->aumentaAcumulado(distancia);
    }

};

void Consultor::ajustaPosicionesPivotes(int posElim){
    int k;
    for( vector<Objeto*>::iterator i = saco.begin(); i != saco.end(); ++i ){
        (*i)->distancias->deleteValue( posElim );
    }
    k = 0;
    for( vector<Pivote*>::iterator i = pivotes.begin(); i != pivotes.end(); ++i  ){
        (*i)->pos = k;
        k++;
    }
};

void Consultor::ajustaPosicionesSaco(){
    int k = 0;
    for(vector<Objeto*>::iterator i = saco.begin(); i != saco.end(); ++i ){
        (*i)->pos = k;
        k++;
    }
};


Objeto* Consultor::cercanoA(Objeto* centro){
	double dist, distMin;
	distMin = DBL_MAX;
	Objeto* cercano;
	for( vector<Objeto*>::iterator i = saco.begin(); i != saco.end(); ++i ){
		calcMetrica++;
		if( distMin > (*i)->metrica(centro) ){
			distMin = dist;
			cercano = (*i);
		}
	}

	return cercano;
};

void Consultor::reescribeCluster(Pivote* centro){
	vector<Objeto*>::iterator i;
	if (tipo == 1){
		if( (4096*cantPags)*centro->numCluster != ftell(archivo) ) movCabezal++;
		fseek( archivo, (4096*cantPags)*centro->numCluster, SEEK_SET );
	}
	else{
		if( centro->posArch != ftell(archivo) ) movCabezal++;
		fseek( archivo, centro->posArch, SEEK_SET );
	}
    //cout << "escribiendo" << endl;
    int count = 0;
    int ini;
    for( i = centro->cercanos.begin(); i != centro->cercanos.end(); ++i ){
    	if(tipo == 1){
	        const char* pointer = reinterpret_cast<const char*>(&((Vector*)(*i))->valores[0]);
	        fwrite( pointer, sizeof(double), ((Vector*)(*i))->valores.size(), archivo );
	        count += sizeof(double) * ((Vector*)(*i))->valores.size();
    	}
    	else{
    		for(ini = 0; ini < ((String*)(*i))->tamReal; ini++ )
    			fputc(((String*)(*i))->valores[ini], archivo);
            fputc('\n', archivo);
    	}
    }
    if( tipo == 1 && count < 4096*cantPags ){
    	double trash = -1.11111;
    	while( count < 4096*cantPags ){
    		const char* pointer = reinterpret_cast<const char*>(&trash);
    		fwrite( pointer, sizeof(double), 1, archivo);
    		count += sizeof(double);
    	}
    }

};

void Consultor::resetContadores(){
	lecturas = 0; 
	escrituras = 0;
	movCabezal = 0;
	calcMetrica = 0;
}