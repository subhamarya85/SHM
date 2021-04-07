#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/shm.h>




#define TEXT_SZ 2048
#define BUFSIZE 1024

#define TRUE 1
#define FALSE 0

struct shared_use_st
{
	int data_available;
	char message[TEXT_SZ];

};


int main()
{

	int process_running = TRUE;
	void *shared_memory = (void*)0;


	struct shared_use_st *shared_stuff;
	char buffer[BUFSIZE];

	int shmid;

	shmid=shmget((key_t)1234, sizeof(struct shared_use_st),IPC_CREAT|0666);

	if(shmid== -1)
	{
		fprintf(stderr,"shmget failed\n");
		exit(EXIT_FAILURE);

	}

	shared_memory=shmat(shmid,(void*)0, 0);

	if(shared_memory == (void*)-1)
	{
		fprintf(stderr,"shmat failed\n");
		exit(EXIT_FAILURE);
	}


	printf("Memory Attached at %x\n", (int) shared_memory);

	shared_stuff = (struct shared_use_st *)shared_memory;


	while(process_running)
	{
		while(shared_stuff->data_available == TRUE)
		{

			sleep(1);
			printf("Waiting for client .........\n");
		}

		printf("Enter string : ");

		fgets(buffer,BUFSIZE,stdin);

		strncpy(shared_stuff->message, buffer, TEXT_SZ);

		shared_stuff->data_available=TRUE;

		if(strncmp(buffer, "end" ,3)== FALSE)
		{
			process_running=FALSE;
		}

	}


	if(shmdt(shared_memory)==-1)
	{
		fprintf(stderr,"shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
