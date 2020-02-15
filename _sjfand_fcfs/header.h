//global headers
//
#ifndef HEADER_H
#define HEADER_H



#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <malloc.h>


char HOMEDIR[1024];
char LOG_FILE[1024];

int     debug ;
pthread_mutex_t lock;

struct address_size
{
	long size;
	char address[1024];
  char LOG[1024];
  char BUFFER[1024];
};

 char *MONTH[12]= {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
 time_t rawtime;
 struct tm * ptm;

/*
   struct readyqueue

   {
   struct readyqueue *head;
   int priority;
   char *msg;
//augmentation required
struct readyqueue *tail;
};*/

#endif
