/*
* converts SP_LIST_MSG to SP_BPQUEUEMSG
*/
SP_BPQUEUE_MSG convertListMsgToQueueMsg(SP_LIST_MSG msg);

typedef struct sp_bp_queue_t* SPBPQueue {
	int size; 		//the maximum capacity of the queue
	SPList head; 	// points to a SPList which will hold queues elements
};

SPBPQueue spBPQueueCreate(int maxSize){
	SPBPQueue queue;
	if (maxSize < 0) {
		return NULL;
	}
	if (maxSize == 0) {
		print_warning("creating queue of max size 0");
	}

	queue = (SPBPQueue)malloc(sizeof(*queue));
	if (queue == NULL) {
		print_error("error allocating new queue");
		return NULL;
	}
	queue->size = maxSize;
	queue->head = spListCreate();
	
	return queue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source) {
	SPBPQueue new_queue;
	if (source == NULL) {
		return NULL;
	}

	new_queue = (SPBPQueue)malloc(sizeof(*new_queue));
	if (new_queue == NULL) {
		print_error("ALLOCATION ERRorr");
		return NULL;
	}
	new_queue->size = source->size;
	if (source->head == NULL) {
		print_warning("trying to copy and NUlL pointer");
		new_queue->head = NULL;
	} else {
		new_queue->head = spListCopy(source->head);
		if (new_queue->head == NULL) {
			print_error("allocation error - error allocationg list");
		}
	}
	return new_queue;
}
void spBPQueueDestroy(SPBPQueue source){
	if (source != NULL) {
		if (source->head != NULL) {
			spListDestroy(source->head);
		}
		free(source);
	}
	else {
		print_warning("didnt destroy null pointer");
	}
}
void spBPQueueClear(SPBPQueue source) {
	SP_LIST_MSG msg = spListClear(source->head);
	if (msg == SP_LIST_NULL_ARGUMENT) {
		print_warning("try to clear an NULL pointer");
	} 
	else {
		print_info("cleared queue successfully");
	}

}

int spBPQueueSize(SPBPQueue source){
	return spListGetSize(source->head);
}

int spBPQueueGetMaxSize(SPBPQueue source) {
	if (source == NULL){
		return -1;
	}
	return source->size;
}



SP_BPQUEUE_MSG convertListMsgToQueueMsg(SP_LIST_MSG msg) {
	if (msg == SP_LIST_OUT_OF_MEMORY) {
		return SP_BPQUEUE_OUT_OF_MEMORY;
	}
	else if ((msg == SP_LIST_INVALID_CURRENT) ||(msg == SP_LIST_NULL_ARGUMENT)) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	else if (msg == SP_LIST_SUCCESS) {
		return SP_BPQUEUE_SUCCESS;
	}	
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element) {
	double newVal; // the value of the new element to enqueue
	double maxVal; // the maximum value of the queue before inserting the new element
	double eVal;   // iterates over all values in the queue
	SPListElement elementCopy; // a copy of the element
	SP_LIST_MSG msg;

	// argument validation check
	if (element == NULL || source == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;	
	}
	
	// try to copy element
	elementCopy = spListElementCopy(element);
	if (elementCopy == NULL) {
		return SP_BPQUEUE_OUT_OF_MEMORY;
	}

	// if queue is empty insert the element as first element
	if (spBPQueueIsEmpty(source)){
		msg = spListInsertFirst(source->head, elementCopy);
		return convertListMsgToQueueMsg(msg);
	} 

	newVal = SPListElement(element);
	maxVal = spBPQueueMaxValue(source);
	// if new value to insert is bigger than the maximum value - insert it to the end of the queue if it's not full
	if (newVal >= maxVal) {
		// if queue is full don't insert the new element
		if (spBPQueueIsFull(source)) {
			return SP_BPQUEUE_FULL;
		}
		// if queue is not full - insert new element to the end of the queue
		else {
			msg = spListInsertLast(source->head, elementCopy);
			return convertListMsgToQueueMsg(msg);
		}
	}
	
	// insert new element in the right place in queue
  	SP_LIST_FOREACH(SPListElement, e, list) {
  		eVal = spListElementGetValue(e);
		
		// if we didn't try to insert already
		// and the current value is bigger than the new value
		// than insert the new value before the current value
		if ((msg != NULL) && (newVal < eVal)) {
			msg = spListInsertBeforeCurrent(source->head, elementCopy);
			// return in case of failure
			// keep iterating till the end of the queue in case of success insert
			if (msg != SP_LIST_SUCCESS) {
				return convertListMsgToQueueMsg(msg);
			}
		}
	}

	// at this point, iterator is at the end of the queue
	// if the queue has more elementes than it's maximum size - remove last element from the queue
	if (spBPQueueSize(source) > spBPQueueGetMaxSize(source)) {
		msg = spListRemoveCurrent(source->head); // removes the last element in the queue
	}

	return convertListMsgToQueueMsg(msg);
}


SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source) {
	SPListElement currentElement;
	SP_LIST_MSG msg ;

	if (source == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;	
	}
	// there is nothing to dequqe if queue is empty
	if (spBPQueueIsEmpty(source)) {
		return SP_BPQUEUE_EMPTY;
	}
	// set current iterator to the first element and remove it 
	// (first element in the queue is the element with the minimum value)
	currentElement = spListGetFirst(source->head);
	return convertListMsgToQueueMsg(spListRemoveCurrent(source->head));
}

SPListElement spBPQueuePeek(SPBPQueue source) {
	SPListElement currentElement;
	if (source == NULL) {
		return NULL;
	}
	currentElement = spListGetFirst(source->head);
	return spListElementCopy(currentElement);
}

SPListElement spBPQueuePeekLast(SPBPQueue source) {
	SPListElement currentElement;
	if (source == NULL) {
		return NULL;
	}
	currentElement = spListGetFirst(source->head);
	if (currentElement == NULL) {
		return NULL;
	}
	while (currentElement) {
		currentElement = spListGetNext(source->head);
	}

	return spListElementCopy(currentElement);
}

double spBPQueueMinValue(SPBPQueue source) {
	return spListElementGetValue(spListGetFirst(source->head));
}


double spBPQueueMaxValue(SPBPQueue source) {
	SPListElement currentElement = spListGetFirst(source->head);
	if (currentElement == NULL) {
		return NULL;
	}
	while (currentElement) {
		currentElement = spListGetNext(source->head);
	} 
	return spListElementGetValue(currentElement);
}


bool spBPQueueIsEmpty(SPBPQueue source) {
	return spListGetSize(source->head) == 0;
}

bool spBPQueueIsFull(SPBPQueue source) {
	return source->size == spListGetSize(source->head);
}

