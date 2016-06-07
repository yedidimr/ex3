#include "../SPBPriorityQueue.h"
#include "../SPList.h"
#include "unit_test_util.h"
#include <stdbool.h>

// test queue creation failure
static bool spBPQueueCreateFailTest(){
	// create with negative queue size
	SPBPQueue source = spBPQueueCreate(-1);
	ASSERT_TRUE(source == NULL);
	spBPQueueDestroy(source);
	
	return true;
}

// test queue creation success
static bool spBPQueueCreateSuccessTest(){
	SPBPQueue source = spBPQueueCreate(1);
	ASSERT_TRUE(source != NULL);
	spBPQueueDestroy(source);
	return true;
}

// test queue copy 
static bool spBPQueueCopyTest(){
	SPBPQueue source = spBPQueueCreate(3); 
	SPBPQueue copy;

	// insert 3 elements to the queue
	SPListElement element1 = spListElementCreate(1,1.0);
	SPListElement element2 = spListElementCreate(2,2.0);
	SPListElement element3 = spListElementCreate(3,3.0);
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element1));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element2));
	
	// create a copy
	copy = spBPQueueCopy(source);

	// check that new queue size is the same as the original queue size
	// if the size is the same we can assume the elemets are the same since that copy is done by SPList
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == spBPQueueGetMaxSize(copy));
	ASSERT_TRUE(spBPQueueSize(source) == spBPQueueSize(copy));

	// test that if you change one queue it does not affect the copy queue
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueDequeue(source));
	ASSERT_TRUE(spBPQueueSize(source) != spBPQueueSize(copy));

	spBPQueueDestroy(source);
	spBPQueueDestroy(copy);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	return true;
}

// test queue clear
static bool spBPQueueClearTest() {
	SPBPQueue source = spBPQueueCreate(3);
	// create queue and insert 3 elements 
	SPListElement element1 = spListElementCreate(1,1.0);
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_SUCCESS);
	// clear queue and check queue max size (should not change) and queue size (should be 0)
	spBPQueueClear(source);
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == 3);
	ASSERT_TRUE(spBPQueueSize(source) == 0);
	ASSERT_TRUE(spBPQueueIsEmpty(source));

	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	return true;	
}

// test queue size
static bool spBPQueueSizeTest() {
	SPBPQueue source = spBPQueueCreate(2);
	SPListElement element1 = spListElementCreate(1,1.0);
	
	// queue size is 0 before insertion
	ASSERT_TRUE(spBPQueueSize(source) == 0);
	// queue size is 1 after one insertion
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(source) == 1);
	// queue size is 2 after two insertions
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueSize(source) == 2);
	// queue size is 2 after three insertions (since max size of queue is 2)
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_FULL);
	ASSERT_TRUE(spBPQueueSize(source) == 2);
	
	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	return true;
}

// test queue max size.
static bool spBPQueueGetMaxSizeTest() {
	SPBPQueue source = spBPQueueCreate(3);
	SPListElement element1 = spListElementCreate(1,1.0);
	// check max size does not change with element enqueuing
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == 3);
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(source, element1) == SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueGetMaxSize(source) == 3);

	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	return true;
}

// test enqueue with invalid arguments
static bool spBPQueueEnqueueInvailArgTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	SPListElement element1 = spListElementCreate(1,1.0);
	// enqueue to null queue
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueEnqueue(NULL, element1));
	// enqueue with null element
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueEnqueue(source, NULL));
	// enqueue with null element and null queue
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueEnqueue(NULL, NULL));
	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	return true;
}


// test enqueue to full queue
static bool spBPQueueEnqueueFullTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	SPListElement element1 = spListElementCreate(1,1.0);
	SPListElement element2 = spListElementCreate(2,2.0);
	SPListElement element3 = spListElementCreate(3,3.0);
	SPListElement element4 = spListElementCreate(4,4.0);
	

	// enqueue to empty queue
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element1));
	// enqueue to not empty and not full queue
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source,  element3));
	// enqueue to full queue - bigger value
	ASSERT_TRUE(SP_BPQUEUE_FULL == spBPQueueEnqueue(source, element4));
	// enqueue to full queue - ok value
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element2));
	
	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	spListElementDestroy(element4);
	return true;
}


// test enqueue success when inserting elements in descending order
static bool spBPQueueEnqueueSuccessDescendingOrderTest() {
	SPBPQueue source = spBPQueueCreate(3); 
	SPListElement element1 = spListElementCreate(1,1.0);
	SPListElement element2 = spListElementCreate(2,2.0);
	SPListElement element3 = spListElementCreate(3,3.0);
	
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element2));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element1));
	
	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	return true;
}

// test enqueue success when inserting elements in ascending order
static bool spBPQueueEnqueueSuccessAscendingOrderTest() {
	SPBPQueue source = spBPQueueCreate(3); 
	SPListElement element1 = spListElementCreate(1,1.0);
	SPListElement element2 = spListElementCreate(2,2.0);
	SPListElement element3 = spListElementCreate(3,3.0);
	
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element1));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element2));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3));
	
	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	return true;
}


// test enqueue success when inserting elements in no order
static bool spBPQueueEnqueueSuccessNoOrderTest() {
	SPBPQueue source = spBPQueueCreate(3); 
	SPListElement element1 = spListElementCreate(1,1.0);
	SPListElement element2 = spListElementCreate(2,2.0);
	SPListElement element3 = spListElementCreate(3,3.0);
	
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element1));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element2));
	
	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	return true;
}

// test dequeue with invalid argument
static bool spBPQueueDequeueInvalidArgTest() {
	// test dequeue from null pointer
	ASSERT_TRUE(SP_BPQUEUE_INVALID_ARGUMENT == spBPQueueDequeue(NULL));
	return true;
}
// test dequeue from empy queue
static bool spBPQueueDequeueEmptyTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	ASSERT_TRUE(SP_BPQUEUE_EMPTY == spBPQueueDequeue(source));
	spBPQueueDestroy(source);
	return true;
}

static bool spBPQueueDequeueSuccessTest() {
	SPBPQueue source = spBPQueueCreate(1); 
	SPListElement element1 = spListElementCreate(1,1.0);
	// test OK dequeue 
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element1));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueDequeue(source));
	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	return true;
}


// test peek fron null queue
static bool spBPQueuePeekFromNullTest(){
	SPListElement peek_element = spBPQueuePeek(NULL);
	ASSERT_TRUE(NULL == peek_element);
	spListElementDestroy(peek_element);
	return true;
}

// test peek from empty queue
static bool spBPQueuePeekFromEmptyTest(){
	SPBPQueue source = spBPQueueCreate(2); 
	SPListElement peek_element = spBPQueuePeek(source);
	ASSERT_TRUE(NULL == peek_element);
	spBPQueueDestroy(source);
	spListElementDestroy(peek_element);
	return true;
}

static bool spBPQueuePeekSuccessTest(){
	SPListElement peek_element;
	SPListElement smallest_element = spListElementCreate(2,2.0);
	SPListElement element3 = spListElementCreate(3,3.0);
	SPBPQueue source = spBPQueueCreate(2); 

	// insert elements to queue
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, smallest_element));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3));
	// peek 
	peek_element = spBPQueuePeek(source);
	// check element peek is the smallest equal
	ASSERT_TRUE(spListElementCompare(peek_element, smallest_element) == 0);
	// test copy element did really made a copy 
	spListElementSetIndex(smallest_element, 4);
	ASSERT_TRUE(spListElementCompare(peek_element, smallest_element) != 0);

	spBPQueueDestroy(source);
	spListElementDestroy(peek_element);
	spListElementDestroy(element3);
	spListElementDestroy(smallest_element);
	return true;
}

// test peek last from null queue
static bool spBPQueuePeekLastFromNullTest(){
	SPListElement peek_element = spBPQueuePeek(NULL);
	ASSERT_TRUE(NULL == spBPQueuePeekLast(NULL));
	spListElementDestroy(peek_element);
	return true;
}


// test peek last from empty queue
static bool spBPQueuePeekLastFromEmptyTest(){
	SPBPQueue source = spBPQueueCreate(2);
	SPListElement peek_element = spBPQueuePeekLast(source);
	
	ASSERT_TRUE(NULL == peek_element);

	spBPQueueDestroy(source);
	spListElementDestroy(peek_element);
	return true;
}

// test peek last succession
static bool spBPQueuePeekLastSuccessTest(){
	SPListElement element;
	SPListElement biggest_element = spListElementCreate(2,2.0);
	SPListElement element1 = spListElementCreate(1,1.0);
	SPBPQueue source = spBPQueueCreate(2);

	// insert elements to queue
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, biggest_element));
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element1));
	// peek 
	element = spBPQueuePeekLast(source);
	// check element peek is the biggest element equal
	ASSERT_TRUE(spListElementCompare(element, biggest_element) == 0);
	// test copy element did really made a copy 
	spListElementSetIndex(biggest_element, 4);
	ASSERT_TRUE(spListElementCompare(element, biggest_element) != 0);

	spBPQueueDestroy(source);
	spListElementDestroy(element);
	spListElementDestroy(element1);
	spListElementDestroy(biggest_element);
	return true;
}

// test get min value from emprt queue
static bool spBPQueueMinValueEmptyTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	// test minValue on empty queue
	ASSERT_TRUE(-1.0 == spBPQueueMinValue(source));
	spBPQueueDestroy(source);
	return true;
}

// test get min value
static bool spBPQueueMinValueSuccessTest() {
	SPListElement element1 = spListElementCreate(1,1.0);
	SPListElement element2 = spListElementCreate(2,2.0);
	SPListElement element3 = spListElementCreate(3,3.0);
	SPBPQueue source = spBPQueueCreate(3); 
	// insert 1 element to queue - it should be the min value
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element2));
	ASSERT_TRUE(2.0 == spBPQueueMinValue(source));

	// insert one more element (smaller one) (2 in total) - it should be the minimum value
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element1));
	ASSERT_TRUE(1.0 == spBPQueueMinValue(source));

	// insert one more element (bigger one) (2 in total) - minimum value should remain as before
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3));
	ASSERT_TRUE(1.0 == spBPQueueMinValue(source));
	
	spBPQueueDestroy(source);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	return true;
}

// test get max value from emptyt queue
static bool spBPQueueMaxValueEmptyTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	ASSERT_TRUE(-1.0 == spBPQueueMaxValue(source));
	spBPQueueDestroy(source);
	return true;
}

static bool spBPQueueMaxValueSuccessTest() {
	SPListElement element2 = spListElementCreate(2,2.0);
	SPListElement element3 = spListElementCreate(3,3.0);
	SPListElement element3_2 = spListElementCreate(3,2.0);
	SPBPQueue source = spBPQueueCreate(2);

	// insert 1 element to queue - it should be the max value
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element2));
	ASSERT_TRUE(2.0 == spBPQueueMaxValue(source));

	// insert one more element (bigger one) (2 in total) - it should be the max value
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3));
	ASSERT_TRUE(3.0 == spBPQueueMaxValue(source));
	
	// insert one more element (smaller value one) (2 in total) - it should be the max value
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3_2));
	ASSERT_TRUE(2.0 == spBPQueueMaxValue(source));

	spBPQueueDestroy(source);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	spListElementDestroy(element3_2);
	return true;
}


// test isEmpty on empty queue
static bool spBPQueueIsEmptyTrueTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	ASSERT_TRUE(spBPQueueIsEmpty(source));
	spBPQueueDestroy(source);
	return true;
}

// test is empty on non-empty queue
static bool spBPQueueIsEmptyFalseTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	SPListElement element3_2 = spListElementCreate(3,2.0);
	
	// insert element and check if is empty
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3_2));
	ASSERT_FALSE(spBPQueueIsEmpty(source));

	spBPQueueDestroy(source);
	spListElementDestroy(element3_2);
	return true;
}

// test isFull on full queue
static bool spBPQueueIsFullTrueTest() {
	SPListElement element3_2 = spListElementCreate(3,2.0);
	SPBPQueue source = spBPQueueCreate(1); 
	
	// fill the queue and check if it's full
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3_2));
	ASSERT_TRUE(spBPQueueIsFull(source));
	
	spBPQueueDestroy(source);
	spListElementDestroy(element3_2);
	return true;
}



// test isFull on non-full (and not-empty because that the assertion) queue
static bool spBPQueueIsFullFalseTest() {
	SPBPQueue source = spBPQueueCreate(2); 
	SPListElement element3_2 = spListElementCreate(3,2.0);
	ASSERT_TRUE(SP_BPQUEUE_SUCCESS == spBPQueueEnqueue(source, element3_2));

	ASSERT_FALSE(spBPQueueIsFull(source));

	spBPQueueDestroy(source);
	spListElementDestroy(element3_2);
	return true;
}

int main() {
	// creation tests
	RUN_TEST(spBPQueueCreateFailTest);
	RUN_TEST(spBPQueueCreateSuccessTest);

	// copy test
	RUN_TEST(spBPQueueCopyTest);
	// clear test
	RUN_TEST(spBPQueueClearTest);

	// sizes tests
	RUN_TEST(spBPQueueSizeTest);
	RUN_TEST(spBPQueueGetMaxSizeTest);

	// enqueue tests
	RUN_TEST(spBPQueueEnqueueInvailArgTest);
	RUN_TEST(spBPQueueEnqueueFullTest);
	RUN_TEST(spBPQueueEnqueueSuccessAscendingOrderTest);
	RUN_TEST(spBPQueueEnqueueSuccessDescendingOrderTest);
	RUN_TEST(spBPQueueEnqueueSuccessNoOrderTest);
	
	// dequeue tests
	RUN_TEST(spBPQueueDequeueInvalidArgTest);
	RUN_TEST(spBPQueueDequeueEmptyTest);
	RUN_TEST(spBPQueueDequeueSuccessTest);


	// test peek first element
	RUN_TEST(spBPQueuePeekFromNullTest);
	RUN_TEST(spBPQueuePeekFromEmptyTest);
	RUN_TEST(spBPQueuePeekSuccessTest);

	// test peek last element
	RUN_TEST(spBPQueuePeekLastFromNullTest);
	RUN_TEST(spBPQueuePeekLastFromEmptyTest);
	RUN_TEST(spBPQueuePeekLastSuccessTest);

	// test get min value
	RUN_TEST(spBPQueueMinValueEmptyTest);
	RUN_TEST(spBPQueueMinValueSuccessTest);

	// test get max value
	RUN_TEST(spBPQueueMaxValueEmptyTest);
	RUN_TEST(spBPQueueMaxValueSuccessTest);

	// test isEmpty
	RUN_TEST(spBPQueueIsEmptyTrueTest);
	RUN_TEST(spBPQueueIsEmptyFalseTest);
	
	// test is full
	RUN_TEST(spBPQueueIsFullTrueTest);
	RUN_TEST(spBPQueueIsFullFalseTest);
	return 0;
}
