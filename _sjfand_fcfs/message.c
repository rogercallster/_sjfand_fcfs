#include <stdio.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include "header.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#ifndef SERVER 
#define SERVER "MYHTTPSERVER V1.0"
#endif 



#define      is_space(x)           isspace((int)(x))
char ipstr[INET6_ADDRSTRLEN];

//fprintf(stdout, " inside message.c <-\n\n\n\n");
///////////////////////////////////////////////////////////////////////////////////////
struct address_size * find_size(char *add)
{

	char path[1024];
	extern char HOMEDIR[1024];
	char HOME[1024],directory[1024];
	struct address_size *new_return_var;
	//check for ~ ----->
	int x=0;
  new_return_var = (struct address_size*) malloc(sizeof (  struct address_size));
  memset (new_return_var->address,'\0',strlen(new_return_var->address));
  new_return_var->size=0;
	if (add[1]=='~')
	{
		strcpy(HOME,"/home/");
		while (x<=strlen(add))
			//strcat (HOME,add);
		{add[x]=add[x+1];
			x++;
		}
		x=0;
		if (add[0]=='~')
		{
			while (x<=strlen(add))
				//strcat (HOME,add);
			{add[x]=add[x+1];
				x++;
			}
			x=0;
		}
		//printf("\naddress is ~ for %s\n\n",add);
		strcat(HOME,add);
		strcat (HOME,"/myhttpd/");
		//fprintf(stdout,"HOME  PATH IS with ~ of course  ----> %s<--\n\n",HOME);
	}

	else
	{
		
		//fprintf(stdout,"HOME DIR PATH IS ----> %s < --",HOMEDIR);
		
		strcpy(HOME,HOMEDIR);
		//fprintf(stdout,"HOME  PATH IS ----> %s<--",HOME);
		if (strlen (HOME) < 2 )
			if (getcwd(HOME, sizeof(HOME)) != NULL)
				fprintf(stdout,"CURRENT PATH IS ----> %s\n",HOME);  
			else
				perror("getcwd() error");

		strcat (HOME,add);
	}

	char *p;
	int i=0;
	int file=0;
	int  dir=0;
	long size=1;
	struct stat ser_buf;
	//fprintf(stdout,"new path is %s\n\n\n\n",HOME);
	if( lstat( HOME, &ser_buf ) == -1 ) 
	{
		 printf("neither a file nor dir:\n\n\n\n");
		//perror(HOME);
    new_return_var->size=0;
    strcpy(new_return_var->address,"404");
    return  new_return_var;
	}
	else  
	{
		if( S_ISDIR( ser_buf.st_mode ) )
		{  
			memset (directory,'\0',strlen(directory));

			strcat(directory,HOME);
			//search index.html
			strcat(HOME,"/index.html");
			//fprintf(stdout," HOME++++/index.html -->%s<--\n\n\n\n",HOME);
			if( lstat( HOME, &ser_buf ) == -1 )
			{
				perror(HOME);
			}  

			if( S_ISREG(ser_buf.st_mode))
			{
				//search for index.html
				file=1;
				//fprintf(stdout,"\n\nfile found in dir as index.html\n\n\n\n\n\n");
			}
			/////END 
			else
			{
				//directory;
				dir=1;

			}
			//search for file
		}
		else file=1;
	}


	strcpy (add,HOME);
	/*REINITALIZING CHARECTER STRING */
	while (strlen (HOME))
		HOME[(strlen(HOME))-1]='\0';
	//printf("HOME DIR AFTER REINITIALIZATION IS => %s <- and HOME IS NOW %s and strcpy to add is--> %s<--",HOMEDIR,HOME,add) ;
	///////////////////////////////////////////////

//	printf("HOME DIR COPIED TO ADDRESS SO AADRESS IS -->%s",add);
	if(file)
	{
		struct stat buf;

		stat(add, &buf);
		size = buf.st_size+1;

		//fprintf(stdout,"size of current file is %d\n\n",size);
		///////////////////////////
		new_return_var->size=size;
		strcpy(new_return_var->address,add);
	//	fprintf(stdout,"\nFile address is >%s \n and return new_return var line 138 <\n",new_return_var->address);

		return new_return_var;

	}

	else 
	{

		if (dir)
		{
			new_return_var->size=1;
			memset (new_return_var->address,'\0',strlen(new_return_var->address));
			strcpy(new_return_var->address,directory);
			//printf("\nline no 134 message .c file was not there hence dir is searched and will be list afor path %s\n",directory);
			return new_return_var;

		}

	}
	//check the address is file or directory

	//if dir check if dir has index.html change 
	//if file search file and serve size



}



/*end of find_size*/
int get_line(int client, int size, char *buff)
{
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(client, &c, 1, 0);
		//code
		if (n > 0)
		{
			if (c == '\r')
			{
				n = recv(client, &c, 1, MSG_PEEK);

				if ((n > 0) && (c == '\n'))
					recv(client, &c, 1, 0);
				else
					c = '\n';
			}
			buff[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	buff[i] = '\0';

	return(i);
}
/*END OF getline*/


/*  client_IP_address*/
char *client_IP_address(int socket)
{
socklen_t len;
struct sockaddr_storage addr;

int port;

len = sizeof addr;
getpeername(socket, (struct sockaddr*)&addr, &len);

// for  with both IPv4 and IPv6:
if (addr.ss_family == AF_INET) {
    struct sockaddr_in *socket = (struct sockaddr_in *)&addr;
    port = ntohs(socket->sin_port);
    inet_ntop(AF_INET, &socket->sin_addr, ipstr, sizeof ipstr);
} else { // IPV6 format
    struct sockaddr_in6 *socket = (struct sockaddr_in6 *)&addr;
    port = ntohs(socket->sin6_port);
    inet_ntop(AF_INET6, &socket->sin6_addr, ipstr, sizeof ipstr);
}

//printf("/n/n-----------Peer IP address: %s\n", ipstr);
//printf("Peer port      : %d\n", port);

return ipstr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*return size and file path with correction*/
///////////////////////////////////////////////////////////////////////////////////////////////////////
struct address_size  *message_type (int client,int listener)
{
	int countchar;
	char buffer[1024];
	char path[512];
	char method[255];
	size_t  i, j;
	struct  stat st;
	char add[1024];
	long s=1;
  //char client_ip[INET6_ADDRSTRLEN];
  
  
	struct address_size *return_var,*new_return_var;
	return_var = (struct address_size*) malloc(sizeof (  struct address_size));
	i = 0; j = 0;
	return_var->size=0;
   
	countchar = get_line(client, sizeof(buffer),buffer);

/*Geting information of the client*/  
 

 // printf ("ip address got is ***** >>>%s<<<\n\n\n\n\n",return_var->LOG);
/*end of the call find details in function */

	while (!is_space(buffer[j]) && (i < sizeof(method) - 1))
	{
		method[i] = buffer[j];
		i++; j++;
	}
	method[i] = '\0';

//printf ("\n\n\n\n\n\n******************buffer string is >%s<*******************\n\n",buffer);
 
	//if ----------------->   inside get
	i = 0;
	//get address of file
	while ( is_space(buffer[j] )  && (j <  sizeof(buffer))) 
		j++;

	while (!is_space(buffer[j]) && (j< sizeof(buffer)) && (i<sizeof(add)-1))  
	{
		return_var->address[i]=buffer[j];
		//fprintf(stdout, "%c",return_var->address[i]);
		i++;
		j++;
		return_var->address[i]='\0';
	}  
	if (!strcasecmp(method, "GET"))
	{
		//inside get flag
	//	fprintf(stdout,"Got GET request  in the detection\n");
		return_var=find_size(return_var->address);
		//fprintf(stdout,"size of file is  %d",s);

	}
	//fprintf(stdout,"ready for return %d",return_var->size);



	if (!strcasecmp(method, "HEAD"))
	{
		//fprintf(stdout,"Got HEAD request  in the detection\n");
		s=0;
	}
	i=0;
	while(return_var->address[i]!='\0')
	{
		//fprintf(stdout, "%c",return_var->address[i]);
		i++;
	}
	//
	//	return_var->size=s;
 // fprintf(stdout, "\n\n==++client_IP_address(listener) = %s\n",client_IP_address(listener));
strcpy (return_var->LOG, client_IP_address(listener));
 // fprintf(stdout, " inside message.c <-   \n return_var->LOG = %s\n",return_var->LOG);
  time ( &rawtime );
        ptm=gmtime(&rawtime);
  sprintf(return_var->LOG+strlen(return_var->LOG)," - [%d/%s/%d :%2d:%2d:%2d -0600] ",ptm->tm_mday,MONTH[ptm->tm_mon],(ptm->tm_year+1900),ptm->tm_hour,ptm->tm_min,ptm->tm_sec );
  sprintf(return_var->LOG+strlen(return_var->LOG)+strlen(return_var->LOG) ,buffer);
  
   //fprintf(stdout,"%s and buffer is buffer %s",return_var->LOG,buffer);
  //fprintf(stdout,"\n<-||||||||||||||||||||||||||");
  strcpy(return_var->BUFFER,buffer);
  //memset(log,'\0',strlen(log));
	return  return_var;
  }
/*end of --message_type */



/*SERVICE FOR HEAD REQUEST STARTS HERE*/
int _head(int client,char *filename,char *log,int len)
{
	   char buf[1024];
     
           sprintf(buf+strlen(buf),"HTTP/1.0 200 OK\r\n");
  //send(client, buf, strlen(buf), 0);

  //time
  
  
  time ( &rawtime );
  ptm=gmtime(&rawtime);
  sprintf(buf+strlen(buf),"Date GMT %d/%s/%d :%2d:%2d:%2d\r\n ",ptm->tm_mday,MONTH[ptm->tm_mon],(ptm->tm_year+1900),ptm->tm_hour,ptm->tm_min,ptm->tm_sec );
        //sprintf(execute->log+strlen(execute->log),execute->buffer);
  sprintf (buf+strlen (buf), "Server HTTPD SERVER \r\n");
  //      send(client, buf, strlen(buf), 0);
    //last modified
  sprintf(buf+strlen(buf), "Last-Modified  GMT 31/Oct/2013 :20:19: 9\r\n");
       // send(client, buf, strlen(buf), 0);
        sprintf(buf+strlen(buf), "Content-Type: text/html or image/gif\r\n");
        //send(client, buf, strlen(buf), 0);
  //content  length
  sprintf(buf+strlen(buf), "Content-Length  %d \r\n",len);
        //send(client, buf, strlen(buf), 0);
        sprintf(buf+strlen(buf),"\r\n");
        send(client, buf, strlen(buf), 0);
     /*
        //(void)filename;  // could use filename to determine file type 
//printf("/n-------------------------------------------------------------- log at end of headis%s<----n",log);
        strcpy(buf, "HTTP/1.0 200 OK\r\n");
  send(client, buf, strlen(buf), 0);

  //time
  time ( &rawtime );
  ptm=gmtime(&rawtime);
  sprintf(buf,"Date GMT %d/%s/%d :%2d:%2d:%2d\r\n ",ptm->tm_mday,MONTH[ptm->tm_mon],(ptm->tm_year+1900),ptm->tm_hour,ptm->tm_min,ptm->tm_sec );
        send(client, buf, strlen(buf), 0);
        strcpy(buf, "Server HTTPD SERVER \r\n");
        send(client, buf, strlen(buf), 0);
    //last modified
  strcpy(buf, "Last-Modified  GMT 31/Oct/2013 :20:19: 9\r\n");
        send(client, buf, strlen(buf), 0);
        sprintf(buf, "Content-Type: text/html or image/gif\r\n");
        send(client, buf, strlen(buf), 0);
  //content  length
  sprintf(buf, "Content-Length  %d \r\n",len);
        send(client, buf, strlen(buf), 0);
        strcpy(buf, "\r\n");
        send(client, buf, strlen(buf), 0);
*/

	return 0;
}
/*end of _head the HEAD service*/

/*serve get request */
void _serve_get(char *address,int clientconn ,char *log,int len)
{

	char data_to_send[1024];
	int fd,bytes_read; 

  
	if ( (fd=open(address, O_RDONLY))!=-1 ) //file search success
	{
		send(clientconn, "HTTP/1.0 200 OK\n\n", 17, 0);
		// HEAD REQUEST SERVICE
    _head(clientconn,address,log,len);

		while ( (bytes_read=read(fd, data_to_send, 1024))>0 )

			write (clientconn, data_to_send, strlen(data_to_send));
		memset (data_to_send,'\0',sizeof(data_to_send));

	}
	strcpy(data_to_send, "\r\n");
	send(clientconn, data_to_send, strlen(data_to_send), 0);
	//return 0;
}

/*endserve _serve_get*/


void _dir_list_serve(char *address,int clientconn,char *log,int len)
{
	struct dirent **list;
	int n,i;
	char buf[4096];

	//printf("line 302 inside_dir_list_serve message.c-address here is  %s",address);
	//execute->address;
  //FOR HEAD REQUEST
  	send(clientconn, "HTTP/1.0 200 OK\n\n", 17, 0);
	_head(clientconn,address,log,0);
 // sleep(1);
	n = scandir(address, &list, 0, alphasort);
	if (n < 0)
	{ perror("scandir");

		send(clientconn, "FILE NOT FOUND ERROR 404\r\n", 29, 0);
	}
	else {
		for (i = 0; i < n; i++){
      if (list[i]->d_name[0] != '.' && list[i]->d_name[0]!=  '.')
		{	
    strcat (buf, list[i]->d_name);
			send(clientconn, buf, strlen(buf), 0);
      
			memset(buf,'\0',strlen(buf));
			strcpy(buf, "\n");
      }
			send(clientconn, buf, strlen(buf), 0);


			// strcpy(buf, "\r\n");       
		}
		send(clientconn, "\r\n",4,0);

		//printf("%s\n", buf);
		//free(namelist);


	}

}
