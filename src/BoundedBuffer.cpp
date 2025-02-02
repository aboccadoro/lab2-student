#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int N){
	//TODO: constructor to initiliaze all the varibales declared in BoundedBuffer.h
	buffer = new int[N];
	buffer_size = N;
	buffer_cnt = 0;
	buffer_last = 0;
	buffer_lock = PTHREAD_MUTEX_INITIALIZER;
	buffer_full = PTHREAD_COND_INITIALIZER;
	buffer_empty = PTHREAD_COND_INITIALIZER;
}


void BoundedBuffer::append(int data){
	//TODO: append a data item to the circular buffer
	pthread_mutex_lock(&buffer_lock);
	while(buffer_cnt == buffer_size) {
		pthread_cond_wait(&buffer_full, &buffer_lock);
	}
	buffer[buffer_last] = data;
	buffer_last = (buffer_last + 1) % buffer_size;
	buffer_cnt++;
	pthread_cond_broadcast(&buffer_empty);
	pthread_mutex_unlock(&buffer_lock);
}

int BoundedBuffer::remove(){
	//TODO: remove and return a data item from the circular buffer
	pthread_mutex_lock(&buffer_lock);
	while(isEmpty()) {
		pthread_cond_wait(&buffer_empty, &buffer_lock);
	}
	int i = (buffer_last - buffer_cnt-- + buffer_size) % buffer_size;
	int data = buffer[i];
	pthread_cond_broadcast(&buffer_full);
	pthread_mutex_unlock(&buffer_lock);
	return data;
}

bool BoundedBuffer::isEmpty(){
	//TODO: check is the buffer is empty
	return buffer_cnt == 0;
}