#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <stdbool.h>

//Checks if copy Works

bool spBPQueueCreateTest(){
	
	SPBPQueue source;
	// create with negative queue size
	SPBPQueue source = spBPQueueCreate(-1);
	ASSERT_TRUE(source == NULL);
	spBPQueueDestroy(source);
	
	return true;
}
bool spBPQueueCopyTest(){
	SPBPQueue source = spBPQueueCreate(3); 
	SPBPQueue copy ;
	spBPQueueEnqueue(source, spListElementCreate(1,1.0));
	spBPQueueEnqueue(source, spListElementCreate(2,2.0));
	spBPQueueEnqueue(source, spListElementCreate(3,3.0));
	// create copy
	copy = spBPQueueCopy(source);
	// compare each element in the queue
	SP_LIST_FOREACH(SPListElement, e, source->head) {
		ASSERT_TRUE(spListElementCompare(spListElementGetValue(copy), spListElementGetValue(source)) == 0);
	}
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == spBPQueueGetMaxSize(copy));
	ASSERT_TRUE(spBPQueueSize(source) == spBPQueueSize(copy));
	// test that if you change one queue it's not affect other queue
	spBPQueueDequeue(source);
	ASSERT_TRUE(spBPQueueSize(source) != spBPQueueSize(copy));

	spBPQueueDestroy(source);
	spBPQueueDestroy(copy);
	return true;
}

bool spBPQueueClearTest() {

	SPBPQueue source = spBPQueueCreate(3);
	spBPQueueEnqueue(source, spListElementCreate(1,1.0));
	spBPQueueEnqueue(source, spListElementCreate(2,2.0));
	spBPQueueEnqueue(source, spListElementCreate(3,3.0));
	spBPQueueClear(source);
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == 3);
	ASSERT_TRUE(spBPQueueSize(source) == 0);
	spBPQueueDestroy(source);
	return true;	
}


bool spBPQueueSizeTest() {

	SPBPQueue source = spBPQueueCreate(2);
	ASSERT_TRUE(spBPQueueSize(source) == 0);
	spBPQueueEnqueue(source, spListElementCreate(1,1.0));
	ASSERT_TRUE(spBPQueueSize(source) == 1);
	spBPQueueEnqueue(source, spListElementCreate(2,2.0));
	ASSERT_TRUE(spBPQueueSize(source) == 2);
	spBPQueueEnqueue(source, spListElementCreate(3,3.0));
	ASSERT_TRUE(spBPQueueSize(source) == 2);
	spBPQueueDestroy(source);
}


bool spBPQueueGetMaxSizeTest() {
// check queue size
	SPBPQueue source = spBPQueueCreate(3);
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == 3);
	spBPQueueEnqueue(source, spListElementCreate(1,1.0));
	spBPQueueEnqueue(source, spListElementCreate(2,2.0));
	spBPQueueEnqueue(source, spListElementCreate(3,3.0));
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == 3);
	spBPQueueDestroy(source);
}

bool spBPQueueEnqueueTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	// enqueue to null queue
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueEnqueue(NULL, spListElementCreate(1,1.0)));
	// enqueue with null element
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueEnqueue(source, NULL);
	// enqueue with null element and null queue
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueEnqueue(NULL, NULL);
	// enqueue to empty queue
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, spListElementCreate(1,1.0)));
	// enqueue to not empty and not full queue
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, spListElementCreate(2,2.0)));
	// enqueue to full queue - bigger value
	ASSERT_TRUE(SP_BPQUEUE_FULL == spBPQueueEnqueue(source, spListElementCreate(3,3.0));
	// enqueue to full queue - ok value
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, spListElementCreate(1,1.5)));
	spBPQueueDestroy(source);
}

bool spBPQueueDequeueTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	// test dequeue from null pointer
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueDequeue(NULL));
	// test dequeue from empty queue
	ASSERT_TRUE(SP_BPQUEUE_EMPTY == spBPQueueDequeue(source));
	// test OK dequeue 
	spBPQueueEnqueue(source, spListElementCreate(1,1.0));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueDequeue(source));
	// test dequeue from empty queue
	ASSERT_TRUE(SP_BPQUEUE_EMPTY == spBPQueueDequeue(source));
	// test OK dequeue different senario
	spBPQueueEnqueue(source, spListElementCreate(1,1.0));
	spBPQueueEnqueue(source, spListElementCreate(2,2.0));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueDequeue(source));

	spBPQueueDestroy(source);
}

bool spBPQueuePeekTest{
	SPListElement element;
	SPListElement smallest_element = spListElementCreate(1,1.0);
	SPBPQueue source = spBPQueueCreate(2); 
	//peek from NULL queue
	ASSERT_TRUE(NULL == spBPQueuePeek(NULL));
	// peek from empty queue
	ASSERT_TRUE(NULL == spBPQueuePeek(source));
	// insert elements to queue
	spBPQueueEnqueue(source, smallest_element);
	spBPQueueEnqueue(source, spListElementCreate(2,2.0));
	// peek 
	element = spBPQueuePeek(source);
	// check elements equal
	ASSERT_TRUE(spListElementCompare(element, smallest_element) == 0);
	// test copy element did really made a copy 
	spListElementSetIndex(smallest_element, 4);
	ASSERT_TRUE(spListElementCompare(element, smallest_element) != 0);

	spBPQueueDestroy(source);
	spListElementDestroy(element);
	spListElementDestroy(smallest_element);
}


bool spBPQueuePeekLastTest{
	SPListElement element;
	SPListElement biggest_element = spListElementCreate(2,2.0);
	SPBPQueue source = spBPQueueCreate(2); 
	//peek from NULL queue
	ASSERT_TRUE(NULL == spBPQueuePeekLast(NULL));
	// peek from empty queue
	ASSERT_TRUE(NULL == spBPQueuePeekLast(source));
	// insert elements to queue
	spBPQueueEnqueue(source, biggest_element);
	spBPQueueEnqueue(source, spListElementCreate(1,1.0));
	// peek 
	element = spBPQueuePeekLast(source);
	// check elements equal
	ASSERT_TRUE(spListElementCompare(element, biggest_element) == 0);
	// test copy element did really made a copy 
	spListElementSetIndex(biggest_element, 4);
	ASSERT_TRUE(spListElementCompare(element, biggest_element) != 0);

	spBPQueueDestroy(source);
	spListElementDestroy(element);
	spListElementDestroy(biggest_element);
}


bool spBPQueueMinValueTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	// test minValue on empty queue
	ASSERT_TRUE(-1.0 == spBPQueueMinValue(source));
	spBPQueueEnqueue(source, spListElementCreate(2,2.0));
	// test different scenarios
	ASSERT_TRUE(2.0 == spBPQueueMinValue(source));
	spBPQueueEnqueue(source, spListElementCreate(1,1.0));
	ASSERT_TRUE(1.0 == spBPQueueMinValue(source));
	spBPQueueEnqueue(source, spListElementCreate(0,0.0));
	ASSERT_TRUE(0.0 == spBPQueueMinValue(source));
	
	spBPQueueDestroy(source);
}



bool spBPQueueMaxValueTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	// test maxValue on empty queue
	ASSERT_TRUE(-1.0 == spBPQueueMaxValue(source));
	spBPQueueEnqueue(source, spListElementCreate(2,2.0));
	// test different scenarios
	ASSERT_TRUE(2.0 == spBPQueueMaxValue(source));
	spBPQueueEnqueue(source, spListElementCreate(3,3.0));
	ASSERT_TRUE(3.0 == spBPQueueMaxValue(source));
	spBPQueueEnqueue(source, spListElementCreate(4,4.0));
	ASSERT_TRUE(3.0 == spBPQueueMaxValue(source));
	spBPQueueEnqueue(source, spListElementCreate(3,2.0));
	ASSERT_TRUE(2.0 == spBPQueueMaxValue(source));
	spBPQueueDestroy(source);
}


bool spBPQueueIsEmptyTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	ASSERT_TRUE(spBPQueueIsEmpty(source));
	spBPQueueEnqueue(source, spListElementCreate(3,2.0));
	ASSERT_FALSE(spBPQueueIsEmpty(source));
	spBPQueueDestroy(source);
};


bool spBPQueueIsFullTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	ASSERT_FALSE(spBPQueueIsFull(source));
	spBPQueueEnqueue(source, spListElementCreate(3,2.0));
	ASSERT_FALSE(spBPQueueIsFull(source));
	spBPQueueEnqueue(source, spListElementCreate(2,3.0));
	ASSERT_TRUE(spBPQueueIsFull(source));
	spBPQueueDestroy(source);
}

int main() {
	RUN_TEST(spBPQueueCreateTest);
	RUN_TEST(spBPQueueCopyTest);
	RUN_TEST(spBPQueueClearTest);
	RUN_TEST(spBPQueueSizeTest);
	RUN_TEST(spBPQueueGetMaxSizeTest);
	RUN_TEST(spBPQueueEnqueueTest);
	RUN_TEST(spBPQueueDequeueTest);
	RUN_TEST(spBPQueuePeekTest);
	RUN_TEST(spBPQueuePeekLastTest);
	RUN_TEST(spBPQueueMaxValueTest);
	RUN_TEST(spBPQueueMinValueTest);
	RUN_TEST(spBPQueueIsEmptyTest);
	RUN_TEST(spBPQueueIsFullTest);
	return 0;
}
