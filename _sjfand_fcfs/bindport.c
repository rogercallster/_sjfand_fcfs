#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <sys/wait.h>         /*  for waitpid()             */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include "bindport.h"         /*  local header              */

#define LISTENQ          (1024)


/* this file takes SERVER_PORT 
 * Creates socket
 * Binds the socket to IP
 * Returns the listening variable
 */
int socket_set(int SERVER_PORT)
{
	int    listen_var, conn;
	pid_t  pid;
	struct sockaddr_in servaddr;


	/*  Create socket  */

	if ( (listen_var = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		QUIT_DUE_TO_ERROR("Couldn't create listening socket.");


	/*  Populate socket address structure  */

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERVER_PORT);


	/*  Assign socket address to socket  */ 

	if ( bind(listen_var, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
		QUIT_DUE_TO_ERROR("Couldn't bind listening socket.");


	/*  Make socket a listening socket  */

	if ( listen(listen_var, LISTENQ) < 0 )
		QUIT_DUE_TO_ERROR("Call to listen failed.");

	fprintf(stdout, "Port number is %d\n", ntohs(servaddr.sin_port));	
	return listen_var;
}
