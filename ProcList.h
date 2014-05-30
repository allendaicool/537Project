/*****************************************************
 * FILE NAME: parser.h
 *
 * Created on: Sep 14 2013
 * Author: Yihong Dai guangpu zhou
 * Professor : Barton P Miller
 * cs login: yihong guangpu
 * PURPOSE: the header file of ProcList.h which will be included in optionProcessing.c
 *
 *****************************************************/

#ifndef PROCLIST_H_
#define PROCLIST_H_

// struct node used in optionProcessing
struct procList
{
	char * name;
	struct procList *next ;

};

struct procList* OpenFileAndGenerate ();

#endif /* PROCLIST_H_ */
