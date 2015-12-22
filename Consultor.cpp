
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

long Consultor::consultarBusquedaRango(Objeto* ob, double r){
	vector<Objeto*> cercanos;
	vector<Pivote*> candidatos;
	vector<Pivote*>::iterator l;
	vector<Objeto*>::iterator m;
	int i, j, k;
	double dist;
	bool esta;
	long respuesta = 0;
	//int contTot = 0;
	//BUSCANDO EN LOS CLUSTER
	//cout << "Buscando clusters con posibles vecinos" << endl;
	double *distanciaAClusters(new double[clusters.size()]);
	k=0;
	for( l = clusters.begin(); l != clusters.end(); ++l ){
		calcMetrica++;
		dist = ob->metrica((*l)->centro);
		if( dist <= (r + (*l)->radio )){
			candidatos.push_back((*l));
			distanciaAClusters[k]= dist;
			k++;
		}
	}

	//cout << "sacando los posibles vecinos de los clusters" << endl;
	vector<Objeto*> objetosBloque;
	k=0;
	if(!candidatos.empty()){

		for( l = candidatos.begin(); l != candidatos.end(); ++l ){

			objetosBloque = extraerCluster( (*l)->numCluster, (*l)->posArch );
			lecturas++;

			for( m = objetosBloque.begin(); m != objetosBloque.end(); ++m ){
				if( fabs( distanciaAClusters[k] - (*m)->distanciaACentro ) <= r ){
					calcMetrica++;
					if( ob->metrica((*m)) <= r ){
						respuesta += 1;
					}
				}
				/**/
			}
			for( m = objetosBloque.begin(); m != objetosBloque.end(); ++m ){
				delete *m;
			}
			k++;
			vector<Objeto*>().swap(objetosBloque);
			objetosBloque.resize(0);
			objetosBloque.shrink_to_fit();
			
		}
	}

	vector<Pivote*>().swap(candidatos);
	candidatos.resize(0);
	candidatos.shrink_to_fit();
	//cout << "buscando en los pivotes en ram, calculando distancias a los pivotes" << endl;


	double *distPivs(new double[pivotes.size()]);
	i=0;
	for( l = pivotes.begin(); l != pivotes.end(); ++l ){
		calcMetrica++;
		distPivs[i] = ob->metrica((*l)->centro);
		i++;
	}

	//ELIGIENDO POR DESIGUALDAD TRIANGULAR
	//cout << "buscando por distancia triangular y luego directo a los posibles vecinos" << endl;
	bool sobrevive;
	for( m = saco.begin(); m != saco.end(); ++m ){
		sobrevive = true;
		for( j = 0; j < (*m)->sizeDistancias; j++ ){
			if( fabs( distPivs[j] - (*m)->distancias[j] ) > r ){ // DESIGUALDAD TRIANGULAR
				sobrevive = false;
				break;
			}
		}
		if( sobrevive ){
			calcMetrica++;
			if( ob->metrica((*m)) <= r){
				respuesta += 1;
			}
		}
	}
	delete[] distPivs;
	//cout << "total encontrados " << cercanos.size() << endl;
	
	return respuesta;

};

bool Consultor::eliminarObjeto(Objeto* ob){
	bool eliminado = false;
	bool igual;

	vector<Pivote*>::iterator k;
	vector<Objeto*>::iterator i;
	vector<Objeto*>::iterator m;

	vector<Objeto*> bloque;
	vector<Pivote*> candidatos;
	bloque.resize(0);
	bloque.shrink_to_fit();

	double dist;
	int j, o;
	//Buscando en RAM
	double *distPivs(new double[pivotes.size()]);
	j=0;
	for( k = pivotes.begin(); k != pivotes.end(); ++k ){
		calcMetrica++;
		distPivs[j] = ob->metrica((*k)->centro);
		j++;
	}

	for( i = saco.begin(); i != saco.end() && !eliminado; ++i ){
		for(j = 0; j < (*m)->sizeDistancias; j++){
			if( fabs( distPivs[j] - (*i)->distancias[j] ) < 0.0001 ){
				calcMetrica++;
				if(ob->metrica(*i) <= 0.00001){
					if( (*i)->esPivote ){
						eliminaPivote( (*i)->id );
						ajustaPosicionesPivotes((*i)->posPiv);
						elegirPivote();
					}
					saco.erase(saco.begin()+(*i)->pos);
					ajustaPosicionesSaco();
					eliminado = true;
					return eliminado;
				}
			}
		}
	}

	//BUSCAR EN CLUSTERS
	int l;
	double dist2;
	for( l = 0; l < clusters.size() && !eliminado; l++){
		dist = ob->metrica(clusters[l]->centro);
		calcMetrica++;
		if( dist <= clusters[l]->radio ){
			bloque = extraerCluster( clusters[l]->numCluster,  clusters[l]->posArch);
			lecturas++;
			for( j= 0; j < tamCluster; j++ ){
				if( dist - bloque[j]->distanciaACentro <= 0.000001 ){
					dist2 = ob->metrica(bloque[j]);
					calcMetrica++;
					if( dist2 <= 0.000001 ){
						if( j == 0 ){ //CENTRO DE CLUSTER
							double menor = DBL_MAX;
							int posMen = 0;
							Objeto* menorOb = bloque[0];
							for( o = 0; o < tamCluster; o++){
								if( bloque[o]->distanciaACentro < menor ){
									menor = bloque[o]->distanciaACentro;
									menorOb = bloque[o];
									posMen = o;
								}
							}
							bloque[0] = menorOb;
							for( o = 0; o < tamCluster; o++){
								bloque[o]->distanciaACentro = bloque[o]->metrica(bloque[0]);
							}
							j = posMen;
							delete clusters[l]->centro;
							clusters[l]->centro = menorOb;
						}
						Objeto* nuevo = cercanoA( bloque[0] );
						nuevo->distanciaACentro = nuevo->metrica(bloque[0]);
						if( nuevo->esPivote ){
							eliminaPivote( nuevo->id );
							ajustaPosicionesPivotes(nuevo->posPiv);
							if( (saco.size()-1)%tamCluster == 1 )
								elegirPivote();
						}
						delete bloque[j];
						bloque[j] = nuevo;
						saco.erase( saco.begin() + nuevo->pos);
						ajustaPosicionesSaco();
						clusters[l]->cercanos.swap(bloque);
						reescribeCluster(clusters[l]);
						eliminado = true;
						for(i = clusters[l]->cercanos.begin()+1; i != clusters[l]->cercanos.end(); ++i ){
							delete (*i);
						}
						clusters[l]->cercanos.resize(0);
						clusters[l]->cercanos.shrink_to_fit();
						return eliminado;
					}
				}
			}
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
		double *block(new double[512*cantPags]);
		fread( block, sizeof(double), 512*cantPags, archivo );
		for( j = 0; j < ((dimension+1)*tamCluster); j+=(dimension+1) ){
			Vector* objMem = new Vector();
			for( k = 0; k < dimension; k++ ){
				objMem->poneValor( block[j+k] );
			}
			objMem->distanciaACentro = block[j+k];
			//cout << endl;
			objetos.push_back(objMem);
		}
		delete[] block;
	}
	else{
		if(position != ftell(archivo) ) movCabezal++;
		fseek( archivo, position, SEEK_SET );
		char *linea(new char[dimension+5]);
		char *palabra;
		char *numero;
		char *end;
		double d;
		for(k = 0; k < tamCluster; k++){
			fscanf( archivo, "%s", linea);
			palabra = strtok(linea, " ");
			numero = strtok(NULL, "\n");
			String* objMem = new String();
			objMem->valores = (new char[dimension]);
			for(j = 0; j < strlen(palabra); j++){
				objMem->valores[j] = palabra[j];
			}
			d = strtod(numero, &end);
			objMem->distanciaACentro = d;
			objMem->valores[strlen(linea)] = 0;
			objMem->tamReal = strlen(linea);
			objMem->dimension = dimension;
			objetos.push_back(objMem);
		}
		delete[] linea;			
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
        (*i)->eliminaDistancia( posElim );
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
    		(*i)->poneValor((*i)->distanciaACentro);
	        const char* pointer = reinterpret_cast<const char*>(&((Vector*)(*i))->valores);
	        fwrite( pointer, sizeof(double), ((Vector*)(*i))->sizeValores, archivo );
	        count += sizeof(double) * (((Vector*)(*i))->sizeValores);
    	}
    	else{
    		for(ini = 0; ini < ((String*)(*i))->tamReal; ini++ )
    			fputc(((String*)(*i))->valores[ini], archivo);
    		fprintf( archivo, " %f\n", (*i)->distanciaACentro);
            
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