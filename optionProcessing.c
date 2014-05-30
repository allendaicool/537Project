/*****************************************************
 * FILE NAME: optionProcessing.c
 *
 * Created on: Sep 14 2013
 * Author: Yihong Dai guangpu zhou
 * Professor : Barton P Miller
 * cs login: yihong
 * PURPOSE: this file reads options from command line and call other functions to
 *          check the validity of these inputs. At the end, it calls PorcListInfoGenerator
 *          function to display all the information corresponding to the options.
 *          if process id is specified, then only display the info related with such id
 *          else if process id is not specified then display the info of process associated with
 *          current usr
 *
 *****************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "ProcList.h"
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "parser.h"

/* initialize flags that will be used to check which options are in the input*/
int pFlag = 0 ;
int sFlag = 0 ;
int UFlag = 0 ;
int SFlag = 0 ;
int vFlag = 0 ;
int cFlag = 0 ;

/* format in printing */
char * Format =  ":  " ;

/* check the input in their order*/
char order [5]; /* order of the command option  counter*/
int count = 0 ; /* use "count" to track how many options are in the input */


/* check whether the character is in the system */
int hasit(char *);

/* check whether the char* points to a character type */
int IsStringOfInt(char *);

/* the ID provided in the argv[]'s second element */
char * ProcessID ;

/* main uses command-line arguments to specify a set of options
 *  The program display the information about the process
 *  and check if input is valid or not.
 * Parameters:
 *  argc:  number of arguments
 *  *argv[]:  the array of arguments
 */
int main(int argc, char *argv[])
{

	/* if the first arg is not 537ps, then it is invalid and exit*/
	if (strcmp(argv[1],"537ps") != 0)
	{
		printf("bad  command p \n");
		exit(EXIT_FAILURE);
	}
	// checkInput is initialized to 2, check if the input following the 537ps is valid or not.
	int checkInput = 2 ;

	while ( checkInput < argc)
	{
		if (argv[checkInput][0] != '-'&& strcmp(argv[checkInput - 1],"-p" ) != 0 )
		{
			printf("bad  command \n");
			exit(EXIT_FAILURE);
		}

		checkInput++ ;
	}


	// initialize the IDsize to be 50
	int IDsize = 50 ;
	ProcessID = (char *) malloc (IDsize); // allocate space for ProcessID
	int b ;
	for (b= 0 ; b< IDsize ; b++)
	{
		ProcessID[b] = '\0';

	}  // set all to null
	// opt is the input after -.
	int opt;
	/* to get all the valid option command, record the corresponding to
	 * be 1 and check if duplicate command is typed
	 */
	while ((opt = getopt(argc, argv, "psUSvc")) != -1 ) {

		switch (opt) {
		case 'p':

			/* check whether option 'p' has been processed already */
			if (pFlag == 1)
			{
				printf("duplicate commnad p \n");
				exit(EXIT_FAILURE);
			}
			if(optind >= argc)
			{

				printf("no value after p \n");
				exit(EXIT_FAILURE);
			}
			// if IDSize is enougth to hold the processID
			if ( strlen(argv[optind]) < IDsize)
			{
				strcpy(ProcessID,argv[optind]) ;
			}
			else
			{
				// allocate more space for IDSize to hold ID
				while(strlen(argv[optind]) > IDsize)
				{
					IDsize = 2*IDsize ;
				}

				ProcessID = (char *) realloc (ProcessID, IDsize);
				/* the ProcessID is the string following the 'p' option */
				strcpy(ProcessID,argv[optind]) ;
				ProcessID[IDsize] ='\0';
			}

			/* if IsStringOfInt returns -1 if the ProcessID is not a string of integers */
			int inputValid = IsStringOfInt(ProcessID);
			//exit if ID is not valid
			if (inputValid == -1)
			{
				printf("bad input for PID. quitting\n");
				exit(EXIT_FAILURE);
			}

			//set flag to one
			pFlag = 1;
			break;

		case 's':
			// exit if duplicate command

			if(sFlag<1)
			{
				order [count] = 's'; /* add option 's' into oder */
				count++ ;            // update the number of command
				sFlag = 1;           // set flag to be one
			}

			break;
		case 'S':
			// exit if duplicate command

			if(SFlag <1)
			{
				order [count] = 'S'; /* add option 'S' into oder */

				count++ ;           // update the number of command
				SFlag = 1 ;         // set flag to be one
			}
			break;
		case 'U':
			// exit if duplicate command

			if(UFlag <1)
			{
				order [count] = 'U';   /* add option 'S' into oder */
				count++ ;             // update the number of command
				UFlag = 1 ;      }    // set flag to be one
			break;
		case 'v':
			// exit if duplicate command

			if(vFlag < 1 )
			{
				order [count] = 'v';  /* add option 'v' into oder */

				count++ ;            // update the number of command
				vFlag = 1 ;
			}// set flag to be one}
			break;
		case 'c' :
			//exit if duplicate command

			cFlag = 1 ; // default to be true
			break;

		default: /* '?' */
			fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	// c default to be true
	order[4] = 'c';
	count ++;
	cFlag = 1 ;

	if (optind >= argc) {
		fprintf(stderr, "Expected argument after options\n");
		exit(EXIT_FAILURE);
	}
	printf("\n");
	/*printf("name argument = %s\n", argv[optind]);*/

	if (pFlag == 1)
	{
		/* check whether the ProcessID is in the process ID list */
		if ( hasit(ProcessID) == -1 )
		{
			printf("no such process running.\n");

			exit(EXIT_FAILURE);

		}
		/*find files corresponding to options
		in proc file system and display info*/
		else
		{
			// print its Process ID
			printf("%s", ProcessID);
			printf("%s", Format);
			/* find files corresponding to options in proc
			 *  file system and display info            */
			PorcListInfoGenerator(ProcessID, order, count );

		}
	}
	/* if process ID is not specified, then generate a linked
	 * list to store the ID
	 */
	else
	{
		/* procList structure is defined in the Prolist.c
		 *function openFileAndGenerate generates
		 * a linked list  of all process of current usr*/
		struct procList * listNode = OpenFileAndGenerate();
		// pointing to the next since first node is dummyNode
		listNode = listNode->next;

		// iterate across the list and store the
		// ID name in it
		while (listNode != NULL)
		{
			/* copy  process Id*/
			/* check if Idsize exceed the capacity IDsize
			 * if exceed then resize the ProcessID by
			 * double its bufferspace
			 */
			if ( strlen(listNode->name) < IDsize)
			{
				strcpy(ProcessID,listNode->name) ;
			}
			else
			{
				// allocate more space for IDSize to hold ID
				while(strlen(listNode->name) > IDsize)
				{
					IDsize = 2*IDsize ;
				}

				ProcessID = (char *) realloc (ProcessID, IDsize);
				/* the ProcessID is the string following the 'p' option */

				// copy iistnode name to processID

				strcpy(ProcessID,listNode->name) ;

				ProcessID[IDsize -1] ='\0';
			}

			listNode = listNode->next;
			// print out the process ID
			printf("%s", ProcessID);
			printf("%s", Format);
			// PorcListInfoGenerator display the info
			// PorcListInfoGenerator is defined in parser.c
			PorcListInfoGenerator(ProcessID, order, count );
			// reset process ID to null
			int l ;
			for (l= 0; l <IDsize;l++)
			{
				ProcessID [l] = '\0';
			}
		}
	}

	/* Other code omitted */
	free (ProcessID);
	exit(EXIT_SUCCESS);
}


/* this function read the /proc file system and return 1 if
 * char *b is one of the process IDs. Otherwise return -1.
 *
 * Parameters:
 *  b: the process ID passed to be checked
 */

int hasit(char *b)
{
	/* open directory to check
	 * if id is valid
	 */
	DIR * dir ;
	struct dirent *entry;
	char * proc = "/proc";


	if ((dir = opendir(proc)) == NULL)
	{
		// exit if problem in opening directory
		perror("opendir() error");
		exit(EXIT_FAILURE);
	}
	else {
		/* read contents in "/proc" file system and return 1
		 * if the char *b is also in the "/proc" directory */
		while ((entry = readdir(dir)) != NULL)

		{
			if (strcmp(entry->d_name, b) == 0 )

			{
				return 1 ;
			}


		}
		// close dir
		closedir(dir);
		return -1 ;
	}

}



/* this function check if the string is int
 * if not return -1  if it is int return 0
 * Parameters:
 * intString: the process ID passed to be checked
 */

int IsStringOfInt(char *intString){
	char *curr = intString;           //current character in this string

	/*repeat this procure until the end of string*/
	while((*curr) != '\0'){
		// check if the character is number or not
		if(!(*curr >='0' && *curr <='9')){
			/*if the current character is number, increment the counter,
			convert it to decimal, change the flag*/
			return -1;
		}
		curr++;

	}

	/*move the iterator to the next character*/

	/*if there is negative number marked, make it negative*/

	return 0;
}



