/****************************************************/
/* File: globals.h                                  */
/* Yueq C Compiler		              */
/* Author : K.C.L                                   */
/* Further Modified by Qi Yue (yueq@cse.buaa.edu.cn)*/
/****************************************************/

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

   

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* number of reserved words */
#define MAXRESERVED 15

typedef enum {   
    /* book-keeping tokens */
    ENDFILE,ERROR,
    /* reserved words */
    CASE,CHAR,CONST,ELSE,FLOAT,IF,INT,RETURN,VOID,WHILE,INPUT,OUTPUT,MAIN,SWITCH,DO,
    /* multicharacter tokens */
    ID,NUM,NUM_F,NUM_C,
    /* special symbols */
    ASSIGN,EQ,LT,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,SEMI,LQUATA,RQUATA,LCIR,RCIR,LTEQ,BG,BGEQ,NEQ,COMOS,COLON
   } TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno; /* source line number for listing */

extern char *func[20];

//extern struct my_functable
//{
//char *p;
//int type;
//}my_functable[10];

 
/**************************************************/
/***********   Syntax tree for parsing ************/
/**************************************************/

typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,WhileK,OutputK,InputK,ReturnK} StmtKind;
typedef enum {OpK,IntarrayK,ConstK,IdK,ArrayK,DefuncK,FuncK,IntK,VoidK,EmptyK,CharK,FloatK} ExpKind;

/* ExpType is used for type checking */
typedef enum {Void,Integer,Boolean, Char, Float, Const_Int, Const_Float, Const_Char} ExpType;

#define MAXCHILDREN 10

typedef struct treeNode
{ 
	struct treeNode *child[MAXCHILDREN];
     	struct treeNode *sibling;
	int lineno;
	NodeKind nodekind;
     union { 
	StmtKind stmt;
	ExpKind exp;} kind;
     union { 
	TokenType op;
	int val;
	char val_c;
	float val_f;
	char *name; } attr;
	ExpType type; /* for type checking of exps */
   } TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 *be echoed to the listing file with line numbers
 *during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 *printed to the listing file as each token is
 *recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 *printed to the listing file in linearized form
 *(using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 *and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 *to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif
