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
    int escrituras;
    int lecturas;
    int movCabezal;
    int calcMetrica;
    int cantPags;
    Cargador();
    Cargador(int pages);
    Cargador(const Cargador& orig);
    virtual ~Cargador();
    int leer(string archivo, int modo);
    void distanciasAPivotes(Objeto* ob);
    void distanciasACentros(Objeto* ob);
    double distanciaPivoteNuevo(Objeto* ob, Pivote* piv);
    void obtieneCercanos(Pivote* p);
    void sacaPivoteDelSaco(int id);
    void sacaPivoteDeProvisorios(int id);
    void elegirPivoteProvisorio();
    void pasarAMemoria(int nCluster, int* nPivotes);
    void ajustaPosicionesPivotes(int posElim);
    void ajustaPosiciones();
    bool estaEnCercanos(int id, Pivote* piv);
    //void escribeOb(Objeto* ob);
    void escribeCluster(Pivote* centro);

private:

};

#endif	/* CARGADOR_H */

