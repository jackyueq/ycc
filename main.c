/****************************************************/
/* File:    main.h                                  */
/* Yueq C C0mpiler		                    */
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

#include "globals.h"


#define NO_PARSE FALSE

#define NO_ANALYZE FALSE

#define NO_CODE FALSE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "tree.h"
#if !NO_ANALYZE
#include "semantic.h"
#if !NO_CODE
#include "generate.h"
#endif
#endif
#endif
char *func[20];
/* allocate global variables */
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;
//struct my_functable
//{
//char *p;
//int type;
//}my_functable[10];
/* allocate and set tracing flags */

int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = FALSE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;
int Error = FALSE;

main( int argc, char *argv[] )
{
	int i=0;
	listing = stdout;
	TreeNode *syntaxTree;
	char pgm[120]; /* source code file name */
	
	if ((argc<2)||(argc>6))
	{ 
		fprintf(stderr,"usage: %s [-e] [-s] [-p] [-t] <filename>\n",argv[0]);
			exit(1);
	}
	
	if((argc==2)&&(!strcmp(argv[1],"-h") || !strcmp(argv[1], "--help")))
	{
		fprintf(listing,"\nYueq C Compiler Options:\n");
		fprintf(listing,"%5s\t%s", "-e", "Show the source code\n");
		fprintf(listing,"%5s\t%s", "-s", "Show the scan progress\n");
		fprintf(listing,"%5s\t%s", "-p", "Show the parse process\n");
		fprintf(listing,"%5s\t%s", "-t", "Show the analysis process\n");
		fprintf(listing,"%5s\t%s", "-h --help", "Show this help\n");
		return 0;
	}
	else if((argc==2)&&(!strcmp(argv[1],"-v")||!strcmp(argv[1], "--version")))
	{
		fprintf(listing,"Yueq C Compiler: 0.1.0.0\n");
		return 0;
	}
	
	for(i=1;i<argc;i++)
	{
		if(!strcmp(argv[i],"-e"))
			EchoSource=TRUE;
		else if(!strcmp(argv[i],"-s"))
			TraceScan=TRUE;
		else if(!strcmp(argv[i],"-p"))
			TraceParse=TRUE;
		else if(!strcmp(argv[i],"-t"))
			TraceAnalyze=TRUE;
	}
	
	
	
	
	
	
	
	strcpy(pgm,argv[argc-1]) ;
	if (strchr (pgm, '.') == NULL)
		strcat(pgm,".c0");
	source = fopen(pgm,"r");
	if (source==NULL)
	{ 
		fprintf(stderr,"ycc : %s : File does not exist\n",pgm);
		exit(1);
	}
	/* send listing to screen */
	
	fprintf(listing,"Yueq C Compiler is compiling %s ...\n",pgm);
	
	#if NO_PARSE
	while (getToken()!=ENDFILE);
	
	#else
		syntaxTree = parse();
	// fprintf(listing,"\nSuccessfully constructed the syntax tree for the source file .\n");
	if (TraceParse) {
		fprintf(listing,"\nSyntax tree:\n");
		printTree(syntaxTree);
		//printf("%s",syntaxTree->sibling->child[1]->sibling->attr.name);
	}
	
	#if !NO_ANALYZE
	if (! Error)
	{ 
		if (TraceAnalyze) 
			fprintf(listing,"\nBuilding Symbol Table...\n");
		buildSymtab(syntaxTree);
		// fprintf(listing,"\nSuccessfully builded the synbol table for the source file .\n");
		
	}
	
	
	#if !NO_CODE
	if (!Error)
	{ 
		char *codefile;
		int fnlen = strcspn(pgm,".");
		codefile = (char *) calloc(fnlen+4, sizeof(char));
		strncpy(codefile,pgm,fnlen);
		strcat(codefile,".vm");
		code = fopen(codefile,"w");
		if (code == NULL)
		{ 
			printf("Error: %s does not exists\n",codefile);
			exit(1);
		}
		codeGen(syntaxTree,codefile);
		fclose(code);
	}
	#endif
	
	#endif
	
	#endif
	fprintf(listing, "\nCompile completed\n");
	
	fclose(source);
	return 0;
}


