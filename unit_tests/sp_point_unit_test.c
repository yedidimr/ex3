#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>


//TODO: do we need to check destroy? if so, how?

//check if create works
static bool pointBasicCreateTest() {
	double data1[5] = {0.0, 1.0, 2.0, 3.0, 4.0};
	double data2[7] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
	int dim = 5;
	int index = 5;
	int i;
	//create points
	SPPoint p1 = spPointCreate(data1, dim, index);
	SPPoint p2 = spPointCreate(data2, dim, index);
	SPPoint p3 = spPointCreate(NULL, dim, index);
	SPPoint p4 = spPointCreate(data1, 0, index);
	SPPoint p5 = spPointCreate(data1, dim, -1);
	//check create works in general
	ASSERT_TRUE(spPointGetIndex(p1) == index);
	ASSERT_TRUE(spPointGetDimension(p1) == dim);
	for (i = 0; i < dim; i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p1, i) == data1[i]);
	}
	//check create works when length(Data)>dim
	ASSERT_TRUE(spPointGetIndex(p2) == index);
	ASSERT_TRUE(spPointGetDimension(p2) == dim);
	for (i = 0; i < dim; i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p2, i) == data2[i]);
	}
	//check create returns NULL when data==Null
	ASSERT_TRUE(p3 == NULL);
	//check create returns NULL when dim=0
	ASSERT_TRUE(p4 == NULL);
	//check create returns NULL when index==-1
	ASSERT_TRUE(p5 == NULL);
	//destroy points
	spPointDestroy(p1);
	spPointDestroy(p2);
	spPointDestroy(p3);
	spPointDestroy(p4);
	spPointDestroy(p5);
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
	ASSERT_TRUE(spPointGetDimension(p) == 5);
	spPointDestroy(p);
	return true;
}

//Checks if spPointGetIndex Works
static bool pointGetIndexTest() {
	double data[5] = {4.0, 3.0, 2.0, 1.0, 1.0};
	int dim = 5;
	int index = 3;
	SPPoint p = spPointCreate(data, dim, index);
	ASSERT_TRUE(spPointGetIndex(p) == 3);
	spPointDestroy(p);
	return true;
}

//Checks if spPointGetAxisCoor Works
static bool pointGetAxisCoorTest() {
	double data1[5] = {0.0, 1.0, 2.0, 3.0, 4.0};
	double data2[7] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
	int dim = 5;
	int index = 3;
	int i;
	//create points
	SPPoint p1 = spPointCreate(data1, dim, index);
	SPPoint p2 = spPointCreate(data2, dim, index);
	//assertions for p1
	for (i = 0; i < dim; i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p1, i) == data1[i]);
	}
	//assertions for p2, spPointGetAxisCoor works also when length(data)>dim
	for (int i = 0; i < dim; i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p2, i) == data2[i]);
	}
	spPointDestroy(p1);
	spPointDestroy(p2);
	return true;
}

//check if spPointL2SquaredDistance works
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

int main() {
	RUN_TEST(pointBasicCreateTest);
	RUN_TEST(pointBasicCopyTest);
	RUN_TEST(pointGetDimentionTest);
	RUN_TEST(pointGetIndexTest);
	RUN_TEST( pointGetAxisCoorTest);
	RUN_TEST(pointBasicL2Distance);
	return 0;
}
