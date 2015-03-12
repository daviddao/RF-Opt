/*
    Copyright (C) 2015 Tomas Flouri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact: Tomas Flouri <Tomas.Flouri@h-its.org>,
    Heidelberg Institute for Theoretical Studies,
    Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany
*/
%{
#include "rfopt.h"

extern int yylex();
extern FILE * yyin;
extern void yylex_destroy();

int yywrap() 
{ 
  return 1;
}

void yyerror(tree_node_t * tree, const char * s) 
{
  fprintf(stderr, "%s.\n", s);
}

%}


%union
{
  char * s;
  char * d;
  struct tree_noderec * tree;
}

%error-verbose
%parse-param {struct tree_noderec * tree}
%destructor { yy_dealloc_tree($$); } subtree

%token OPAR
%token CPAR
%token COMMA
%token COLON SEMICOLON 
%token<s> STRING
%token<d> NUMBER
%type<s> label optional_label
%type<d> number optional_length
%type<tree> subtree
%start input
%%

input: OPAR subtree COMMA subtree COMMA subtree CPAR optional_label optional_length SEMICOLON
{
  tree->next               = yy_create_tree();
  tree->next->next         = yy_create_tree();

  tree->back               = $2;
  tree->next->back         = $4;
  tree->next->next->back   = $6;

  tree->label              = $8;
  tree->next->label        = $8;
  tree->next->next->label  = $8;

  tree->length             = $2->length;
  tree->next->length       = $4->length;
  tree->next->next->length = $6->length;
  free($9);
};

subtree: OPAR subtree COMMA subtree CPAR optional_label optional_length
{
  $$                     = yy_create_tree();
  $$->next               = yy_create_tree();
  $$->next->next         = yy_create_tree();

  $$->next->back         = $2;
  $$->next->next->back   = $4;

  $$->label              = $6;
  $$->next->label        = $6;
  $$->next->next->label  = $6;

  $$->length = $7 ? atof($7) : 0;
  free($7);

  $$->next->length       = $2->length;
  $$->next->next->length = $4->length;
}
       | label optional_length
{
  $$ = yy_create_tree();
  $$->label  = $1;
  $$->length = $2 ? atof($2) : 0;
  $$->next   = NULL;
  free($2);
};

 
optional_label:  { $$ = NULL;} | label  {$$ = $1;};
optional_length: { $$ = NULL;} | COLON number {$$ = $2;};
label: STRING    { $$=$1;};
number: NUMBER   { $$=$1;};

%%

tree_node_t * yy_parse_tree(const char * filename)
{
  struct tree_noderec * tree;

  tree = yy_create_tree();

  yyin = fopen(filename, "r");
  if (!yyin)
  {
    yy_dealloc_tree(tree);
    fatal("Cannot open file %s", filename);
  }
  else if (yyparse(tree))
  {
    yy_dealloc_tree(tree);
    tree = NULL;
    fclose(yyin);
    yylex_destroy();
    fatal("Cannot parse tree file %s (maybe not unrooted?)", filename);
  }
  
  if (yyin) fclose(yyin);

  yylex_destroy();

  return tree;
}
