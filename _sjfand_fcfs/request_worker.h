//#include "print_error.c"
#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>
#include "header.h"

struct readyqueue
{
	struct readyqueue *next;
	long priority;
	int  clientconn;
  char *address;
  char *log;
  char *buffer;
	struct readyqueue *prev;
};



