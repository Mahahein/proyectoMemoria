/* 
 * File:   Saco.h
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 03:30 PM
 */

#ifndef CARGADOR_H
#define	CARGADOR_H
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include "Objeto.h"
#include "Pivote.h"
using namespace std;

class Cargador {
public:
    vector<Objeto*> bolsa;
    vector<Pivote*> pivotesProvisorios;
    vector<Pivote*> pivotesEnMemoria;
    ofstream file;
    ofstream file2;
    int dimension;
    int tamCluster;
    int totalObjs;
    int tipo;
    unsigned long escrituras;
    unsigned long lecturas;
    unsigned long movCabezal;
    unsigned long calcMetrica;
    int cantPags;
    Cargador();
    Cargador(int pages);
    Cargador(const Cargador& orig);
    virtual ~Cargador();
    int leer(string archivo, int modo);
    void distanciasAPivotes(Objeto* ob);
    void distanciasACentros(Objeto* ob);
    void obtieneCercanos(Pivote* p);
    void sacaPivoteDeProvisorios(int id);
    void elegirPivoteProvisorio();
    void pasarAMemoria(int nCluster, int* nPivotes);
    void ajustaPosicionesPivotes(int posElim);
    void ajustaPosiciones();
    void escribeCluster(Pivote* centro);
private:

};

#endif	/* CARGADOR_H */

