/****************************************************/
/* File: tree.c                                  */
/* Yueq C Compiler		                   */
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
#include "util.h"
#include "scan.h"
#include "tree.h"

static TokenType token; /* holds current token */
static TreeNode *declaration_list(void);
static TreeNode *declaration(void);
static TreeNode *var_declaration(void);
static TreeNode *const_declaration(void);
static TreeNode *type_specifier(void);
static TreeNode *params(void);
static TreeNode *param_list(void);
static TreeNode *param(void);
static TreeNode *compound_stmt(void);
static TreeNode *local_declarations(void);
static TreeNode *statement_list(void);
static TreeNode *statement(void);
static TreeNode *expression_stmt(void);
static TreeNode *selection_stmt(void);
static TreeNode *switch_stmt(void);
static TreeNode *case_stmt(void);
static TreeNode *iteration_stmt(void);
static TreeNode *do_stmt(void);
static TreeNode *return_stmt(void);
static TreeNode *expression(void);
static TreeNode *var(void);
static TreeNode *simple_expression(void);
static TreeNode *additive_expression(void);
static TreeNode *term(void);
static TreeNode *factor(void);
static TreeNode *call(void);
static TreeNode *args(void);
static TreeNode *arg_list(void);
static int compareFunc(char *);
static void initfunc(void);
static int FuncFlag=1;

static void initfunc(void)
{
	int i=0;
	for(i=0;i<18;i++)
		func[i]="YCC_Init";
	func[18]="scanf";
	func[19]="printf";
}
static int compareFunc(char *a)
{
	int j=1;
	int i=0;
	int flag=1;
	for(i=0;i<20;i++)
	{
		j=strcmp(a,func[i]);
		if(j==0 && (flag=0)){}
	}

	if(flag==0)
		return FALSE;
	else
		return TRUE;
}


static void syntaxError(char *message)
{ 
	fprintf(listing,"\n>>> ");
	fprintf(listing,"Syntax error at line %d: %s",lineno,message);
	Error = TRUE;
}

static void match(TokenType expected)
{ 	
	if (token == expected) 
		token = getToken();
	else {
		syntaxError("unexpected token -> ");
		printToken(token,tokenString);
		syntaxError("  expected token == ");
		printToken(expected, "");
		fprintf(listing,"      ");
	}
}


TreeNode *declaration_list(void)
{
	TreeNode *t = declaration();
	TreeNode *p = t;
	while(token!=ENDFILE)
	{
		TreeNode *q=declaration();
		if(q!=NULL)
			p->sibling=q;
		p=q;
	}
	return t;
}

TreeNode *declaration(void)
{
	TreeNode *t=type_specifier();
	if((t!=NULL)&&((token == ID)||(token==MAIN)))
	{
		t->attr.name=strdup(tokenString);
		match(token);
	}
	switch(token)
	{
		case SEMI:
			match(SEMI);
			break;
		case LQUATA:
			match(LQUATA);
			TreeNode *p=newExpNode(IntarrayK);
			p->type=t->type;;
			p->attr.name=t->attr.name;
			if(token==NUM)
			{
				TreeNode *q=newExpNode(ConstK);
				q->attr.val = atoi(tokenString);
				p->child[0]=q;
				match(NUM);
			}
			match(RQUATA);
			match(SEMI);
			t=p;
			break;
		case LPAREN:
			match(LPAREN);
			TreeNode *k=newExpNode(DefuncK);
			k->attr.name=t->attr.name;
			if(strcmp("main",k->attr.name))
				func[FuncFlag++]=t->attr.name;
			else
				func[0]=t->attr.name;
			k->type=t->type;
			TreeNode *m=params();
			TreeNode *l=m;
			while(m!=NULL)
			{
				if(m->kind.exp==ArrayK)
					m->kind.exp=IntarrayK;
				m=m->sibling;
			}
			k->child[0] = l;
			match(RPAREN);
			TreeNode *n=compound_stmt();
			k->child[1] = n;
			t=k;
			break;
		default:
			break;
	}
	return t;
}

TreeNode *type_specifier(void)
{
	TreeNode *t = NULL;
	if(token == INT)
	{
		t = newExpNode(IntK);
		t->type=Integer;
		match(INT);
	}
	else if(token == CHAR)
	{
		t = newExpNode(CharK);
		t->type=Char;
		match(CHAR);
	}
	else if(token == FLOAT)
	{
		t = newExpNode(IntK);	
//		t = newExpNode(FloatK);
//		t->type=Float;
//this has to be noticed that
		t->type = Integer;
		match(FLOAT);
	}
	else
	{
		t=newExpNode(VoidK);
		t->type=Void;
		match(VOID);
	}
	return t;
}

TreeNode *params(void)
{
	TreeNode *t=NULL;
	if(token==VOID)
	{
		t=newExpNode(VoidK);
		t->type=Void;
		match(VOID);
	}
	else if (token == RPAREN)
	{
		t=newExpNode(VoidK);
		t->type=Void;
	}
	else
		t=param_list();
	return t;
}

TreeNode *param_list(void)
{
	TreeNode *t=param();
	TreeNode *q=t;
	while(token==COMOS)
	{
		match(COMOS);
		TreeNode *p=param();
		q->sibling=p;
		q=p;
	}
	return t;
}

TreeNode *param(void)
{
	TreeNode *t=type_specifier();
	if(token==ID)
	{
		t->attr.name=strdup(tokenString);
		match(ID);
	}
	if(token==LQUATA)
	{
		t->kind.exp=ArrayK;
		match(LQUATA);
		match(RQUATA);
	}
	return t;
}


TreeNode *compound_stmt(void)
{
	match(LCIR);
	TreeNode *t=local_declarations();
	TreeNode *q=statement_list();
	TreeNode *p = t;
	while(p->sibling!=NULL)
		p=p->sibling;
	p->sibling=q;
	match(RCIR);
	return t;
}

TreeNode *local_declarations(void)
{
	TreeNode *head=newExpNode(EmptyK);
	TreeNode *q=head;
	TokenType temp_token;
	if (token == CONST)
	{
		match(CONST);
		head = const_declaration();
		q = head->sibling;
	}	
	while (token == CONST)
	{
		match(CONST);
		TreeNode *p=const_declaration();
		q->sibling=p;
		q=p->sibling;		//q=p;
	}	
	if((token==INT)||(token==VOID)||(token==CHAR)||(token==FLOAT))
	{
		if (head->kind.exp == EmptyK)	//there is no const declared
		{
			temp_token = token;
			head=var_declaration();
			q=head;
		}
		else							
		{
			temp_token = token;
			TreeNode *p=var_declaration();
			q->sibling=p;
			q=p;	
		}
		while (token == COMOS)
		{
			token = temp_token;
			TreeNode *next = var_declaration();
			q->sibling = next;
			q = next;		 
		}
	}
	if (token == SEMI)
	{
		//printf("first dec matches\n");
		match(SEMI);
	}
	//printf("outside while");
	while((token==INT)||(token==VOID)||(token==CHAR)||(token==FLOAT))
	{
		//printf("Inside while");
		temp_token = token;
		TreeNode *p=var_declaration();
		q->sibling=p;
		q=p;
	
		while (token == COMOS)
		{
			token = temp_token;	
			TreeNode *next = var_declaration();
			q->sibling = next;
			q = next;
		}
		match(SEMI);	
		if (token == SEMI)
		{
		//	printf("it is really a SEMI\n");
			match(SEMI);
		}
	}
	//printf("the next starting is %s\n", tokenString);
	return head;
}

TreeNode *var_declaration(void)
{
	TreeNode *t=type_specifier();
	if(token==ID)
	{
		t->attr.name=strdup(tokenString);
		match(ID);
	}
	if(token==LQUATA)
	{
		match(LQUATA);
		t->kind.exp=IntarrayK;
		TreeNode *n=newExpNode(ConstK);
		n->attr.val=atoi(tokenString);
		t->child[0]=n;
		//t->attr.val=atoi(tokenString);
		match(token);
		match(RQUATA);
	}
//	match(SEMI);
	//printf("successfule\n");
	return t;
}

TreeNode *const_declaration(void)
{
	TokenType temp_token = token;
	TreeNode *t=type_specifier();
/*
	if (temp_token == INT)
		t->type = Const_Int;
	else if (temp_token == FLOAT)
		t->type = Const_Float;
	else if (temp_token == CHAR)
		t->type = Const_Char;

	switch(temp_token)		//change type to Const_*
	{
		printf("Inside change type to Const_*!!!\n");
		case INT:t->type = Const_Int;break;
		case CHAR:t->type = Const_Char;break;
		case FLOAT:t->type = Const_Float;break;
		default:break;
	}
*/
	if(token==ID)
	{
		t->attr.name=strdup(tokenString);
		//printf("the name is: %s\n", t->attr.name);
		match(ID);
	}
	match(ASSIGN);
	if (temp_token == INT)
	{
		//printf("before readin const num\n");
		TreeNode *n = newExpNode(ConstK);

		n->attr.val = atoi(tokenString);

		TreeNode *p = newExpNode(OpK);
		p->attr.op = ASSIGN;

		TreeNode *t2 = (TreeNode*)malloc(sizeof(TreeNode));
		memcpy(t2, t, sizeof(TreeNode));
		p->child[0] = t2;
		p->child[1] = n;
	
		t->sibling = p;

/*
	TreeNode *p = newExpNode(OpK);
	p->attr.op = EQ;
	p->child[0] = exp;
	p->child[1] = case_val;
	
	t->child[0] = p;
	t->child[1] = case_stmt();
*/

		//printf("after readin const num\n");
	}
	else if (temp_token == FLOAT)
	{
		TreeNode *n = newExpNode(ConstK);
		n->attr.val = (int)atof(tokenString);
		n->attr.val_f = (float)atof(tokenString);
		t->child[0]=n;
	}
	else if (temp_token == CHAR)
	{
		TreeNode *n = newExpNode(ConstK);
		n->attr.val_c = tokenString[0];
		t->child[0]=n;
	}
	match(token);
	match(SEMI);
	//printf("finished const dec\n");
	/*
	if(token==LQUATA)
	{
		match(LQUATA);
		t->kind.exp=IntarrayK;
		TreeNode *n=newExpNode(ConstK);
		n->attr.val=atoi(tokenString);
		t->child[0]=n;
		//t->attr.val=atoi(tokenString);
		match(token);
		match(RQUATA);
	}
	*/
//	match(SEMI);
	//printf("successfule\n");
	//printf("pass the var_declartion()\n");
	return t;
}
/*
TreeNode *local_declarations(void)
{
	//printf("In the local declaration\n");
	TreeNode *t=newExpNode(EmptyK);
	TreeNode *tail;
	
	if((token==INT)||(token==VOID)||(token==CHAR)||(token==FLOAT))
	{
		t=var_declaration(&tail);
	//	tail=t;		// t is the head, tail is the last 
	}
	printf("outside while\n");
	while((token==INT)||(token==VOID)||(token==CHAR)||(token==FLOAT))
	{
		printf("Inside while");
		TreeNode *p=var_declaration(&tail);
		//tail->sibling=p;
		//tail=p;
	}
	
//	printf("tail name is: %s\n", tail->attr.name);
	printf("pass the local_declaration()\n");
	return t;
}

TreeNode *var_declaration(TreeNode **tail)
{
	//printf("In the var declartion\n");
	TreeNode *t=type_specifier();		// fill in var type 
	*tail = t;
	
	if(token==ID)
	{
		t->attr.name=strdup(tokenString);
		printf("token == ID:%s\n", t->attr.name);
		match(ID);
	}
	if(token==LQUATA)
	{
		match(LQUATA);
		t->kind.exp=IntarrayK;
		TreeNode *n=newExpNode(ConstK);
		n->attr.val=atoi(tokenString);
		t->child[0]=n;
		//t->attr.val=atoi(tokenString);
		match(token);
		match(RQUATA);
	}

	while (token == COMOS)
	{
		printf("before match comos\"%s\"\n", tokenString);
		match(COMOS);
		printf("after match comos\"%s\"\n", tokenString);
		if (token == ID)
		{
			TreeNode *p;
			switch(t->type)
			{
				case Integer:
					p = newExpNode(INT);
					p->type = Integer;
					break;
				case Char:
					token = CHAR; break;
				case Float:
					token = FLOAT; break;
				case Void:
					token = VOID; break;
				default:break;
			}
//			TreeNode *p = type_specifier();
			p->attr.name = strdup(tokenString);
			printf("attr.name %s\n", p->attr.name);
			(*tail)->sibling = p;
			*tail = p;
			match(ID);
		}
	}
	match(SEMI);
	//printf("matched semicolon\n");
	//printf("successful\n");
	//printf("pass the var_declartion()\n");
	return t;
}
*/
TreeNode *statement_list(void)
{
	TreeNode *t = NULL;
	if((token==IF)||(token==WHILE)||(token == DO)||(token==SWITCH)||(token==RETURN)||(token==ID)||(token==LPAREN)||(token==NUM)||(token==OUTPUT)||(token == INPUT))
	{
		t=statement();
	}
	TreeNode *p = t;
	while((token==IF)||(token==WHILE)||(token == DO)||(token == SWITCH)||(token==RETURN)||(token==ID)||(token==LPAREN)||(token==NUM)||(token==OUTPUT) || (token == INPUT))
	{
		//printf("I am in the while of statement_list\n");
		TreeNode *q = statement();
		p->sibling=q;
		p=q;
	}
	return t;
}

TreeNode *statement(void)
{
	TreeNode *t=NULL;
	switch(token)
	{
		case IF:
			t = selection_stmt();
			break;
		case WHILE:
			t = iteration_stmt();
			break;
		case DO:
			t = do_stmt();
			break;
		case SWITCH:
			t = switch_stmt();
			break;
		case RETURN:
			t = return_stmt();
			break;
		case LPAREN:
		case NUM:
		case ID:
		case INPUT:
		case OUTPUT:
			t=expression_stmt();
			break;
		case LCIR:
			t=compound_stmt();
			break;
		default:
			break;
	}
	
	return t;
}

TreeNode *expression_stmt(void)
{
	TreeNode *t=NULL;

	if(token!=SEMI)
		t=expression();
	match(SEMI);

	return t;
}


TreeNode *selection_stmt(void)
{
	TreeNode *t = newStmtNode(IfK);
	match(IF);
	match(LPAREN);
	t->child[0] = expression();
	match(RPAREN);
	t->child[1] = statement();
	if (token==ELSE) {
		match(ELSE);
		t->child[2] = statement();
	}
	//match(SEMI);
	return t;
}


TreeNode *switch_stmt(void)
{
	TreeNode *t = newStmtNode(IfK);
	TreeNode *t_later;
	TreeNode *first_t;
	TreeNode *exp;
	TreeNode *exp_later;

	int first = 1;

	match(SWITCH);
	match(LPAREN);
	exp = additive_expression();
	match(RPAREN);
	
	match(LCIR);
	match(CASE);

	TreeNode *case_val=newExpNode(ConstK);
	case_val->attr.val = atoi(tokenString);
	match(NUM);
	match(COLON);

	TreeNode *p = newExpNode(OpK);
	p->attr.op = EQ;
	p->child[0] = exp;
	p->child[1] = case_val;
	
	t->child[0] = p;
	t->child[1] = case_stmt();

	first_t = t;

//printf("before while\n");
	while (token == CASE)
	{
		match(CASE);
		case_val=newExpNode(ConstK);
		case_val->attr.val = atoi(tokenString);	//atoi()
		match(NUM);
		match(COLON);
//printf("a new if\n");
		t_later = newStmtNode(IfK);

		if (first)
		{
			first_t->sibling = t_later;
			first = 0;
		}

		exp_later = (TreeNode*)malloc(sizeof(TreeNode));
		memcpy(exp_later, exp, sizeof(TreeNode));
		
		p = newExpNode(OpK);
		p->attr.op = EQ;
		p->child[0] = exp_later;
		p->child[1] = case_val;

		t_later->child[0] = p;
		t_later->child[1] = case_stmt();

		t->child[2] = t_later;
		t = t_later;
	}
//printf("out of while\n");
	match(RCIR);
	return first_t;
}

TreeNode *case_stmt(void)
{
	TreeNode *p = statement();
	return p;
}

TreeNode *do_stmt(void)
{
	TreeNode *p = newStmtNode(WhileK);
	match(DO);
	match(LCIR);
	if (p != NULL)
		p->child[1] = statement();
	match(RCIR);
	match(WHILE);
	match(LPAREN);
	p->child[0] = expression();
	match(RPAREN);
	match(SEMI);
	return p;
}
	
TreeNode *iteration_stmt(void)
{
	TreeNode *t = newStmtNode(WhileK);
	match(WHILE);
	match(LPAREN);
	
	if (t!=NULL) 
		t->child[0] =expression();
	match(RPAREN);
	if (t!=NULL) 
		t->child[1] =statement();
	//match(SEMI);
	return t;
}


TreeNode *return_stmt(void)
{
	TreeNode *t = newStmtNode(ReturnK);
	match(RETURN);
	if (token == LPAREN)
		match(LPAREN);
	if(token!=SEMI)
		t->child[0]=expression();
	if (token == RPAREN)
		match(RPAREN);
	match(SEMI);
	return t;
}

TreeNode *expression(void)
{
	TreeNode *t = NULL;

	if((token==ID)&&compareFunc(tokenString))
	{
		t=var();
		if(token==ASSIGN)
		{
			match(ASSIGN);
			TreeNode *p=newExpNode(OpK);
			p->attr.op=ASSIGN;
			p->child[0]=t;
			TreeNode *q=expression();
			p->child[1]=q;
			t=p;
		}
		else
		{
			TreeNode *p =NULL;
			while((token==LTEQ)||(token==LT)||(token==BG)||(token==BGEQ)||(token==EQ)||(token==NEQ))
			{
				p=newExpNode(OpK);
				p->attr.op=token;
				p->child[0]=t;
				match(token);
				TreeNode *q=additive_expression();
				p->child[1]=q;
				t=p;
			}
			
			while((token==PLUS)||(token==MINUS))
			{
				p = newExpNode(OpK);
				p->attr.op=token;
				p->child[0]=t;
				match(token);
				TreeNode *q = term();
				p->child[1]=q;
				t=p;
			}
			
			while((token==TIMES)||(token==OVER))
			{
				TreeNode *p = newExpNode(OpK);
				p->attr.op=token;
				p->child[0]=t;
				match(token);
				TreeNode *q=factor();
				p->child[1]=q;
				t=p;
			}
		}
	}
	else
	{
		t=simple_expression();
	}
	
	return t;
}

TreeNode *var(void)
{
	TreeNode *t = NULL;
	
	t = newExpNode(IdK);
	t->attr.name=strdup(tokenString);
	match(token);
	if(token==LQUATA)
	{
		match(LQUATA);
		t->kind.exp=ArrayK;
		if(token!=RQUATA)
		{
			TreeNode *q = expression();
			t->child[0]=q;
		}
		else
		{
			t->child[0]=NULL;
		}
		match(RQUATA);
	}
	return t;
}


TreeNode *simple_expression(void)
{
	TreeNode *t = additive_expression();
	TreeNode *p = NULL;
	while((token==LTEQ)||(token==LT)||(token==BG)||(token==BGEQ)||(token==EQ)||(token==NEQ))
	{
		p=newExpNode(OpK);
		p->attr.op=token;
		p->child[0]=t;
		match(token);
		TreeNode *q=additive_expression();
		p->child[1]=q;
		t=p;
	}
	return t;
}

TreeNode *additive_expression(void)
{
	TreeNode *t = term();
	TreeNode *p = NULL;
	while((token==PLUS)||(token==MINUS))
	{
		p = newExpNode(OpK);
		p->attr.op=token;
		p->child[0]=t;
		match(token);
		TreeNode *q = term();
		p->child[1]=q;
		t=p;
	}
	return t;
}

TreeNode *term(void)
{
	TreeNode *t=factor();
	while((token==TIMES)||(token==OVER))
	{
		TreeNode *p = newExpNode(OpK);
		p->attr.op=token;
		p->child[0]=t;
		match(token);
		TreeNode *q=factor();
		p->child[1]=q;
		t=p;
	}
	return t;
}

TreeNode *factor(void)
{
	TreeNode *t=NULL;
	if(token==LPAREN)
	{
		match(LPAREN);
		t=expression();
		match(RPAREN);
	}
	else if((token==ID)||(token==INPUT)||(token==OUTPUT))
	{
		if(!compareFunc(tokenString))
			t=call();
		else
			t=var();
	}
	else if (token == NUM)
	{
		t=newExpNode(ConstK);

		if (!isdigit(tokenString[0]) && ((tokenString[0] != '+') && (tokenString[0] != '-') && (tokenString[0] != '*')))
			t->attr.val = (int)tokenString[0];
		else
			t->attr.val = atoi(tokenString);
//printf("%s, %c, %d, %d",tokenString, tokenString[0], tokenString[0], (int)tokenString[0]);


//		t->attr.val=atoi(tokenString);
		match(NUM);
	}
	else if(token == NUM_F)
	{
		t=newExpNode(ConstK);
		t->attr.val = (int)atof(tokenString);
		printf("%s, %d", tokenString, t->attr.val);
		t->attr.val_f=(float)atof(tokenString);
		//printf("I got a float val:%f\n", t->attr.val_f);
		match(NUM_F);
	}
	return t;
}

TreeNode *call(void)
{
	TreeNode *t= NULL;
	if(token==INPUT)
	{
		t=newStmtNode(InputK);
		t->attr.name=strdup(tokenString);
	}
	else if(token==OUTPUT)
	{
		t=newStmtNode(OutputK);
		t->attr.name=strdup(tokenString);
	}
	else
	{
		t=newExpNode(FuncK);
		t->attr.name=strdup(tokenString);
	}
	match(token);
	match(LPAREN);
	TreeNode *p=args();
	t->child[0]=p;
	return t;
}

TreeNode *args(void)
{
	TreeNode *t=newExpNode(EmptyK);
	if(token!=RPAREN)
		t=arg_list();
	match(RPAREN);
	//match(SEMI);
	return t;
}

TreeNode *arg_list(void)
{
	TreeNode *t=expression();
	TreeNode *p=t;
	while(token==COMOS)
	{
		match(COMOS);
		TreeNode *q=expression();
		p->sibling=q;
		p=q;
	}
	return t;
}

/****************************************/
TreeNode *parse(void)
{
	initfunc();
	TreeNode *t;
	token = getToken();
	t = declaration_list();
	if (token!=ENDFILE)
		syntaxError("Oops, source ends illegally\n");
	return t;
}

