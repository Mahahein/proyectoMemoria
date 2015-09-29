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
}

int Cargador::leer(string archivo, int modo){
    ifstream entrada;
    ofstream pivotesDisco;
    entrada.open(archivo.c_str(), ios::in );
    if( !entrada.is_open() ){
        cout << "Error abriendo el archivo" << endl;
        return -1;
    }
    else{
        //cout << "Archivo abierto correctamente..." << endl;
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

    if( tipo == 1 ){
        file.open(salida.c_str(), ios::out | ios::binary);
        file2.open(salida2.c_str(), ios::out);
    }
    else{
        file.open(salida.c_str(), ios::out );
        file2.open(salida2.c_str(), ios::out);
    }   

    objs = 0;
    string primeraLinea;
    getline( entrada, primeraLinea );
    stringstream pl( primeraLinea );
    pl >> buf;
    buf2 = buf.data();
    totalObjs = atoi(buf2);

    pl >> buf;
    buf2 = buf.data();
    dimension = atoi(buf2);

    pl >> buf;
    buf2 = buf.data();
    tipo = atoi(buf2);

    if(tipo == 1)
        tamCluster = (4096*cantPags)/(sizeof(double)*dimension);
    else 
        tamCluster = (4096*cantPags)/(sizeof(char)*dimension);

    for( string linea; getline(entrada, linea);){
        stringstream ss(linea);
        Objeto* ob;
        if( tipo == 1){
            ob = new Vector();
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
            ((String*)ob)->valores = (char*)malloc(sizeof(char)*dimension);
            for( ini = 0; ini < linea.size() ; ini++ ) ((String*)ob)->valores[ini] = linea[ini];
            ((String*)ob)->valores[linea.size()] = 0;
            ((String*)ob)->tamReal = linea.size();
            ob->largoMax = dimension;
        }
        distanciasAPivotes(ob);
        distanciasACentros(ob);
        ob->pos = bolsa.size();
        ob->esPivote = false;
        ob->id = objs;
        bolsa.push_back(ob);
        objs++;
        if( (bolsa.size()%tamCluster) == 1 ){
            elegirPivoteProvisorio();
        }
        if( bolsa.size() == N ){
            pasarAMemoria(cluster, &nPivotes);
            cluster++;
        }
        if( ((double)objs)/totalObjs >= porcentaje){
            cout << porcentaje*100 << "," << lecturas << "," << escrituras << "," << calcMetrica << "," << movCabezal << endl;
            porcentaje+=0.1;
        }
    }
    cout << porcentaje*100 << "," << lecturas << "," << escrituras << "," << calcMetrica << "," << movCabezal << endl;
    file.close();
    file2.close();
    entrada.close(); 
    return tipo;

}

void Cargador::distanciasAPivotes(Objeto* ob){
    int j;
    double dist;
    if(pivotesProvisorios.size()>0){
        j=0;
        for(j = 0; j < pivotesProvisorios.size(); j++){
            double dist = ob->metrica( pivotesProvisorios[j]->centro );
            calcMetrica++;
            ob->poneDistancia(dist);
            ob->aumentaAcumulado(dist);
        }
    }    
}

double Cargador::distanciaPivoteNuevo(Objeto* ob, Pivote* piv){
    vector<double>::iterator k;
    vector<double>::iterator l;
    double dist = ob->metrica(piv->centro);
    calcMetrica++;
    ob->aumentaAcumulado(dist);
    ob->poneDistancia( dist );
    return dist;
}

void Cargador::obtieneCercanos(Pivote* p){
    p->cercanos.clear();
    p->cercanos.resize(0);
    int k = 0;
    for( vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i){
        if( (*i)->id != p->centro->id && !estaEnCercanos((*i)->id, p) ){
            if(p->cercanos.size()<(tamCluster-1)){
                p->cercanos.push_back((*i));
                p->actualizaMasLejano();
            }
            else{
                if( (*i)->distancias->getValueAt(p->pos) < p->radio ){
                    p->cercanos.erase( p->cercanos.begin()+p->posMasLejano );
                    p->cercanos.push_back((*i));
                    p->actualizaMasLejano();
                }
            }
        }
        k++;
    }
}

void Cargador::sacaPivoteDelSaco(int id){
    for(int i = 0; i < bolsa.size(); i++){
	    if( bolsa[i]->id == id){
	        bolsa.erase(bolsa.begin()+i);
	        return;
	    }
    }
}

void Cargador::elegirPivoteProvisorio(){
    vector<Objeto*>::iterator mayObj;
    vector<Objeto*>::iterator i;
    double disAc = -DBL_MAX;
    double distPivNuevo;
    for( i = bolsa.begin(); i != bolsa.end(); ++i ){
        if( (*i)->distanciaAcumulada >= disAc && !(*i)->esPivote ){
            mayObj = i;
            disAc = (*i)->distanciaAcumulada;
        }
    }
    Pivote* piv = new Pivote((*mayObj));
    piv->pos = pivotesProvisorios.size();
    (*mayObj)->esPivote = true;
    (*mayObj)->posPiv = piv->pos;
    pivotesProvisorios.push_back(piv);
    
    for( i = bolsa.begin(); i != bolsa.end(); ++i ){
        distPivNuevo = (*i)->metrica(piv->centro);
        calcMetrica++;
        (*i)->poneDistancia( distPivNuevo );
        (*i)->aumentaAcumulado( distPivNuevo );
    }
}

void Cargador::pasarAMemoria(int nCluster, int* nPivotes){
    int minPi;
    double radio = DBL_MAX;
    vector<Pivote*>::iterator j;
    vector<Objeto*>::iterator i;
    int k, position;
    k=0;
    for( j = pivotesProvisorios.begin(); j != pivotesProvisorios.end(); ++j ){
        obtieneCercanos((*j));
        (*j)->actualizaRadio();
        if( (*j)->radio <= radio ){
            minPi = k;
            radio = (*j)->radio;
        }
    }
    pivotesProvisorios[minPi]->numCluster = nCluster;
    if(tipo == 1){
        const char* pointer = reinterpret_cast<const char*>(&((Vector*)pivotesProvisorios[minPi]->centro)->valores[0]);
        file.write( pointer, sizeof(double)*dimension );
    }
    else{
        pivotesProvisorios[minPi]->posArch = file.tellp();
        for(int ini = 0; ini < ((String*)pivotesProvisorios[minPi]->centro)->tamReal; ini++ )
            file <<  ((String*)pivotesProvisorios[minPi]->centro)->valores[ini];
        file << endl;
    }
    escribeCluster(pivotesProvisorios[minPi]);
    escrituras++;
    int pivotesLlevados = 0;
    vector<Objeto*> pivotes;
    for(k=0; k < pivotesProvisorios[minPi]->cercanos.size(); k++){
        if( pivotesProvisorios[minPi]->cercanos[k]->esPivote ){
            pivotes.push_back(pivotesProvisorios[minPi]->cercanos[k]);
            pivotesLlevados++;
        }
        bolsa.erase(bolsa.begin()+pivotesProvisorios[minPi]->cercanos[k]->pos);
        ajustaPosiciones();
    }

    for(k=0; k < pivotesLlevados; k++){
        pivotesProvisorios.erase(pivotesProvisorios.begin() + pivotes[k]->posPiv);
        ajustaPosicionesPivotes(pivotes[k]->posPiv);
        elegirPivoteProvisorio();
    }

    bolsa.erase(bolsa.begin()+pivotesProvisorios[minPi]->centro->pos);
    ajustaPosiciones();
    pivotesEnMemoria.push_back( pivotesProvisorios[minPi] );
    for(i = pivotesProvisorios[minPi]->cercanos.begin(); i != pivotesProvisorios[minPi]->cercanos.end(); ++i ){
        delete (*i);
    }
    pivotesProvisorios[minPi]->cercanos.resize(0);
    pivotesProvisorios[minPi]->cercanos.shrink_to_fit();
    sacaPivoteDeProvisorios(pivotesProvisorios[minPi]->centro->id);
}

void Cargador::sacaPivoteDeProvisorios(int id){
    int k = 0;
    for( vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i ){
        if( (*i)->centro->id == id ){
            pivotesProvisorios.erase( i );
            
            ajustaPosicionesPivotes(k);
            
            return;
        }
        k++;
    }
}

void Cargador::ajustaPosicionesPivotes(int posElim){
    int k;
    for( vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i ){
        (*i)->distancias->deleteValue( posElim );
    }
    k = 0;
    for( vector<Pivote*>::iterator i = pivotesProvisorios.begin(); i != pivotesProvisorios.end(); ++i  ){
        (*i)->pos = k;
        (*i)->centro->posPiv = k;
        k++;
    }
}

void Cargador::ajustaPosiciones(){
    int k = 0;
    for(vector<Objeto*>::iterator i = bolsa.begin(); i != bolsa.end(); ++i ){
        (*i)->pos = k;
        k++;
    }
}

bool Cargador::estaEnCercanos( int id, Pivote* piv){
    for(int i = 0; i < piv->cercanos.size(); i++)
        if( piv->cercanos[i]->id == id) return true;
    return false;
}

void Cargador::escribeCluster(Pivote* centro){
    vector<Objeto*>::iterator i;
    int count = 0;
    int ini;    
    for( i = centro->cercanos.begin(); i != centro->cercanos.end(); ++i ){
        if(tipo == 1 ){
            const char* pointer = reinterpret_cast<const char*>(&((Vector*)(*i))->valores[0]);
            file.write( pointer, sizeof(double)*((Vector*)(*i))->valores.size());
            count += sizeof(double) * ((Vector*)(*i))->valores.size();
        }
        else{
            for(ini = 0; ini < ((String*)(*i))->tamReal; ini++ )
                file <<  ((String*)(*i))->valores[ini];
            file << endl;
        }
        
    }
    if( tipo == 1 && count < (4096*cantPags) ){
        double trash = -1.11111;
        while( count < (4096*cantPags) ){
            const char* pointer = reinterpret_cast<const char*>(&trash);
            file.write( pointer, sizeof(double));
            count += sizeof(double);
        }
    }
}

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
