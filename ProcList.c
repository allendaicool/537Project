/*/*****************************************************
 * FILE NAME: ProcList.c
 *
 * Created on: Sep 14 2013
 * Author: Yihong Dai guangpu zhou
 * Professor : Barton P Miller
 * cs login: yihong  guangpu
 * PURPOSE: this file generates a linked list where each node store
 * process ID
 *
 *****************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include "optionProcessing.h"
#include <string.h>

/* this structure is the linked list
 *it store the process name'
 *and point to the next process
 */
struct procList
{
	char *name ;
	struct procList *next ;
};

// initializ the head the current pointer
struct procList *head = NULL;
struct procList *curr = NULL;


struct procList* OpenFileAndGenerate ();
struct procList*  generateProList( char * );
int IsCurrentUsr ( char * );


/* this function generates one node to be added
 * in the linked list
 * Parameters:
 * string: the process ID passed to be stored
 */
struct procList* generateProList( char * string)
{

    //  create a node to store the name and
	// return the current pointer
	struct procList * temp = (struct procList *) malloc(sizeof(struct procList));

	temp->name = string ;
	temp->next = NULL;

	curr->next = temp ;
	curr = temp;
	return curr ;
}

/* this function generates one node to be added
 * in the linked list
 * Parameters:
 * string: the process ID passed to be stored
 */
int create_list()
{
	/* create a linked list that the head
	 * is the dummy head
	 */
	struct procList *ptr = (struct test_struct*)malloc(sizeof(struct procList));
	if(NULL == ptr)
	{
		printf("\n Node creation failed \n");

	}
   // create a dummy
	ptr->name = "dummy";
	ptr->next = NULL;

	head = curr = ptr;

	return 1 ;
}

/* this function generates a linked list
 * which store the Process ID of current usr
 * parameter: none
 */
struct procList* OpenFileAndGenerate ()
{
	DIR * dir ;
	struct dirent *entry;
	char * proc = "/proc";
	/*printf("%s", fn);*/
	if ((dir = opendir(proc)) == NULL)
	{
		perror("opendir() error");
	}
	else {
        // create an empty linked list
		create_list();
		char* temp ;

		while ((entry = readdir(dir)) != NULL)
		{
			// look through the dir and copy the name to temp
			temp = (char *) malloc (strlen(entry->d_name) +1 );
			strcpy(temp ,   entry->d_name );

			// check if name is processID
			if(  IsStringOfInt ( temp)  == 0  )
			{
				// check if proess belongs to the current usr
				if(IsCurrentUsr(temp) ==1 )
				{
					// add node to the linked list
					generateProList( temp);
				}
			}

		}
		// close the dir when done
		closedir(dir);
	}

	return head ;
}


/* this function check if the process belongs to
 * current user.
 * return 1 if it is -1 if not
 * parameter: char name
 */
int IsCurrentUsr ( char * name)
{
	/* create a string to open the processstatus
	* to check if the name is a proess ID*/
	char * proc = "/proc/";
	char * status = "/status";
	char * read= (char * )malloc(strlen(name)+ strlen(proc)+strlen(status)+1);
	strcpy(read, proc);
	strcat(read, name);
	strcat(read, status);


	char * store = NULL ;

	size_t n = 0;
	// open status to find the usrID
	FILE *fpStat ;
	fpStat = fopen(read, "rb");
	if( fpStat == NULL )
	{
		perror("Error while opening the file.!!! \n");
		exit(EXIT_FAILURE);
	}

	int loop1 = 0 ;
	char *b = NULL  ;
	// s string is the delimiter
	char *s = "\n \t";
	/* this while loop is used to find the
	 * current usr ID by find the exact
	 * place where the usr ID is written
	 */
	while(getline(&store,&n,fpStat) != -1 )
	{

		char * first = strtok(store,s);
		int loop2 = 0 ;
		loop1++ ;

		while( first != NULL )
		{
			loop2 ++ ;
			first = strtok(NULL, s);
            // if we find the spot for ID the place is 7 and 3
			// which mean usr ID is line 7 and 3rd words in that line
			if( loop1 == 7 && loop2 == 3)
			{
				b = (char * ) malloc(strlen(first) + 1 );
				strcpy (b,first)  ;
			}
		}
	}
	// convert the ID to Int value
	int num = atoi(b);
	free(b);
	// get current ID
	int usrID = getuid();
	free (read);
	// compare UId with id associated with process
	if (num == usrID  )
	{
		return 1 ;
	}
	else
	{
		return -1 ;
	}
}


