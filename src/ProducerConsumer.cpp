#include "ProducerConsumer.h"
#include <signal.h>

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *buffer;
int p_cnt = 0;
int c_cnt = 0;
int p_sleep;
int c_sleep;
int bound;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//also see instruction for implementation
	buffer = new BoundedBuffer(items);
	for(int i = 0; i < p; i++) {
		pthread_t tid;
		pthread_create(&tid, NULL, producer, (void *)tid);
	}
	for(int i = 0; i < c; c++) {
		pthread_t tid;
		pthread_create(&tid, NULL, consumer, (void *)tid);
	}
	p_sleep = psleep;
	c_sleep = csleep;
	bound = items;
}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
	while(true) {
		if(p_cnt + c_cnt > bound) {
			pthread_join((pthread_t)threadID, NULL);
			pthread_detach((pthread_t)threadID);
		}
		sleep(p_sleep);
		int data = rand() % 10 + 1;
		pthread_mutex_lock(&lock);
		buffer->append(data);
		p_cnt++;
		pthread_mutex_unlock(&lock);
	}
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
	while(true) {
		if(p_cnt + c_cnt > bound) {
			pthread_join((pthread_t)threadID, NULL);
			pthread_detach((pthread_t)threadID);
		}
		sleep(c_sleep);
		pthread_mutex_lock(&lock);
		buffer->remove();
		c_cnt--;
		pthread_mutex_unlock(&lock);
	}
}
