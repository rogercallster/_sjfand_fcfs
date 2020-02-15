/*file is to print standard error on console
 *
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

//#include "helper.h"


/*  Prints an error message and quits  */

void QUIT_DUE_TO_ERROR(char const * msg) {
	fprintf(stderr, "WEBSERV: %s\n", msg);
	exit(EXIT_FAILURE);
}

