#include "ProducerConsumer.h"

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *buffer;
ofstream file;
time_t timer;
int p_cnt = 0;
int c_cnt = 0;
int p_sleep;
int c_sleep;
int bound;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t datal = PTHREAD_MUTEX_INITIALIZER;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//also see instruction for implementation
	file.open("output.txt");
  	file << "";
  	file.close();
	buffer = new BoundedBuffer(items);
	p_sleep = psleep;
	c_sleep = csleep;
	bound = items;
	pthread_t pthreads[p];
	for(int i = 0; i < p; i++) {
		void* ptr = malloc(sizeof(int));
    	*((int*)ptr) = i + 1;
		pthread_create(&pthreads[i], NULL, producer, ptr);
	}
	pthread_t cthreads[c];
	for(int k = 0; k < c; k++) {
		void* ptr = malloc(sizeof(int));
    	*((int *)ptr) = k + 1;
		pthread_create(&cthreads[k], NULL, consumer, ptr);
	}
	for(int i = 0; i < p; i++) {
		pthread_join(pthreads[i], NULL);
	}
	for(int k = 0; k < c; k++) {
		pthread_join(cthreads[k], NULL);
	}
}

void* producer(void* threadID) {
	//TODO: producer thread, see instruction for implementation
	while(true) { 
		usleep(p_sleep*1000);
		pthread_mutex_lock(&datal);
		int data = rand();
		pthread_mutex_unlock(&datal);
		buffer->append(data);
		pthread_mutex_lock(&lock);
		if(p_cnt == bound) {
			pthread_mutex_unlock(&lock);
			pthread_exit(NULL);
		}
		p_cnt++;
		file.open("output.txt", std::ios_base::app);
		file << "Producer #" << threadID << ", time = " << clock() << ", data item #" << p_cnt << " , item value=" << data << '\n';
		file.close();
		pthread_mutex_unlock(&lock);
	}
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
	while(true) {
		usleep(c_sleep*1000);
		int data = buffer->remove();
		pthread_mutex_lock(&lock);
		if(c_cnt == bound) {
			pthread_mutex_unlock(&lock);
			pthread_exit(NULL);
		}
		c_cnt++;
		file.open("output.txt", std::ios_base::app);
		file << "Consumer #" << threadID << ", time = " << clock() << ", data item #" << c_cnt << " , item value=" << data << '\n';
		file.close();
		pthread_mutex_unlock(&lock);
	}
}