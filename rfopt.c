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

#include "rfopt.h"

static char * progname;
static char progheader[80];
static char * cmdline;

void args_init(int argc, char ** argv)
{
  progname = argv[0];
}

void getentirecommandline(int argc, char * argv[])
{
  int len = 0;
  int i;

  for (i = 0; i < argc; ++i)
    len += strlen(argv[i]);

  cmdline = (char *)xmalloc(len + argc + 1);
  cmdline[0] = 0;

  for (i = 0; i < argc; ++i)
  {
    strcat(cmdline, argv[i]);
    strcat(cmdline, " ");
  }
}

void fillheader()
{
  snprintf(progheader, 80,
           "%s %s_%s",
           PROG_NAME, PROG_VERSION, PROG_ARCH);
}

void show_header()
{
  fprintf(stdout, "%s\n", progheader);
  fprintf(stdout, "https://github.com/daviddao/RF-Opt\n");
  fprintf(stdout,"\n");
}

void cmd_showtree(const char * filename)
{
  tree_node_t * tree = yy_parse_tree(filename);  

  if (!tree)
    fatal("Tree is probably not unrooted.\n");

  show_ascii_tree(tree);
  yy_dealloc_tree(tree);
}


int main(int argc, char * argv[])
{
  fillheader();
  getentirecommandline(argc, argv);

  args_init(argc, argv);

  show_header();

  if (argc != 2)
  {
    fprintf(stderr,  "syntax: %s [NEWICK-FILE]\n", argv[0]);
    return (EXIT_FAILURE);
  }

  cmd_showtree(argv[1]);
  free(cmdline);
  printf("Here\n");

  return EXIT_SUCCESS;
}
