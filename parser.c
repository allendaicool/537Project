/*****************************************************
* FILE NAME: parser.c
*
* Created on: Sep 21 2013
* cs 537
* cs login: yihong  guangpu
* Professor  Barton P.Miller
* Author: Yihong Dai  guangpu zhou
* PURPOSE: check options, take corresponding files
*          from proc directory and display information
*
*****************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include "optionProcessing.h"
#include <string.h>

// export the flag information from  optionProcessing.c
extern int sFlag  ;
extern int UFlag  ;
extern int SFlag ;
extern int vFlag  ;

/* initilize the statstring  and statmstringto
 * keep the stat and statm file
 */
char * statString = NULL ;
char * statmString = NULL;
// outTotal keep the output string
char *outTotal [5] ;

/* this function print out the info associated with
 * the given processID
 * Parameters:
 * processID: specific ID generated by the linked list
 *            or specified as argument
 * count:  the number of option in the argv
 * order:  the order of the input option
 */
int  PorcListInfoGenerator (char* processID, char * order , int count)
{
	/* initilize all the element in the outtotal
	* to be null */
	int a ;
	for (a = 0 ; a<5 ;a++)
	{
		outTotal [a] =NULL;
	}
    /* the following is the string required to
	* open the stat or statm file*/
	const char * proc = "/proc/";
	const char * stat = "/stat";
	const char * size = "size=";

	// the following is the print format
	const char * formatnUtime = "Utime=";
	const char * statm = "/statm";
	const char *formatnStime = "Stime=" ;

	// copy the ID
	char * ID = (char *)malloc(strlen(processID)+1);
	strcpy (ID, processID);

	// the string to open stat
	char * procMenuStat = (char *)malloc(strlen(proc)+strlen(ID)+strlen(stat)+1);
	strcpy (procMenuStat, proc);
	strcat (procMenuStat, ID);
	strcat (procMenuStat, stat);

	// the string to open statm
	char * procMenuStatm = (char *)malloc(strlen(proc)+strlen(ID)+strlen(statm)+1);
	strcpy (procMenuStatm, proc);
	strcat (procMenuStatm, ID);
	strcat (procMenuStatm, statm);


    // if any of s S U flag is on open stat
	if ( sFlag == 1 || UFlag == 1 ||SFlag== 1 )
	{
		// fpstat pointer to the file
		FILE *fpStat ;
		fpStat = fopen(procMenuStat, "rb");
		// exit if error
		if( fpStat == NULL )
		{
			perror("Error while opening the file.!!! \n");
			exit(EXIT_FAILURE);
		}

        // allocate 100 bytes for statString

		int buffersize = 100;
		statString = ( char *) malloc(buffersize+1);

		// loopcount
		int loopCount = 0 ;
		while( loopCount < 100)
		{
			statString[ loopCount ] = '\0';
			loopCount++;
		} // initialize all element to be null

		loopCount = 0;

		/* read in the character in the statfile
		 * if statstring out of space, then resize statstring
		 * to hold more characters
		 */
		char ne;

		// end if end of file
		while((ne = fgetc(fpStat) ) != EOF )
		{
			int length = strlen(statString) ;
			// if out of space for statstring
			if (( strlen(statString) + 1) == buffersize)
			{
				statString = (char*) realloc(statString,buffersize*2);
				int ccount = buffersize-1 ;
				// resize the statstring
				while(ccount< (buffersize*2 ))
				{
					statString[ ccount ] = '\0';
					ccount++;
				}
				buffersize = buffersize*2 ;

			}
           // store the char in the string
			statString[length] = ne;

		}

		// parse the string to get the item we want
		char * tempoo = strtok (statString,"\n ");

		int LoopCount = 0;
		int count1 = 0;

        /* loopcount is 15 then to get the 2, 13, 14 item
         *
         */

		while ( LoopCount<15)
		{
			tempoo = strtok (NULL, "\n ");

			if(tempoo == NULL)
			{
				printf("it is null ");
			}

			LoopCount ++ ;
			if (LoopCount == 2 ||LoopCount == 13||LoopCount == 14)
			{
				// store the specified item in the array
				outTotal [count1 ] = tempoo ;

				count1++;

			}
		}
// close the file at the end
		fclose(fpStat);
	}


	if(vFlag == 1 )
	{
		/* open the statm file*/

		FILE *fpStatm ;
		fpStatm =  fopen(procMenuStatm, "rb");
		//exit if error
		if( fpStatm == NULL )
		{
			perror("Error while opening the file.\n");
			exit(EXIT_FAILURE);
		}

		/* set the buffer for string that contain the
		 * first field in the statm to be 50
		 */

		int buffersize = 50 ;
		statmString = ( char *) malloc(buffersize+1);
		int loopCount2 = 0 ;
		while( loopCount2 < buffersize)
		{
			statmString[ loopCount2 ] = '\0';
			loopCount2++;
		} // initialize the string to be null

		char ne ;
		// read in each charactre of statm in its first field
		while((ne = fgetc(fpStatm) ) != ' ' && ne != '\n' && ne != '\t' )
		{

			int length = strlen(statmString) ;
			// resize the statm string if out of space
			if (( strlen(statmString) + 1) == buffersize)
			{
				statmString = (char*) realloc(statmString,buffersize*2);
				int ccount = buffersize-1 ;
				while(ccount< (buffersize*2 ))
				{
					statmString[ ccount ] = '\0';
					ccount++;
				}
				buffersize = buffersize*2 ;

			}
            // store the character in statmstring
			statmString[length] = ne;

		}
        // store the string we need in the outtotal array
		char * iterator = strtok (statmString,"\n ");
		outTotal[3] = iterator;
		// close the statm file
		fclose(fpStatm);
	}



	int haveLoop =  0 ;
	// print the info basd on the input option
	while ( haveLoop < count &&  order[haveLoop]!= '\0')
	{
		switch (order[haveLoop]) {
		case 's':

			printf(" state = %s " , outTotal[0]);
			break;
		case 'S':

			printf(" %s  " , formatnStime );
			printf("%s", outTotal[2]);

			break;
		case 'U':
			printf(" %s  " , formatnUtime );
			printf("%s", outTotal[1]);


			break;
		case 'v':
			printf(" %s  " , size );
			printf("%s", outTotal[3]);


			break;
		case 'c':
			printf(" ");
			break;
		default:
			printf(" 1error occur invalid input");
			exit(EXIT_FAILURE);
		}
		haveLoop++ ;
	}

	// print the cmdline
	const char * cmdlineConst = "/cmdline";
	char * cmdlineComma = (char *) malloc (strlen(cmdlineConst) + strlen(proc) + strlen(ID)+1);
	strcpy(cmdlineComma,proc );
	strcat (cmdlineComma , ID  );
	strcat (cmdlineComma , cmdlineConst  );
	// create a pointer to the cmdline file
	FILE * cmdPointer;
	cmdPointer = fopen (cmdlineComma, "rb" );
	char * linePointer= NULL ;
	size_t dum = 0 ;
	char *s = "\n \t" ;
	printf(" [");
	// to get each words in the cmdline file by using getline
	while(getline(&linePointer , &dum ,cmdPointer ) != -1 )
	{
		char * first = strtok(linePointer,s);

		while( first != NULL )
		{
			printf("%s  " , first);

			first = strtok(NULL, s);
		}

	}
	printf("]");

	printf("\n");

	// free the memory for string
	free(procMenuStat);
	procMenuStat = NULL ;
	free(procMenuStatm);
	procMenuStatm = NULL ;
	free (statString);
	statString = NULL;
	free (statmString);
	statmString = NULL;

	free (ID);
	ID = NULL ;
	return 1 ;


}


