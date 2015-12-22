
#include "MSvector.h"
#include <stdlib.h>
#include <math.h>

using namespace std;


MSvector::MSvector(){
	size = 0;
	capacity = 0;
	this->arr = NULL;//(double*) malloc(sizeof(double)*1);
}

MSvector::MSvector( const MSvector& orig ){

}

MSvector::~MSvector(){
	if(arr)
		delete[] arr;
}

void MSvector::putValue( double val ){

	double* arr2;
	
	if( this->size == 0 ){
		this->size = 1;
		this->capacity = 1;
		this->arr = (new double[capacity]);//(double*)malloc(sizeof(double)*capacity);
		this->arr[0] = val;
		return;
	}
	if( this->size < this->capacity ){
		this->arr[size] = val;
	}
	else{
		this->capacity = this->size*2;
		arr2 = (new double[capacity]); //= (double*)malloc( sizeof(double) * capacity );

		if( arr2 != NULL){
			for( int i = 0; i < this->size; i++){
				arr2[i] = arr[i];
			}
			delete[] this->arr;
			this->arr = arr2;
			this->arr[size] = val;
		}
		else{
			cout << "No pudo agregarse" << endl;
			return;
		}
	}
	this->size++;
	//delete arr2;
	return;
}

void MSvector::deleteValue( int pos ){
	if( pos >= this->size ) return;
	else{
		for( int i = pos; i < (this->size-1); i++){
			this->arr[i] = this->arr[i+1];
		}
		this->arr[this->size-1] = -1; 
		this->size--;
	}
}
double MSvector::getValueAt( int pos ){
	if( pos >= size ) return NULL;
	else return this->arr[pos];
}

int MSvector::getSize(){
	return this->size;
}

int MSvector::getCapacity(){
	return this->capacity;
}