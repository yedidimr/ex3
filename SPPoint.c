/*
 * SPPoint.c
 *
 *  Created on: 22 במאי 2016
 *      Author: elisheva
 */
#include "SPPoint.h"
#include <stdlib.h>
#include <assert.h>



struct sp_point_t{
	int index;
	int dim;
	double* coordinates;
};

	SPPoint spPointCreate(double* data, int dim, int index){

		SPPoint point;
		double* coorData;
		int i;



		// check validation of parameters
		if (data == NULL) {
			return NULL;
		}
		if (dim <= 0) {
			return NULL;
		}
		if (index < 0) {
			return NULL;
		}

		point = (SPPoint)malloc(sizeof(*point));
		if (point == NULL){
			return NULL;
		}

		// allocate memory for coorData
		coorData = (double*)malloc(dim*sizeof(double));
		if (coorData == NULL){
			return NULL;
		}

		//get data
		for (i=0; i<dim; i++){
			coorData[i] = data[i];
		}


		// initializing point arguments
		point->index = index;
		point->dim = dim;
		point->coordinates = coorData; //TODO: do like in lec3

		return point;
		}

	SPPoint spPointCopy(SPPoint source){
		SPPoint copiedPoint;
		// assertions
		assert (source != NULL);

		copiedPoint = spPointCreate(source->coordinates, source->dim, source->index);
		if (copiedPoint==NULL){
			return NULL;
		}
		return copiedPoint;
	}

	void spPointDestroy(SPPoint point){
		//if point is NULL nothing happens
		if (point==NULL){
			return;
		}
		if (point->coordinates!=NULL){
			free(point->coordinates);
		}
		free(point);
	}

	int spPointGetDimension(SPPoint point){
		assert (point != NULL);
		return point->dim;
	}

	int spPointGetIndex(SPPoint point){
		assert (point != NULL);
		return point->index;
	}

	double spPointGetAxisCoor(SPPoint point, int axis){
		assert(point!=NULL && axis < point->dim);
		return point->coordinates[axis];
	}

	double spPointL2SquaredDistance(SPPoint p, SPPoint q){
		double sum;   // Holds temporary sum
		double pVal;  // holds current p_i value
		double qVal;  // holds current q_i value
		double pqSub; // holds current p_i-q_i value
		int dim;      // dim of p and q
		int i;

		assert(p!=NULL && q!=NULL && p->dim == q->dim);

		dim = p->dim;
		for (i=0; i<dim; i++){
			pVal = spPointGetAxisCoor(p, i); // get current p_i value
			qVal = spPointGetAxisCoor(q, i); // get current q_i value
			pqSub = pVal-qVal;
			sum += (pqSub*pqSub);
		}
		return sum;
	}















