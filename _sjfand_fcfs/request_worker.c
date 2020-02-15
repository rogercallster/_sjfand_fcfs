/*
   This file contains ->
   ---------------------
   a) Queueing thread source
   b) Schedular thread source
   c) Execution thread source 
   d)
   */


#include "request_worker.h"
#include "message.c"



extern int debug;
extern pthread_mutex_t lock;
struct readyqueue *head,*newnode,*head_sched;
int arrival_lock=0;
extern char LOG_FILE[1024];
//struct Queue Q = {.front = 0, .rear = 0};

void*  start_readyqueue(int listener)
{
	fprintf(stdout,"inside ready queue thread\n\n");

	// flag to see we are in function
	//fprintf(stdout,"request_worker.c  %d",listener);
	int clientcon=-1,pri=0;
	struct address_size *prior;
	struct readyqueue *tail;

	head=(struct readyqueue*) malloc (sizeof (struct readyqueue)); //<<- allocation of mem to header
	head->next=NULL;//header head is null
	head->prev=NULL;//header tail is null
	head->priority=pri;//header priority contains no of request in linked list
	head->clientconn=0;   //msg is actually nothing
	head->address = NULL;

	//	fprintf (stdout,"debug == %d \n",debug);// 0 
	while(1)
	{
		if ( (clientcon = accept(listener, NULL, NULL)) < 0 )
			QUIT_DUE_TO_ERROR("Error calling accept()");
		//ideally it should return a structure to make things imple
		prior=message_type (clientcon,listener);
		newnode=(struct readyqueue*) malloc (sizeof (struct readyqueue));
		head->priority++;

		newnode->next=NULL;
		newnode->priority=prior->size;
		newnode->clientconn=clientcon;
		newnode->address=prior->address;
    newnode->log=prior->LOG;
    newnode->buffer=prior->BUFFER;
		fprintf(stdout ,"\nnow got line no 54 request_worker #### return   head-> priority = - %d  and  size here  %d \n\n LOG is %s \n\n address is >>%s<<",head->priority,newnode->priority,newnode->log,newnode->address);

		//flag to check address inside 
		int i=0;//flag print 
    //fprintf(stdout, "\nwhile loop \n");
	/*	while(newnode->address[i]!='\0')
		{
			fprintf(stdout, "%c",prior->address[i]);
			i++;
		}
		fprintf(stdout, " <-\n\n\n\n");
*/



		if (head->next==NULL)
		{
		//	fprintf(stdout ," \ninside if condition  pri = %d\n",pri);
			head->next=newnode;
			newnode->prev=head;


		}     

		else
		{
			//fprintf(stdout,"\n /ninside else of linkedlist\n");
			newnode->prev=tail;
			tail->next=newnode;

		}
		tail=newnode;

		//fprintf (stdout,"flag3\n");
	}
	//d=debug;
	prior=0;//Can cause error
	//            fprintf (stdout,"flag5\n d== %d  and debug is %d\n",d,debug);
}
/*scheduler thread starts here
//    if(pthread_create(&schedular, NULL,  (void*) schedule,(void*) listener)<0)
*/

/*end of queuing thread*/

/*
   The scheduling policy to be used will be set via the [–s sched] option when myhttpd server is
   first started. The available policies are First Come First Serve (FCFS) and Shortest Job First
   (SJF). When SJF scheduling policy is selected, you can use the file size information as the job
   length for scheduling purposes, assuming serving larger files will take longer. The scheduler
   thread will choose one of the requests in the ready queue according to the scheduling policy
   selected. The request will then be assigned to one of the available execution threads for service.
   There will be no scheduling done during the first t seconds after the myhttpd server is started.
   This time period will be used to accumulate some requests in the ready queue.
   */
void* schedule(int sched)
{
	fprintf(stdout,"inside schedular thread\n\n");
	head_sched=(struct readyqueue*) malloc (sizeof (struct readyqueue)); //<<- allocation of mem to header
	head_sched->next=NULL;//header head is null
	head_sched->prev=NULL;//header tail is null
	head_sched->priority=0;//header priority contains no of request in linked list
	head_sched->clientconn=0;   //msg is actually nothing
	head_sched->address=NULL;
	printf("hello world\n\n");
	//fcfs = sched=0 
	//sjf  = sched =1


	struct readyqueue *temp,*adjustment, *sjf,*i ;
	int x=1;
	while (1)
	{

		if (head->next==NULL || head_sched->next!=NULL )
		{
			while (x==1)
			{
				if (head->next!=NULL && head_sched->next==NULL )
					x=0;
			
			}
      x=1;
			//fprintf(stdout,"line numberis 101 and inside head->next = NULL\n");
		}
		fprintf(stdout,"line numberis 141 and inside head->next = NULL**&&--------\n");

		if(sched==0)//fcfs
		{
			//fprintf (stdout ,"line number is 107 and inside head->next = NULL\n");

			if(head->next->next==NULL)
			{
				//fprintf(stdout,"inside sched FCFS where head->next->next==NULL\n");

				temp=head->next;
        head->next=NULL;
        temp->prev=head_sched;
				head_sched->next=temp;
        temp->next=NULL;
        			fprintf(stdout,"insi desched FCFS where head->next->next==NULL out of 155\n");
              

			}
			//fprintf(stdout,"insi desched FCFS where head->next->next==NULL out of 118\n");
       if(head->next!=NULL)
       {
			if(head->next->next!=NULL)
			{
				//	fprintf(stdout,"inside sched FCFS where head->next->next !=   NULL >>>>the else condition \n\n");
				temp=head->next;
				head->next=temp->next;
//adjustment=
				temp->next->prev=head;
				temp->prev=head_sched;
				head_sched->next=temp;
        temp->next=NULL;
                			fprintf(stdout,"insi desched FCFS where head->next->next==NULL out of 170\n %s\n\n\n\n",head_sched->next->address);
}
			}
      
		}
  //  printf("head->sched-next->address %s \n\n",head_sched->next->address );
		//printing the array in schedular
	//	i=head_sched;
		//fprintf(stdout, " head-> priority is --%d\n\n\n\n",head->priority);
//sleep (10);
 //  if(head_sched->next!=NULL){
	/*	while (i->next!=NULL)
		{
		i=i->next;
		//fprintf(stdout, " inside scheduler client connection is >%d< and address is >%s< and alos priority is > %d<\n\n",i->clientconn,i->address,i->priority);

		}
    */
  //  }
		
		//	fprintf(stdout, " end of loop ");
		//	sleep (2);
		// still to be filled ->
    
    int min=99999;
		if (sched == 1) //sjf
		
    {	printf("hello world now the sched is sjf");
       if (head->next)
{       
    if (head->next->next!=NULL)
    {
     sjf=head->next;
    //sjf
    while(sjf->next!=NULL) 
		{
     if(min > sjf->priority)
     min = sjf->priority;
     sjf=sjf->next; 
     }
     //while itteration for min fin
     printf("\nmin ----->%d\n",min);
     while(sjf->priority > min )
     {
        
        sjf=sjf->prev;
        if(head== sjf->prev)
        continue;
     }
     
     //sjf=sjf->prev;
 
     head_sched->next=sjf;
     sjf->prev->next=sjf->next;
    
    sjf->prev=head_sched;
   sjf->next=NULL;
     
     if (sjf->next)
    { 
        sjf->next->prev=sjf->prev;
        
    }
    
   
     
		}
    //head->next finsih if cond
    else
    {
    sjf=head->next;
    sjf->next=NULL;
    sjf->prev=head_sched;
    head->next=NULL;
    head_sched->next=sjf;
    
    }
    }
    }//sjf finished

                			fprintf(stdout,"insi desched FCFS where head->next->next==NULL out of 238\n %s\n\n\n\n",head_sched->next->address);

	}
}
/*end of scheduling thread*/

/*
   The server will consist of n threads. A pool of n threads will be always ready for
   executing/serving incoming requests (n x execution threads). The number n is given as a
   parameter when you start the server (using option: -n threadnum). 
   */
//_______________________________________________________________________________________________

void* create_worker_thead(int threadnum)
{

	fprintf(stdout ,"\ninside the worker thread no %d\n\n\n ",threadnum);
  

	char data_to_send[1024];
	int fd,bytes_read; 
	struct readyqueue *execute,*execute_next;
      //  fprintf(stdout,"line 209");

  //Lock common variable  and head schedule is inside while so that 
        //fprintf(stdout,"line 209"); 

	
 // pthread_cond_t count_nonzero;
	while(1) 
	{
      while(head_sched->next==NULL)
      {
      sleep (1);
     // printf("sleeping thread no %d",threadnum);
      }
      
      
      //printf("line 216 head_sched->next= %d",head_sched->next);
      if(head_sched->next!=NULL)
      {
           printf("\n \nline 227 head_sched->next->address= %s thread no = %d",head_sched->next->address,threadnum);
        
      pthread_mutex_lock(&lock);
     // printf("line 220 head_sched->next= %d",head_sched->next);
      // printf("line 225");
      execute=head_sched->next;// head_sched
      	
       if(execute)
{
head_sched->next=NULL;
  //    if(execute->next)
//execute->next->prev=head_sched;
      //printf("line 241");
execute->next=NULL;
execute->prev=NULL;
}

       
        //     printf("line 246");
      

      pthread_mutex_unlock(&lock);
       
    printf("\n \nline 291 head_sched->next->address= %s thread no = %d",execute->address,threadnum);
       
		//execute=execute->next;

		if (execute)
		{

      if(execute->priority==0 && strcmp(execute->address,"404"))
			{
      
				//fprintf(stdout, " inside scheduler client connection is temp %d\n\n",execute->clientconn);
				
				_head(execute->clientconn,execute->address,execute->log,execute->priority);//log is also required
				close(execute->clientconn);
				
				/*
				   if(execute->next !=NULL)
				   {
				   head_sched->next=execute->next;
				   execute->next->prev=execute->prev;
				   execute->prev=NULL;
				   execute->next=NULL;
				   }
				   else 
				   {
				   head_sched->next=NULL;
				   execute->prev=NULL;
				   execute->next=NULL;
				   }
				   */
				//if(execute->address !=NULL)
				// free(execute);
			}
			if(execute->priority==1 && strcmp(execute->address,"404"))
			{ 
				fprintf(stdout,"got GET REQUEST with dir will now list dir ");
       // _head(execute->clientconn,execute->address,execute->log);
        printf ("value of arrival lock is %d",arrival_lock);
         _dir_list_serve(execute->address,execute->clientconn,execute->log,execute->priority);
        shutdown (execute->clientconn, SHUT_RDWR);
				close(execute->clientconn);
      }
      
			if(execute->priority>1 && strcmp(execute->address,"404"))
			{
      printf("\n \nline  294 head_sched->next->address= %s thread no = %d", execute->address,threadnum);
				//pthread_mutex_lock(&lock);
			//	fprintf(stdout,"got GET REQUEST WILL NOW SERVE FILE AS FILE FOUND %d request URL is %s size of request file is %d\n",execute->clientconn,execute->address,execute->priority);
				//_head(execute->clientconn,execute->address->);
      //  _head(execute->clientconn,execute->address,execute->log);
				_serve_get(execute->address,execute->clientconn,execute->log, execute->priority);         
      //   printf("\n\n----execute->log %s\n",execute->log);
				shutdown (execute->clientconn, SHUT_RDWR);
				close(execute->clientconn);

				//pthread_mutex_unlock(&lock);
           
			}
      
      else
      {
      // printf ("first 404 cautght\n\n\n\n");
      char errorbuf[128];
         strcpy(errorbuf, " error HTTP/1.0 404 \r\n");
      send(execute->clientconn, errorbuf, strlen(errorbuf), 0);
      strcpy(errorbuf, "\r\n");
      send(execute->clientconn, errorbuf, strlen(errorbuf), 0);
        shutdown (execute->clientconn, SHUT_RDWR);
				close(execute->clientconn);
        execute->priority=1;
      }
      
      

		}
	}


  
  if (execute->log!=NULL)
  
  {
  fprintf(stdout,"\n\n LOGFILE IS  >>> %s\n",LOG_FILE);
      //fprintf(stdout,"hello world new hero is herer -*&*&&*&*&*&*%s",execute->log);
  time ( &rawtime );
  ptm=gmtime(&rawtime); 
  sprintf(execute->log+strlen(execute->log),"- [%d/%s/%d :%2d:%2d:%2d -0600] ",ptm->tm_mday,MONTH[ptm->tm_mon],(ptm->tm_year+1900),ptm->tm_hour,ptm->tm_min,ptm->tm_sec );
  sprintf(execute->log+strlen(execute->log),execute->buffer);
  sprintf(execute->log+strlen(execute->log)," 200 ");
  
  sprintf(execute->log+strlen(execute->log),"%d" ,execute->priority);
        
    fprintf(stdout,"FINAL LOG AFTER HEADER IS-->> %s\n",execute->log);
    
  //execute->log=NULL;
  }
  
  
  //loging here
  
//log file
  if((strlen(LOG_FILE)>1)&& (execute->log!=NULL))
  {
  printf("log file is found\n");
  FILE* write;
  if ( write = fopen(LOG_FILE, "a+") )
  {
  fprintf(write, "%s\n",execute->log );
  fclose(write);
  }
  else QUIT_DUE_TO_ERROR("Error calling fopen wrong file part()");
 //memset(execute->log,'\0',strlen(execute->log));
}  
   
  
 //execute=NULL;
 if(execute->priority!=-1)
 {
 free (execute);
 /*
  execute->next=NULL;
  execute->log=NULL;
  execute->priority=-1;
  execute->address=NULL;
  */
  }
     printf("\n\n//// line 429\n\n");

  
  }
}
