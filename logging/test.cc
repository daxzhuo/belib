#include <stdio.h>
#include <pthread.h>
#include "Logger.h"
#include "AsyncLogging.h"
#define NTHREAD 4

void* logthread(void* arg) {
	for (int i = 0; i < 10000; ++i) {
		LOGDEBUG("%s", "hello world");
	}
	long id = (long)arg;
	printf("thread %ld finish\n", id);
	return NULL;
}


int
main(void)
{
	FILE* file = fopen("./log.log", "w");
	if (file == NULL) {
		perror("open file failed:");
	}
	logging::AsyncLogging al(file, 3);

	logging::Logger::setLogLevel(logging::Logger::DEBUG);
	logging::Logger::setOutput([&](const char* data, int len) { al.append(data, len);});
	
	// create background async log thread
	al.start();
	// start worker log thread
	pthread_t worker[NTHREAD];
	for (int i = 0; i < NTHREAD; ++i) {
		pthread_create(&worker[i], NULL, &logthread, (void*)i);
	}
	for (int i = 0; i < NTHREAD; ++i) {
		pthread_join(worker[i], NULL);
	}
	// sleep(2);
	al.stop();
	fclose(file);
}