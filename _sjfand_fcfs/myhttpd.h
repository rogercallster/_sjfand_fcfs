/* This is the maini header file                     */
///////////////////////////////
//
/////////////////////////////////
///*Private file to be included*/
#include <stdio.h>
#include <sys/stat.h>
#include "header.h"
#include "bindport.c"
#include "request_worker.c"
//#include "header.h" inclusion of this file create>> " error: definition of ‘struct readyqueue’
//"------->REASON its defination has alreadu been used in request_worker.c
//
//it takes the client connection and connects to server the request
//
//////Readyqueue

