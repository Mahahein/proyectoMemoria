#ifndef MSVECTOR_H
#define	MSVECTOR_H
#include <vector>
#include <iostream>
#include <list>
using namespace std;

class MSvector {
public:
    double* arr;
    int size;
    int capacity; 
    MSvector();
    MSvector(const MSvector& orig);
    virtual ~MSvector();
    void putValue(double val);
    void deleteValue(int pos);
    double getValueAt(int pos);
    int getSize();
    int getCapacity();

private:

};

#endif	/* MSVECTOR_H */
