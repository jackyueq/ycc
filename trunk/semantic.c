/*****************************************/
/* File: semantic.c 	                  */
/* Yueq C Compiler                        */
/* Author : K.C.L                         */
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
#include "globals.h"
#include "symtab.h"
#include "semantic.h"

/* counter for variable memory locations */
static int location = 0;
static int i=0;
static int pos = 1;
static int indeflag = 0;

//static void traverse(TreeNode *,void (* preProc) (TreeNode *,int), void (* postProc) (TreeNode *,int) )
/* Procedure traverse is a generic recursive
* syntax tree traversal routine:
* it applies preProc in preorder and postProc
* in postorder to tree pointed to by t
*/


/*
int funcNumCount(TreeNode *t)
{
	while(t!=NULL)
	{
		if(t->nodekind==ExpK)
		{
		if(t->kind.exp=FuncK)
			
	
	}
*/
				
static int returnPos(char *funcName)
{
	int i=0,j=1;
	while(j!=0)
	{
		j=strcmp(funcName,func[i++]);
	}

	return i-1;
}


static void traverse_global(TreeNode *t,void(* preProc)(TreeNode *,int),void(* postProc)(TreeNode *,int))
{
	if(t!=NULL)
	{
		if((t->nodekind==ExpK)&&((t->kind.exp==IntK)||(t->kind.exp==CharK)||(t->kind.exp==FloatK)||(t->kind.exp==IntarrayK)))
			preProc(t,0);
		else if((t->nodekind==ExpK)&&(t->kind.exp==DefuncK)&&(!strcmp("main",t->attr.name)))
		{
			traverse_global(t->child[1],preProc,postProc);
		}
		postProc(t,0);
		traverse_global(t->sibling,preProc,postProc);
	}
}

static void traverse( TreeNode *t,void (* preProc) (TreeNode *,int),void (* postProc) (TreeNode *,int) )
{

	if(t!=NULL)
	{
		//if(t->nodekind==ExpK)
		//printf("ExpK------>%d\n",t->kind.exp);
		//else
		//printf("StmtK----->%d\n",t->kind.stmt);
		//printf("\n\n    %d    \n\n",pos);
		if((t->nodekind==ExpK)&&((t->kind.exp==IntK)||(t->kind.exp==CharK)||(t->kind.exp==FloatK))&&indeflag)
		{
			preProc(t, pos);
		}
		else
		{
			if((t->nodekind==ExpK)&&(t->kind.exp==DefuncK)&&strcmp(t->attr.name,"main"))
			{
				indeflag=1;
				pos=returnPos(t->attr.name);
				preProc(t,pos);
				traverse(t->child[1],preProc,postProc);
			}
		}
		postProc(t,pos);
		traverse(t->sibling,preProc,postProc);
		
	}
}


static void nullProc(TreeNode *t,int k)
{
	int i=k;
	if (t==NULL) return;
	else return;
}

/* Procedure insertNode inserts
* identifiers stored in t into
* the symbol table
*/
static void insertNode( TreeNode *t,int pos)
{
	int i;
	int dump;
	//const char *p="1";
	//char *temp=NULL;
	if(t->nodekind==ExpK)
	{
		//if((t->kind.exp==IntK)||(t->kind.exp==IdK))
		if((t->kind.exp==IntK)||(t->kind.exp==CharK)||(t->kind.exp==FloatK)||(t->kind.exp==IdK))
		{
			if(st_lookup(t->attr.name,pos)==-1)
			{
				st_insert(t->attr.name,t->lineno,location++,pos);
			}
			//else
			//{
			//st_insert(t->attr.name,t->lineno,0,pos);
			//}
		}
		else if(t->kind.exp==IntarrayK)
		{
			if(st_lookup(t->attr.name,pos)==-1)
			{
				st_insert(t->attr.name,t->lineno,location++,pos);
				dump=t->child[0]->attr.val;
				//temp=t->attr.name;
				//strcpy(temp,(const char *)t->attr.name);
				//for(i=0;i<t->child[0]->attr.val;i++)
				st_insert("1",t->lineno,location++,pos);dump--;
				if(dump!=0)
					st_insert("2",t->lineno,location++,pos);dump--;
				if(dump!=0)
					st_insert("3",t->lineno,location++,pos);dump--;
				if(dump!=0)
					st_insert("4",t->lineno,location++,pos);dump--;
				if(dump!=0)
					st_insert("5",t->lineno,location++,pos);dump--;
				if(dump!=0)
					st_insert("6",t->lineno,location++,pos);dump--;
				if(dump!=0)
					st_insert("7",t->lineno,location++,pos);dump--;
				if(dump!=0)
					st_insert("8",t->lineno,location++,pos);dump--;
				if(dump!=0)
					st_insert("9",t->lineno,location++,pos);dump--;
			}
			//else
			//{
			//st_insert(t->attr.name,t->lineno,0,pos);
			//}
		}
	}
/****************************/
/* LAST EDITED 4.2 */
/*
if (t->nodekind == StmtK)
{
	switch (t->kind.stmt)
	{
		case InputK:
			if (st_lookup(t->attr.name, pos) == -1)
				st_insert(t->attr.name, t->lineno, location++, pos);
			else
				st_insert(t->attr.name, t->lineno, 0, pos);
		break;
		default:break;
	}
}
*/
/*
	OpK,ConstK,IdK,ArrayK,FuncK,IntK,VoidK,EmptyK
switch (t->nodekind)
{ 
	case StmtK:
		switch (t->kind.stmt)
		{ 
			case AssignK:
			case ReadK:
				if (st_lookup(t->attr.name) == -1)
					st_insert(t->attr.name,t->lineno,location++);
				else	
					st_insert(t->attr.name,t->lineno,0);
			break;
			default:
			break;
		}
	break;
	case ExpK:
		switch (t->kind.exp)
		{ 
			case IdK:
				if (st_lookup(t->attr.name) == -1)
					st_insert(t->attr.name,t->lineno,location++);
				else
					st_insert(t->attr.name,t->lineno,0);
			break;
			default:
				break;
		}
	break;
	default:
		break;
}*/
}
	
/* Function buildSymtab constructs the symbol
 table by preorder traversal of the syntax tree
*/
void buildSymtab(TreeNode *syntaxTree)
{
	traverse_global(syntaxTree,insertNode,nullProc);
	traverse(syntaxTree,insertNode,nullProc);

	if (TraceAnalyze)
	{ 
		fprintf(listing,"\nSymbol table:\n\n");
		
		for(i=0;i<=pos;i++)
		{
			printf("\n\n");
			printSymTab(listing,i);
		}
	}
}

static void typeError(TreeNode *t, char *message)
{ 
	fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
	Error = TRUE;
}
