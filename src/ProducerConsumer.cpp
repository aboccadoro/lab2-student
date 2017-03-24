#include "ProducerConsumer.h"
#include <time.h>
#include <cstdint>

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *buffer;
time_t timer;
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
	p_sleep = psleep;
	c_sleep = csleep;
	bound = items;
	for(int i = 0; i < p; i++) {
		cout << "creating process thread " << i << '\n';
		pthread_t tid;
		pthread_create(&tid, NULL, producer, (void *)(intptr_t)i);
		cout << "process " << i << " finished creating\n";

	}
	for(int k = 0; k < c; k++) {
		cout << "creating consumer thread " << k << '\n';
		pthread_t tid;
		pthread_create(&tid, NULL, consumer, (void *)(intptr_t)k);
		cout << "consumer " << k << " finished creating\n";
	}
}

void* producer(void* threadID) {
	//TODO: producer thread, see instruction for implementation
	while(true) {
		cout << "producing\n";
		if(p_cnt == bound) pthread_join((pthread_t)threadID, NULL);
		usleep(p_sleep*1000);
		int data = rand() % 10 + 1;
		pthread_mutex_lock(&lock);
		cout << "producer locked\n";
		buffer->append(data);
		p_cnt++;
		fstream file;
		file.open("output.txt", fstream::out);
		cout << "Producer #" << threadID << ", time = " << time(&timer) << ", data item #" << p_cnt << " , item value=" << data << '\n';
		pthread_mutex_unlock(&lock);
		cout << "consumer unlocked\n";
	}
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
	while(true) {
		cout << "consuming\n";
		if(c_cnt == bound) pthread_join((pthread_t)threadID, NULL);
		usleep(c_sleep*1000);
		pthread_mutex_lock(&lock);
		cout << "consumer locked";
		int data = buffer->remove();
		c_cnt++;
		fstream file;
		file.open("output.txt", fstream::out);
		cout << "Consumer #" << threadID << ", time = " << time(&timer) << ", data item #" << c_cnt << " , item value=" << data << '\n';
		pthread_mutex_unlock(&lock);
		cout << "consumer unlocked\n";
	}
}