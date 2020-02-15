/* This is the main file      */
/*Private file to be included*/
#include "myhttpd.h"	


main (argc, argv)
	int argc;
	const char *const *argv;
{
	extern 	int	debug ;
	extern char HOMEDIR[1024];
  extern char LOG_FILE[1024];
  memset( LOG_FILE ,'\0',sizeof(LOG_FILE) );
     
	int P0RT=8080;   //Global Vars with intialization 
	int i,listener,clientcon,sched=0,t=60,threadnum=4,count=0;		// Global Vars
	pthread_t ready_queue_worker,schedular;

	debug=1;

	for (i = 1; i < argc; i++)
	{
		if (! strcmp (argv[i],"-d"))
		{
			debug=0;
			fprintf (stdout,"main ----->");
		}
		if (! strcmp (argv[i],"-h"))
		{
			printf("\n\nmyhttpd [-d] [-h] [-I file] [-p port] [-r dir] [-t time][-n threadnum] [-s schedule]\n\n ");
			exit(0); //EXIT_SUCCESS

		}
    
    if (! strcmp (argv[i],"-l"))
		{
			i=i+1;
      
			strcpy(LOG_FILE ,(argv[i]));
     }
      
		if (! strcmp (argv[i],"-p"))
		{
			i=i+1;
			P0RT= atoi (argv[i]);
		}
		if (! strcmp (argv[i],"-t"))
		{
			i=i+1;
			t= atoi (argv[i]);
		}

		if (! strcmp (argv[i],"-n"))
		{
			i=i+1;
			threadnum= atoi (argv[i]);
		}

		if (! strcmp (argv[i],"-s"))
		{
			// schedulare  data type
			i=i+1;
			if (strcmp ((argv[i]), "SJF"))
				sched=1;
			//sched=  (argv[i]);
		}
		if (! strcmp ((argv[i] ),"-r"))
		{
			strcpy (HOMEDIR,argv[i+1]);
		}
	}
	//ptherad create to activate the readyqueue thread
	pthread_t workerthread[threadnum];
	listener=socket_set(P0RT);
	if(pthread_create(&ready_queue_worker, NULL,  (void*) start_readyqueue,(void*) listener)<0) //create ready queue
		fprintf(stdout,"problem starting ready queue");
	sleep (t);
	if(pthread_create(&schedular, NULL,  (void*) schedule,(void*) sched) <0) //create scheduler
		fprintf(stdout,"error not able to start schedular");

	//initiallizig mutex
	extern pthread_mutex_t lock;
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		fprintf(stdout,"\n mutex init failed\n");
		return 1;
	}

	while (threadnum>0)  
	{
		if(pthread_create(&workerthread[threadnum], NULL,  (void*) create_worker_thead,(void*) threadnum)<0) //create ready queue
		{
			fprintf(stdout,"problem starting new worker thread");
		}
		else 
		{
			threadnum--;
			count++;
		}
    sleep (10);
	}  

	//fprintf(stdout,"created ready queue and schedular thread waiting to join \n");


	pthread_join(ready_queue_worker,NULL);
	pthread_join(schedular,NULL);

	while (count>0)
	{
		pthread_join(workerthread[count],NULL); 
		count--;
	}
	pthread_mutex_destroy(&lock);
	return 0;
}


