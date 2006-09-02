/*****************************************/
/* File: scan.c                          */
/* Yueq C Compiler         */
/* Author : K.C.L                        */
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
#include "util.h"
#include "scan.h"


typedef enum { START,INASSIGN,INCOMMENT,INNUM,INOVER,INENDCOMMENT,INID,INLTEQ,INBGEQ,INNEQ,DONE, INNUM_F, INCHAR, INPLUS, INMINUS, INESCAPE} StateType;


char tokenString[MAXTOKENLEN+1];

#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */
static int aftereq = 0;
static int aftereq_minus = 0;
static int getNextChar(void)
{ 
	if (!(linepos < bufsize))
	{ 
		lineno++;
		if (fgets(lineBuf,BUFLEN-1,source))
		{
			if (EchoSource) 
				fprintf(listing,"%4d: %s",lineno,lineBuf);
						
			bufsize = strlen(lineBuf);
			linepos = 0;
			
			return lineBuf[linepos++];
		}
		else
		{ 
			EOF_flag = TRUE;
			return EOF;
		}
	}
	else
	{		
		return lineBuf[linepos++];
	}
}


static void ungetNextChar(void)
{ 
	if (!EOF_flag) 
		linepos-- ;
}

/* lookup table of reserved words */
static struct
{ 	
	char* str;
	TokenType tok;
} reservedWords[MAXRESERVED]
= {	{"if",IF},
	{"else",ELSE},
	{"int",INT},
	{"char",CHAR},
	{"const", CONST},
	{"float",FLOAT},
	{"return",RETURN},
	{"void",VOID},
	{"while",WHILE},
	{"scanf",INPUT},
	{"switch", SWITCH},
	{"case", CASE},
	{"printf",OUTPUT},
	{"main",MAIN},
	{"do", DO}};


static TokenType reservedLookup (char *s)
{ 	int i;
	for (i=0;i<MAXRESERVED;i++)
		if (!strcmp(s,reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}


TokenType getToken(void)
{
	int tokenStringIndex = 0;
	
	TokenType currentToken;
	
	StateType state = START;
	
	int save;
	int sign = 1;

	while (state != DONE)
	{
		
		int c = getNextChar();
		save = TRUE;
		switch (state)
		{ case START:
			if (isdigit(c))
				state = INNUM;
			else if (isalpha(c) || (c == '_'))
				state = INID;
			else if (c == '=')
				state = INASSIGN;
			else if (c == '<')
				state = INLTEQ;
			else if (c == '>')
				state = INBGEQ;
			else if (c == '!')
				state = INNEQ;
			else if (c == '+')
				state = INPLUS;
			else if (c == '-')
				state = INMINUS;
			else if ((c == ' ') || (c == '\t') || (c == '\n'))
				save = FALSE;
			else if (c == '/')
			{
				save=FALSE;
				state = INOVER;
			}
			else if (c == '\'')
			{
				save=FALSE;
				state = INCHAR;
			}
			else if (c == '\\')
			{
				save = FALSE;
				state = INESCAPE;
			}
			else
			{ 
				state = DONE;
				switch (c)
				{
					case EOF:
						save = FALSE;
						currentToken = ENDFILE;
						break;
					case ':':
//						currentToken=ERROR;
						currentToken = COLON;
						break;
					case '{':
						currentToken = LCIR;
						break;
					case '}':
						currentToken = RCIR;
						break;
					case '[':
						currentToken = LQUATA;
						break;
					case ']':
						currentToken = RQUATA;
						break;
					/*case '+':
						currentToken = PLUS;
						break;
					case '-':
						currentToken = MINUS;
						break;*/
					case '*':
						currentToken = TIMES;
						break;
					case '(':
						currentToken = LPAREN;
						break;
					case ')':
						currentToken = RPAREN;
						break;
					case ';':
						currentToken = SEMI;
						break;
					case ',':
						currentToken = COMOS;
						break;
					default:
						currentToken = ERROR;
						break;
				}
			}
			break;
			case INLTEQ:
				state=DONE;
				if(c=='=')
					currentToken=LTEQ;
				else
				{
					ungetNextChar();
					currentToken=LT;
				}
				break;
			case INBGEQ:
				state=DONE;
				if(c=='=')
					currentToken=BGEQ;
				else
				{
					ungetNextChar();
					currentToken=BG;
				}
				break;
			case INNEQ:
				state=DONE;
				if(c=='=')
					currentToken=NEQ;
				else
				{
					ungetNextChar();
					currentToken=ERROR;
				}
				break;
				
			case INOVER:
				save = FALSE;
				if(c=='*')
					state=INCOMMENT;
				else
				{
					ungetNextChar();
					state=DONE;
					tokenString[tokenStringIndex++] = '/';
					currentToken=OVER;
				}
				break;
			case INCOMMENT:
				save = FALSE;
				if (c == EOF)
				{
					state = DONE;
					currentToken = ENDFILE;
				}
				else if (c == '*')
					state = INENDCOMMENT;
				break;
			case INENDCOMMENT:
				save = FALSE;
				if (c == '/')
					state = START;
				else
					state = INCOMMENT;
				break;
			case INASSIGN:
				state = DONE;
				if (c == '=')
				{
					currentToken = EQ;
					aftereq = 1;
					aftereq_minus = 1;
				}
				else
				{
					aftereq = 1;
					aftereq_minus = 1;
					ungetNextChar();
					//save = FALSE;
					currentToken = ASSIGN;
				}
				break;
			case INPLUS:
				if(isdigit(c) && (aftereq == 1))
				{
					aftereq = 2;
					state = INNUM;
				}
				else if (aftereq == 1024)
				{
					state = DONE;
					ungetNextChar();
					currentToken = PLUS;
				}
				else
				{
					state = DONE;
					ungetNextChar();
					currentToken = PLUS;
				}
				break;
			case INMINUS:
				if (isdigit(c) && (aftereq_minus == 1))
				{
					aftereq_minus = 2;
					state = INNUM;
					sign = -1;
				}
				else if (aftereq_minus == 1024)
				{
					state = DONE;
					ungetNextChar();
					currentToken = MINUS;
				}
				else
				{
					state = DONE;
					ungetNextChar();
					currentToken = MINUS;
				}
				break;
			case INNUM:
				if (c == '.')
				{
					state = INNUM_F;
				}
				if (!isdigit(c) && (c != '.'))
				{
					ungetNextChar();
					save = FALSE;
					state = DONE;
					currentToken = NUM;
				}
				break;
			case INNUM_F:
				if (!isdigit(c))
				{
					ungetNextChar();
					save = FALSE;
					state = DONE;
					currentToken = NUM_F;
				}
				break;
			case INCHAR:
				if (!isalpha(c) && !isdigit(c) && (c != '*') && (c != '+'))
				{
					if (c != '\'')
						ungetNextChar();	
					save = FALSE;
					state = DONE;
					currentToken = NUM;
				}
				break;
			case INID:
				if ((!isalpha(c)) && (c != '_') && (!isdigit(c)))
				{
					ungetNextChar();
					save = FALSE;
					state = DONE;
					currentToken = ID;
				}
				break;
			case DONE:
			default:
				fprintf(listing,"Scanner Bug: state= %d\n",state);
				state = DONE;
				currentToken = ERROR;
				break;
		}
		if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			tokenString[tokenStringIndex++] = (char) c;
		if (state == DONE)
		{ 
			tokenString[tokenStringIndex] = '\0';
			if (currentToken == ID)
			{
				currentToken = reservedLookup(tokenString);
				if (aftereq == 1){aftereq = 1024;}
				if (aftereq_minus == 1){aftereq_minus = 1024;}
			}
			if ((currentToken == NUM) || (currentToken == NUM_F))
			{
				if (aftereq == 1){aftereq = 1024;}
				if (aftereq_minus == 1){aftereq_minus = 1024;}
			}
		}
	}
	if (TraceScan) {
		fprintf(listing,"\t%d: ",lineno);
		printToken(currentToken,tokenString);
	}

	return currentToken;
} /* end getToken */


