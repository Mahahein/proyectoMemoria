/* 
 * File:   main.cpp
 * Author: Maha
 *
 * Created on 5 de enero de 2015, 10:42 AM
 */

#include <vector>
#include <set>
#include <string>
#include <string.h>
#include <fstream>
#include <time.h>
#include "Cargador.h"
#include "Lector.h"
#include "Objeto.h"
#include "Vector.h"
#include "String.h"
#include "Consultor.h"
#include <sys/stat.h>

using namespace std;

/*
 * 
 */

int menuPrincipal();
int menuCargarDatos();

int menuHacerConsulta();
int tipo;
Objeto* submenuHacerConsultaEE( int dim );
string submenuHacerConsultaArchivoEntrada();
string submenuHacerConsultaArchivoSalida();
void escribeRespuesta( vector<Objeto*> resp, ofstream& archivo );
bool verificaClustersBin();

int main(int argc, char** argv) {
    Cargador* s = NULL;
    Consultor* c;
    Lector* l = new Lector();
    
    int dimension;
    ofstream archivoSalida;
    //vector<Objeto*> resp;
    vector<Objeto*> objetos;
    bool respuestaEliminacion;
    string nombreArchivo;
    int opc = 1;
    //double radios[4] = {255.0, 500.0, 760.0, 1010.0};

    while( opc != 0 ){
        
        if( opc == 1){
            int opc1 = 1;
            if( opc1 == 1 ){
                nombreArchivo = "bdNoraFixed.txt";
                s = new Cargador();
                cout << "porcentaje,lecturas,escrituras,calcMetrica,movCabezal" << endl;
                tipo = s->leer( nombreArchivo, 1 );
                dimension = s->dimension;
                /*c = new Consultor();
                c->pivotes.swap(s->pivotesProvisorios);
                c->clusters.swap(s->pivotesEnMemoria);
                c->saco.swap(s->bolsa);
                c->dimension = dimension;
                c->tamCluster = s->tamCluster;
                c->totalObjs = s->totalObjs;
                c->tipo = tipo;*/
                delete s;
                l->dimension = dimension;
                l->tipo = tipo;
                opc=0;
            }
            else if( opc1 == 2 ){
                nombreArchivo = submenuHacerConsultaArchivoEntrada();
                if( s == NULL ){
                    cout << "No se han cargado datos" << endl;    
                }
                else{
                    //Consultor* c = new Consultor();
                    s->pivotesProvisorios.swap(c->pivotes);
                    s->pivotesEnMemoria.swap(c->clusters);
                    s->bolsa.swap(c->saco);
                    tipo = s->leer( nombreArchivo, 2 );
                    if( tipo != -1 ){
                        c = new Consultor();
                        c->pivotes.swap(s->pivotesProvisorios);
                        c->clusters.swap(s->pivotesEnMemoria);
                        c->saco.swap(s->bolsa);
                        c->dimension = s->dimension;
                        c->tamCluster = s->tamCluster;
                        c->totalObjs = s->totalObjs;
                        c->tipo = tipo;
                        l->dimension = s->dimension;
                        l->tipo = tipo;
                    }
                }
            }
            
        }
        else if( opc == 2 ){
            //Consultor* c = new Consultor();
            nombreArchivo = "bdNoraFixed_90p.txt";
            s = new Cargador();
            cout << "porcentaje,lecturas,escrituras,calcMetrica,movCabezal" << endl;
            tipo = s->leer( nombreArchivo, 1 );
            dimension = s->dimension;
            c = new Consultor();
            c->pivotes.swap(s->pivotesProvisorios);
            c->clusters.swap(s->pivotesEnMemoria);
            c->saco.swap(s->bolsa);
            c->dimension = dimension;
            c->tamCluster = s->tamCluster;
            c->totalObjs = s->totalObjs;
            c->tipo = tipo;
            c->cantPags = s->cantPags;
            l->dimension = dimension;
            l->tipo = tipo;
            int opc2 = 2;
            string entrada, salida;
            long resp;
            if( opc2 == 1 ){
                salida = submenuHacerConsultaArchivoSalida();
                Objeto* o = l->leerDesdeEE();
                c->resetContadores();
                //resp = c->consultarBusquedaRango( o, o->distanciaAcumulada );
                archivoSalida.open( salida.c_str(), ios_base::out );
                //archivoSalida << "la respuesta arrojó " << resp.size() << " objetos de respuesta" << endl;
                //escribeRespuesta( resp, archivoSalida );
                archivoSalida.close();
            }
            else if( opc2 == 2 ){
                entrada = "bdNoraFixed_10p.txt";
                //entrada = "vectors-20_10p.dat";
                salida = "salidaExperimentoBusquedaENGPiv.txt";
                archivoSalida.open( salida.c_str(), ios_base::out );
                archivoSalida << "radio,lecturas,escrituras,calcMetrica,movCabezal,tiempo" << endl;
                objetos = l->leerDesdeArchivo( entrada );
                cout << "leyo bien... buscando" << endl;
                unsigned long long totLec, totEsc, totMet, totMov;
                double totTim;
                long k;
                time_t ini, fin;
                //double radios[3] = {0.051768, 0.082514, 0.131163};
                //double radios[3] = {0.12, 0.285, 0.53};
                double radios[4] = {255.0, 500.0, 760.0, 1010.0};//, 3.0, 4.0};
                for( int j = 0; j < 3; j++){
                    k=0;
                    totLec = 0;
                    totEsc = 0;
                    totMet = 0;
                    totMov = 0;
                    totTim = 0;
                    for( vector<Objeto*>::iterator i = objetos.begin(); i != objetos.end(); ++i ){
                        //(*i)->distanciaAcumulada = ;
                        c->resetContadores();
                        ini = time(0);
                        resp = c->consultarBusquedaRango( (*i), radios[j]);
                        fin = time(0);
                        totTim += (double) (fin - ini);
                        totLec += c->lecturas;
                        totEsc += c->escrituras;
                        totMet += c->calcMetrica;
                        totMov += c->movCabezal;
                        k++;
                        //for( vector<Objeto*>::iterator v = resp.begin(); v != resp.end(); ++v )
                        //    delete *v;
                        //vector<Objeto*>().swap(resp);
                        //delete[] resp;
                    }
                    archivoSalida << radios[j] << "," << ((double)totLec)/k <<"," << ((double)totEsc)/k << "," << ((double)totMet)/k <<"," << ((double)totMov)/k << "," << totTim/k << endl;
                }
                archivoSalida.close();
                opc=0;
            }
        }
        else if( opc == 3 ){
            //Consultor* c = new Consultor();
            if( s == NULL ){
                cout << "No se han cargado datos" << endl;       
            }
            else{
                int opc3 = menuHacerConsulta();
                string entrada, salida;
                if( opc3 == 1 ){
                    Objeto* o = l->leerDesdeEE();
                    respuestaEliminacion = c->eliminarObjeto( o );
                    if( respuestaEliminacion ){
                        cout << "El objeto fue eliminado" << endl;
                    }
                    else{
                        cout << "El objeto no fue encontrado, por lo tanto no se eliminó" << endl;
                    }
                }
                else if( opc3 == 2 ){
                    entrada = submenuHacerConsultaArchivoEntrada();
                    salida = submenuHacerConsultaArchivoSalida();
                    archivoSalida.open( salida.c_str(), ios_base::out );
                    objetos = l->leerDesdeArchivo( entrada );
                    int k = 0;
                    //cout << "iniciando eliminacion de objetos" << endl;
                    //for( vector<Objeto*>::iterator i = objetos.begin(); i != objetos.end(); ++i ){
                    for( k = 0; k < objetos.size(); k++ ){
                        //cout << "eliminando objeto " << k << endl;
                        respuestaEliminacion = c->eliminarObjeto( objetos[k] );
                        archivoSalida << "Eliminación " << k << endl;
                        if( respuestaEliminacion ){
                            archivoSalida << "El objeto fue eliminado" << endl;
                            //cout << "eliminado el objeto " << k << endl;
                        }
                        else{
                            archivoSalida << "El objeto no fue encontrado, por lo tanto no se eliminó" << endl;
                            //cout << "no fue eliminado el objeto " << k << endl;
                        }
                    }
                    archivoSalida.close();
                }
            }
        }
        
        //opc = menuPrincipal();
    }
    
    return 0;
}

int menuPrincipal(){
    int opc;
    cout << "[1] Cargar Datos" << endl;
    cout << "[2] Realizar Consulta de Busqueda de Vecinos" << endl;
    cout << "[3] Eliminar Objeto" << endl;
    cout << "[0] Salir" << endl;
    cout << "Ingrese Opción: ";
    cin >> opc;
    while( opc < 0 || opc > 3){
        cout << "Opción no válida. Ingrese Opción: ";
        cin >> opc;
    }
    return opc;
}

int menuCargarDatos(){
    int opc;
    cout << "[1] Cargar datos (Crear BD)" << endl;
    cout << "[2] Agregar datos" << endl;
    cout << "[0] Volver" << endl;
    cout << "Ingrese Opción:";
    cin >> opc;
    while( opc < 0 || opc > 2 ){
        cout << "Opción no válida. Ingrese Opción: ";
        cin >> opc;   
    }
    return opc;
}

int menuHacerConsulta(){
    int opc;
    cout << "[1] Ingresar objeto desde teclado" << endl;
    cout << "[2] Cargar objeto(s) desde archivo" << endl;
    cout << "[0] Volver" << endl;
    cout << "Ingrese Opción: ";
    cin >> opc;
    while( opc < 0 || opc > 2){
        cout << "Opción no válida. Ingrese Opción: ";
        cin >> opc;
    }
    return opc;   
}

Objeto* submenuHacerConsultaEE( int dim ){
    Objeto* ob;
    double vi, r;
    string st;
    cout << "La dimensión del espacio es " << dim << endl;
    if(tipo == 1){
        ob = new Vector();
        for( int i = 0; i < dim; i++ ){
            cout << "Ingrese valor: ";
            cin >> vi;
            ob->poneValor( vi );
        }
    }
    else{
        ob = new String();
        cout << "Ingrese palabra: ";
        cin >> st;
        ((String*)ob)->valores = new char[dim+1];
        strncpy( ((String*)ob)->valores, st.c_str(), sizeof(((String*)ob)->valores));
        ((String*)ob)->valores[dim]=0;
        ((String*)ob)->tamReal = st.size();
    }
    cout << "Ingrese el radio: ";
    cin >> r;
    while( r < 0 ){
        cout << "Radio de consulta no válido. Ingrese el radio: ";
        cin >> r;
    }
    ob->distanciaAcumulada = r;
    return ob;
}

string submenuHacerConsultaArchivoEntrada(){
    string nombre;
    getline(cin, nombre);
    cout << "Ingrese nombre del archivo de entrada: ";
    getline( cin, nombre );
    return nombre;
}

string submenuHacerConsultaArchivoSalida(){
    string nombre;
    getline(cin, nombre);
    cout << "Ingrese nombre del archivo de salida de consulta: ";
    getline( cin, nombre );
    return nombre;
}

void escribeRespuesta( vector<Objeto*> resp, ofstream& archivo ){
    //cout << resp.size() << " ";
    //cout << resp[0]->valores.size() << endl;
    int i, j;
    if(tipo == 1){
        for( i = 0; i < resp.size(); i++ ){
            for( j = 0; j < ((Vector*)resp[i])->sizeValores; j++)
                archivo << ((Vector*)resp[i])->valores[j] << " ";
            archivo << endl;
        }
    }
    else{
        for( i = 0; i < resp.size(); i++ ){
            for( j = 0; j < ((String*)resp[i])->tamReal; j++)
                archivo << ((String*)resp[i])->valores[j];
            archivo << endl;
        }
    }

    /*
    for(vector<Objeto*>::iterator i = resp.begin(); i != resp.end(); ++i ){
        for(vector<double>::iterator j = (*i)->valores.begin(); j != (*i)->valores.end(); ++j ){
            archivo << (*j) << " ";
        }
        archivo << endl;
    }
    */
}

bool verificaClustersBin(){
    struct stat buffer;   
    return (stat ("clusters.bin", &buffer) == 0);
}