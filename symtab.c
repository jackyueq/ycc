/*****************************************/
/* File: Symtab.c                                                  */
/* Yueq C COmpiler		                             */
/* Author : K.C.L                                                   */
/* Further Modified by Qi Yue (yueq@cse.buaa.edu.cn)*/
/*****************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/* SHIFT is the power of two used as multiplier in hash function  */
#define SHIFT 4

/* the hash function */
static int hash ( char *key )
{ 
	int temp = 0;
	int i = 0;
	while (key[i] != '\0')
	{ 
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}


typedef struct LineListRec
{ 
	int lineno;
	struct LineListRec *next;
} *LineList;


typedef struct BucketListRec
{ 
	char *name;
	LineList lines;
	int memloc ; /* memory location for variable */
	struct BucketListRec *next;
} *BucketList;

/* Hash table */
struct symptableRec
{
	BucketList hashTable[SIZE];
};

struct symptableRec symptable[10];

void st_insert(char *name, int lineno, int loc,int pos)
{ 
	int h = hash(name);
	BucketList l =  symptable[pos].hashTable[h];
	
	while ((l != NULL) && (strcmp(name,l->name) != 0))
		l = l->next;
	
	if (l == NULL) /* variable not yet in table */
	{ 
		l = (BucketList) malloc(sizeof(struct BucketListRec));
		l->name = name;
		l->lines = (LineList) malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->lines->next = NULL;
		l->next = symptable[pos].hashTable[h];
		symptable[pos].hashTable[h] = l; }
	else /* found in table, so just add line number */
	{ 
		LineList t = l->lines;
		while (t->next != NULL) t = t->next;
		t->next = (LineList) malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
} /* st_insert */


int st_lookup(char *name,int pos)
{ 
	int h = hash(name);
	BucketList l = symptable[pos].hashTable[h];
	while ((l != NULL) && (strcmp(name,l->name) != 0))
		l = l->next;
	if (l == NULL) 
		return -1;
	else 
		return l->memloc;
}


void printSymTab(FILE *listing,int pos)
{	
	int i;
	fprintf(listing,"Variable Name\tLocation\tLine Numbers\n");
	fprintf(listing,"-------------\t--------\t------------\n");
	for (i=0;i<SIZE;++i)
	{ 
		if (symptable[pos].hashTable[i] != NULL)
		{ 
			BucketList l = symptable[pos].hashTable[i];
			while (l != NULL)
			{ 
				LineList t = l->lines;
				fprintf(listing,"%-14s ",l->name);
				fprintf(listing,"%-8d  ",l->memloc);
				while (t != NULL)
				{ 
					fprintf(listing,"%4d ",t->lineno);
					t = t->next;
				}
				fprintf(listing,"\n");
				l = l->next;
			}
		}
	}
} /* printSymTab */

