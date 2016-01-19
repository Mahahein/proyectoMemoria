/* 
 * File:   Saco.cpp
 * Author: Maha
 * 
 * Created on 5 de enero de 2015, 03:30 PM
 */

#include "Cargador.h"
#include "Objeto.h"
#include "String.h"
#include "Vector.h"
#include "Pivote.h"
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <stdlib.h>
#include <float.h>
#include <algorithm>
#include <math.h>
#define N 8192
#define n 256
#define B 256

using namespace std;

Cargador::Cargador() {
    lecturas = 0;
    escrituras = 0;
    movCabezal = 0;
    calcMetrica = 0;
    cantPags = 1;
}

Cargador::Cargador(int pages){
    cantPags = pages;
    lecturas = 0;
    escrituras = 0;
    movCabezal = 0;
    calcMetrica = 0;
}

Cargador::Cargador(const Cargador& orig) {
}

Cargador::~Cargador() {
    for(vector<Objeto*>::iterator v = bolsa.begin(); v != bolsa.end(); ++v)
        delete *v;
    for(vector<Pivote*>::iterator w = pivotesProvisorios.begin(); w != pivotesProvisorios.end(); ++w )
        delete *w;
    for(vector<Pivote*>::iterator u = pivotesEnMemoria.begin(); u != pivotesEnMemoria.end(); ++u)
        delete *u;
}

int Cargador::leer(string archivo, int modo){
    ifstream entrada;
    ofstream pivotesDisco;
    entrada.open(archivo.c_str(), ios::in );
    if( !entrada.is_open() ){
        cout << "Error abriendo el archivo" << endl;
        return -1;
    }

    string buf;
    const char* buf2;
    long cluster = 0;
    int nPivotes = 0;
    char* end;
    int cont, mayObj, k, l, minPiv, objs, ini;
    double porcentaje = 0.1;
    vector<Objeto*>::iterator i;
    vector<Pivote*>::iterator menor, vi;
    vector<double>::iterator j;    
    string salida = "clusters.bin";
    string salida2 = "clusters.txt";
    objs = 0;

    //######################
    //Leyendo los parametros de la primera linea
    //Cant. objetos
    string primeraLinea;
    getline( entrada, primeraLinea );
    stringstream pl( primeraLinea );
    pl >> buf;
    buf2 = buf.data();
    totalObjs = atoi(buf2);
    //dimension
    pl >> buf;
    buf2 = buf.data();
    dimension = atoi(buf2);
    //tipo
    pl >> buf;
    buf2 = buf.data();
    tipo = atoi(buf2);

    //#######################
    //abriendo el archivo de almacenamiento
    //y definiendo el tamaño de los clusters
    //segun el tipo
    if(tipo == 1){
        file.open(salida.c_str(), ios::out | ios::binary);
        tamCluster = (4096*cantPags)/(sizeof(double)*(dimension+1));
        if(dimension > 64){
            while (tamCluster < 32 ){
                cantPags++;
                tamCluster = (4096*cantPags)/(sizeof(double)*(dimension+1));
            }
        }
    }
    else {
        file.open(salida.c_str(), ios::out );
        tamCluster = (4096*cantPags)/(sizeof(char)*dimension);
    }
    cout << tamCluster << endl;

    //LECTURA DE LOS DATOS
    for( string linea; getline(entrada, linea);){
        //Se obtiene la linea y se procesa
        //segun el tipo
        stringstream ss(linea);
        Objeto* ob;
        if( tipo == 1){
            ob = new Vector();
            ob->dimension = dimension;
            cont = 0;
            while( ss >> buf ){
                buf2 = buf.data();
                if(isdigit(buf2[0]) || buf2[0] == '-' ){
                    if(cont == 0)
                        ob->id = atoi(buf2);
                    else 
                        ob->poneValor(strtod(buf2, &end));           
                }
                cont++;
            }
        }
        else{
            ob = new String();
            ob->dimension = dimension;
            ((String*)ob)->valores = (new char[dimension]);
            for( ini = 0; ini < linea.size() ; ini++ ) ((String*)ob)->valores[ini] = linea[ini];
            ((String*)ob)->valores[linea.size()] = 0;
            ((String*)ob)->tamReal = linea.size();
        }
        //Se calcula y guarda la distancia a los pivotes existentes
        distanciasAPivotes(ob);
        //Se calcula la distancia a los centros de clusters
        distanciasACentros(ob);
        //se le asigna la posición del objeto dentro de la bolsa/buffer
        ob->pos = bolsa.size();
        //no es pivote
        ob->esPivote = false;
        //su id es el numero de elemento en leerse
        ob->id = objs;
        //su tipo
        ob->tipo = tipo;
        //se agrega a la bolsa/buffer
        bolsa.push_back(ob);
        //se aumenta el contador de objetos
        objs++;
        //Si ya llegaron tamCluster elementos
        //se elige pivote nuevo dentro de la bolsa/buffer
        if( (bolsa.size()%tamCluster) == 1 ){
            elegirPivoteProvisorio();
        }
        //Si se llenó la bolsa/buffer
        //se procede a pasar un cluster a memoria secundaria
        if( bolsa.size() == N ){
            //cout << "pasando a memoria" << endl;
            pasarAMemoria(cluster, &nPivotes);
            //cout << "pasó a memoria" << endl;
            cluster++;
        }
        //salida para marcar el porcentaje de avance (cada 10%)
        if( ((double)objs)/totalObjs >= porcentaje){
            cout << porcentaje*100 << "," << lecturas << "," << escrituras << "," << calcMetrica << "," << movCabezal << endl;
            porcentaje+=0.1;
        }
        
    }
    file.close();
    entrada.close(); 
    return tipo;

}
//Función que calcula la distancia de un objeto a los pivotes existentes
//La distancia es guardada en la lista de distancias y aumenta la distancia acumulada del objeto
void Cargador::distanciasAPivotes(Objeto* ob){
    int j;
    double dist;
    if(pivotesProvisorios.size()>0){
        for(j = 0; j < pivotesProvisorios.size(); j++){
            dist = ob->metrica( pivotesProvisorios[j]->centro );
            calcMetrica++;
            ob->poneDistancia(dist);
            ob->aumentaAcumulado(dist);
        }
    }    
}

//Función que obtiene los "tamCluster"-1 vecinos más cercanos de un pivote
void Cargador::obtieneCercanos(Pivote* p){
    p->cercanos.clear();
    //p->cercanos.resize(0);
    int k = 0;
    for( vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i){
        if( (*i)->id != p->centro->id ){
            if(p->cercanos.size()<(tamCluster-1)){
                p->cercanos.push_back((*i));
                p->actualizaMasLejano();
            }
            else{
                if( (*i)->distancias[p->pos] < p->radio ){
                    p->cercanos.erase( p->cercanos.begin()+p->posMasLejano );
                    p->cercanos.push_back((*i));
                    p->actualizaMasLejano();
                }
            }
        }
        k++;
    }
}

//Función que elige un nuevo pivote dentro de la bolsa/buffer
//El objeto cuya distancia acumulada sea la mayor es el nuevo pivote
void Cargador::elegirPivoteProvisorio(){
    vector<Objeto*>::iterator mayObj;
    vector<Objeto*>::iterator i;
    double disAc = -DBL_MAX;
    double distPivNuevo;
    int k, elegido;
    for(k = 0; k < bolsa.size(); k++ ){
        if((bolsa[k])->distanciaAcumulada > disAc && !bolsa[k]->esPivote ){
            elegido = k;
            disAc = (bolsa[k])->distanciaAcumulada;
        }
    }
    Pivote* piv = new Pivote(bolsa[elegido]);
    piv->pos = pivotesProvisorios.size();
    (bolsa[elegido])->esPivote = true;
    (bolsa[elegido])->posPiv = piv->pos;
    pivotesProvisorios.push_back(piv);
    for(k = 0; k < bolsa.size(); k++ ){
        distPivNuevo = (bolsa[k])->metrica(piv->centro);
        calcMetrica++;
        (bolsa[k])->poneDistancia(distPivNuevo);
        (bolsa[k])->aumentaAcumulado(distPivNuevo);
    }
}

//Función que pasa a la memoria un cluster
//Creación de cluster:
//// 1 - Todos los pivotes calculan sus "tamCluster"-1 vecinos más cercanos.
//// 1.1 - Con sus vecinos calculados, obtienen un radio de cobertura.
//// 2 - El pivote cuyo radio de cobertura sea el menor es el que pasa a memoria secundaria.
//// 3 - Se escribe el centro del cluster.
//// 4 - Luego se escriben sus cercanos.
//// 5 - Elimina (de la memoria principal) los vecinos escritos en memoria secundaria.
//// 6 - Verifica si alguno de los vecinos escritos era un pivote.
//// 7 - Elimina de la bolsa/buffer los objetos escritos.
//// 8 - De haber pivotes entre los vecinos, los elimina de la lista de pivotes
////     y elige pivotes nuevos.
//// 9 - Elimina el pivote elegido para pasar a memoria secundaria de la lista de pivotes
////     y lo agrega a la lista de centros de clusters.
void Cargador::pasarAMemoria(int nCluster, int* nPivotes){
    int minPi;
    double radio = DBL_MAX;
    vector<Pivote*>::iterator j;
    vector<Objeto*>::iterator i;
    int k, position, l;
    k=0;
    //###### 1 - 1.1 - 2 ######
    for( j = pivotesProvisorios.begin(); j != pivotesProvisorios.end(); ++j ){
        obtieneCercanos((*j));
        if( (*j)->radio <= radio ){
            minPi = k;
            radio = (*j)->radio;
        }
        k++;
    }
    pivotesProvisorios[minPi]->numCluster = nCluster;
    //###### 1 - 1.1 - 2 ######
    
    //########### 3 ###########
    if(tipo == 1){
        pivotesProvisorios[minPi]->posArch = file.tellp();
        pivotesProvisorios[minPi]->centro->poneValor(0.0);
        const char* pointer = reinterpret_cast<const char*>(&((Vector*)pivotesProvisorios[minPi]->centro)->valores[0]);   
        file.write( pointer, sizeof(double)*(((Vector*)pivotesProvisorios[minPi]->centro))->sizeValores );
    }
    else{
        pivotesProvisorios[minPi]->posArch = file.tellp();
        for(int ini = 0; ini < ((String*)pivotesProvisorios[minPi]->centro)->tamReal; ini++ )
            file <<  ((String*)pivotesProvisorios[minPi]->centro)->valores[ini];
        file << " 0.0" << endl;
    }
    //########### 3 ########### 

    //########### 4 ###########
    escribeCluster(pivotesProvisorios[minPi]);
    //########### 4 ########### 


    escrituras++;
    int pivotesLlevados = 0;
    vector<double> pivotes;
    //######### 5 - 6 #########
    for(k=0; k < pivotesProvisorios[minPi]->cercanos.size(); k++){
        if( pivotesProvisorios[minPi]->cercanos[k]->esPivote ){
            delete pivotesProvisorios[pivotesProvisorios[minPi]->cercanos[k]->posPiv];
            pivotesProvisorios[pivotesProvisorios[minPi]->cercanos[k]->posPiv] = NULL;
            pivotesLlevados++;
        }
        delete pivotesProvisorios[minPi]->cercanos[k];
        bolsa[pivotesProvisorios[minPi]->cercanos[k]->pos] = NULL;
        
    }
    //######### 5 - 6 #########

    //########### 7 ###########
    for(k = 0; k < bolsa.size();){
        if(bolsa[k] != NULL ){
            bolsa[k]->pos = k;
            k++;
        }
        else{
            bolsa.erase(bolsa.begin()+k);
        }
    }
    //########### 7 ###########

    //########### 8 ###########
    for( k = 0; k < pivotesProvisorios.size();){
        if(pivotesProvisorios[k] != NULL){
            pivotesProvisorios[k]->pos = k;
            pivotesProvisorios[k]->centro->posPiv = k;
            k++;
        }
        else{
            pivotesProvisorios.erase(pivotesProvisorios.begin() + k);
            ajustaPosicionesPivotes(k);
            elegirPivoteProvisorio();
        }
    }
    //########### 8 ###########

    //########### 9 ###########
    bolsa.erase(bolsa.begin()+pivotesProvisorios[minPi]->centro->pos);
    ajustaPosiciones();
    pivotesEnMemoria.push_back( pivotesProvisorios[minPi] );
    vector<Objeto*>().swap(pivotesProvisorios[minPi]->cercanos);
    int posi = pivotesProvisorios[minPi]->pos;
    pivotesProvisorios.erase(pivotesProvisorios.begin() + pivotesProvisorios[minPi]->pos);
    ajustaPosicionesPivotes(posi);
    //########### 9 ###########
}

//Función que saca un pivote de la lista de pivotes dado el id del objeto
void Cargador::sacaPivoteDeProvisorios(int id){
    int k = 0;
    for( vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i ){
        if( (*i)->centro->id == id ){
            pivotesProvisorios.erase( pivotesProvisorios.begin() + k );
            ajustaPosicionesPivotes(k);
            return;
        }
        k++;
    }
}

//Función que ajusta las posiciones (la variable) de los pivotes en la lista de pivotes.
//También elimina de la lista de distancias de los objetos el pivote que está siendo eliminado.
void Cargador::ajustaPosicionesPivotes(int posElim){
    int k;
    for( vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i ){
        (*i)->eliminaDistancia( posElim );
    }
    k = 0;
    for( vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i  ){
        if( (*i) != NULL){
            (*i)->pos = k;
            (*i)->centro->posPiv = k;
            k++;
        }
    }
}

//Función que ajusta las posiciones de los objetos (variable) en la bolsa/buffer.
void Cargador::ajustaPosiciones(){
    int k = 0;
    int i;
    for( i = 0; i < bolsa.size(); i++ ){
        (bolsa[i])->pos = k;
        k++;
    }
}

//Función que escribe los vecinos de un pivote en memoria secundaria.
//Si es un espacio vectorial, la escritura es en binario;
//si es un espacio de string, se escribe como texto plano.
void Cargador::escribeCluster(Pivote* centro){
    vector<Objeto*>::iterator i;
    int count = 0;
    int ini;    
    for( i = centro->cercanos.begin(); i != centro->cercanos.end(); ++i ){
        if(tipo == 1 ){
            (*i)->poneValor((*i)->distancias[centro->pos]);
            const char* pointer = reinterpret_cast<const char*>(&((Vector*)(*i))->valores[0]);
            file.write( pointer, sizeof(double)*((Vector*)(*i))->sizeValores);
            count += sizeof(double) * (((Vector*)(*i))->sizeValores);
        }
        else{
            for(ini = 0; ini < ((String*)(*i))->tamReal; ini++ ){
                file <<  ((String*)(*i))->valores[ini];
            }
            file << " " << (*i)->distancias[centro->pos];
            file << endl;
        }
        
    }
    //Relleno para completar las páginas escritas
    if( tipo == 1 && count < (4096*cantPags) ){
        double trash = -1.11111;
        while( count < (4096*cantPags) ){
            const char* pointer = reinterpret_cast<const char*>(&trash);
            file.write( pointer, sizeof(double));
            count += sizeof(double);
        }
    }
}


//Función que calcula la distancia a los centros de clusters
//La distancia aumenta la distancia acumulada solamente
void Cargador::distanciasACentros(Objeto* ob){
    int j;
    double dist;
    if(pivotesEnMemoria.size()>0){
        j=0;
        for(j = 0; j < pivotesEnMemoria.size(); j++){
            double dist = ob->metrica( pivotesEnMemoria[j]->centro );
            calcMetrica++;
            ob->aumentaAcumulado(dist);
        }
    }
}
