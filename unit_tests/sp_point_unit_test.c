#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>



//check if create works in general
static bool pointBasicCreateTest() {
	double data[5] = {0.0, 1.0, 2.0, 3.0, 4.0};
	int dim = 5;
	int index = 7;
	int i;
	// create point
	SPPoint p = spPointCreate(data, dim, index);
	// check point was created correctly
	ASSERT_TRUE(spPointGetIndex(p) == index);
	ASSERT_TRUE(spPointGetDimension(p) == dim);
	for (i = 0; i < dim; i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == data[i]);
	}
	spPointDestroy(p);
	return true;
}

//check create works when length(Data)>dim
static bool pointCreateDataBigrDimTest() {
	double data[7] = {0.1, 1.2, 2.3, 3.9, 4.0, 5.6, 6.3};
	int dim = 5;
	int index = 7;
	int i;
	// create point
	SPPoint p = spPointCreate(data, dim, index);
	// check point was created correctly
	ASSERT_TRUE(spPointGetIndex(p) == index);
	ASSERT_TRUE(spPointGetDimension(p) == dim);
	for (i = 0; i < dim; i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == data[i]);
	}
	spPointDestroy(p);
	return true;
}

//check create returns NULL when data==Null
static bool pointCreateDataNullTest() {
	int dim = 5;
	int index = 7;
	SPPoint p = spPointCreate(NULL, dim, index);
	ASSERT_TRUE(p == NULL);
	spPointDestroy(p);
	return true;
}

//check create returns NULL when dim=0
static bool pointCreateDimZeroTest() {
	double data[9] = {87.5, 42.13, 0.1, 1.2, 2.3, 3.9, 4.0, 5.6, 6.3};
	int dim = 0;
	int index = 3;
	SPPoint p = spPointCreate(data, dim, index);
	ASSERT_TRUE(p == NULL);
	spPointDestroy(p);
	return true;
}


//check create returns NULL when index==-1
static bool pointCreateIndexNegTest() {
	double data[9] = {87.5, 42.13, 0.1, 1.2, 2.3, 3.9, 4.0, 5.6, 6.3};
	int dim = 9;
	int index = -1;
	SPPoint p = spPointCreate(data, dim, index);
	ASSERT_TRUE(p == NULL);
	spPointDestroy(p);
	return true;
}


//Checks if spPointCopy Works
static bool pointBasicCopyTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);
	SPPoint q = spPointCopy(p);
	ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(q));
	ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(q));
	for (int i = 0; i < spPointGetDimension(p); i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == spPointGetAxisCoor(q, i));
	}
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}

//Checks if spPointGetDimension Works
static bool pointGetDimentionTest() {
	double data[5] = {0.0, 1.0, 2.0, 3.0, 4.0};
	int dim = 5;
	int index = 3;
	SPPoint p = spPointCreate(data, dim, index);
	ASSERT_TRUE(spPointGetDimension(p) == dim);
	spPointDestroy(p);
	return true;
}

//Checks if spPointGetIndex Works
static bool pointGetIndexTest() {
	double data[5] = {4.0, 3.0, 2.0, 1.0, 1.0};
	int dim = 5;
	int index = 3;
	SPPoint p = spPointCreate(data, dim, index);
	ASSERT_TRUE(spPointGetIndex(p) == index);
	spPointDestroy(p);
	return true;
}

//Checks if spPointGetAxisCoor Works
static bool pointGetAxisCoorTest() {
	double data[5] = {0.0, 1.0, 2.0, 3.0, 4.0};
	int dim = 5;
	int index = 3;
	int i;
	//create points
	SPPoint p = spPointCreate(data, dim, index);
	// check spPointGetAxisCoor works for every i
	for (i = 0; i < dim; i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == data[i]);
	}
	spPointDestroy(p);
	return true;
}

//Checks if spPointGetAxisCoor Works also when length(data)>dim
static bool pointGetAxisCoorBigDataTest() {
	double data[7] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
	int dim = 5;
	int index = 3;
	SPPoint p = spPointCreate(data, dim, index);
	for (int i = 0; i < dim; i++) {
			ASSERT_TRUE(spPointGetAxisCoor(p, i) == data[i]);
	}
	spPointDestroy(p);
	return true;
}

//spPointL2SquaredDistance: check point with itself is 0, check point with different point is not 0
static bool pointBasicL2Distance() {
	double data1[2] = { 1.0, 1.0 };
	double data2[2] = { 1.0, 0.0 };
	int dim1 = 2;
	int dim2 = 2;
	int index1 = 1;
	int index2 = 1;
	SPPoint p = spPointCreate((double *)data1, dim1, index1);
	SPPoint q = spPointCreate((double *)data2, dim2, index2);
	ASSERT_TRUE(spPointL2SquaredDistance(p,p) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q,q) == 0.0);
	ASSERT_FALSE(spPointL2SquaredDistance(p,q) == 0.0);
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}

//spPointL2SquaredDistance: check  spPointL2SquaredDistance(p,q)==spPointL2SquaredDistance(q,p)
static bool pointBasicL2DistanceDifPlaces() {
	double data1[7] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
	double data2[7] = {87.5, 42.13, 0.1, 1.2, 2.3, 3.9, 4.0};
	int dim1 = 7;
	int dim2 = 7;
	int index1 = 1;
	int index2 = 2;
	SPPoint p = spPointCreate((double *)data1, dim1, index1);
	SPPoint q = spPointCreate((double *)data2, dim2, index2);
	ASSERT_TRUE(spPointL2SquaredDistance(p,q) == spPointL2SquaredDistance(q,p));
	ASSERT_FALSE(spPointL2SquaredDistance(p,q) == 0.0);
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}

//spPointL2SquaredDistance: check  distance between two point with same data is 0
static bool pointBasicL2DistanceTwoPointsSameData() {
	double data[7] = {87.5, 42.13, 0.1, 1.2, 2.3, 3.9, 4.0};
	int dim = 7;
	int index = 2;
	SPPoint p = spPointCreate((double *)data, dim, index);
	SPPoint q = spPointCreate((double *)data, dim, index);
	ASSERT_TRUE(spPointL2SquaredDistance(p,q) == 0.0);
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}


//check NULL case of spPointDestroy
static bool pointDestroyTest() {
	spPointDestroy(NULL);
	return true;
}

int main() {
	RUN_TEST(pointBasicCreateTest);
	RUN_TEST(pointCreateDataBigrDimTest);
	RUN_TEST(pointCreateDataNullTest);
	RUN_TEST(pointCreateDimZeroTest);
	RUN_TEST(pointCreateIndexNegTest);
	RUN_TEST(pointBasicCopyTest);
	RUN_TEST(pointGetDimentionTest);
	RUN_TEST(pointGetIndexTest);
	RUN_TEST(pointGetAxisCoorTest);
	RUN_TEST(pointGetAxisCoorBigDataTest);
	RUN_TEST(pointBasicL2Distance);
	RUN_TEST(pointBasicL2DistanceDifPlaces);
	RUN_TEST(pointBasicL2DistanceTwoPointsSameData);
	RUN_TEST(pointDestroyTest);
	return 0;
}
