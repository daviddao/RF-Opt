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

static int indend_space = 4;

static int tree_indend_level(tree_node_t * tree, int indend);

tree_node_t * yy_create_tree()
{
  tree_node_t * t = xrealloc(0, sizeof(tree_node_t));
  memset(t, 0, sizeof(tree_node_t));
  return t;
}

static void dealloc_tree_recursive(tree_node_t * node)
{
  if (!node->next)
  {
    free(node->label);
    free(node);
    return;
  }

  dealloc_tree_recursive(node->next->back);
  dealloc_tree_recursive(node->next->next->back);

  free(node->next->next);
  free(node->next);
  free(node->label);
  free(node);
}

void yy_dealloc_tree(tree_node_t * root)
{
  if (!root) return;

  if (!root->next)
  {
    free(root->label);
    free(root);
    return;
  }

  free(root->label);
  if (root->next)
    dealloc_tree_recursive(root->next->back);
  if (root->next->next)
    dealloc_tree_recursive(root->next->next->back);
  if (root->back)
    dealloc_tree_recursive(root->back);

  free(root->label);
  free(root->next->next);
  free(root->next);
  free(root);
}

static void print_tree_recurse(tree_node_t * tree, 
                               int indend_level, int * active_node_order)
{
  int i,j;

  if (!tree) return;

  for (i = 0; i < indend_level; ++i)
  {
    if (active_node_order[i])
      printf("|");
    else
      printf(" ");

    for (j = 0; j < indend_space-1; ++j)
      printf(" ");
  }
  printf("\n");

  for (i = 0; i < indend_level-1; ++i)
  {
    if (active_node_order[i])
      printf("|");
    else
      printf(" ");

    for (j = 0; j < indend_space-1; ++j)
      printf(" ");
  }

  printf("+");
  for (j = 0; j < indend_space-1; ++j)
    printf ("-");
  if (tree->next) printf("+");

  printf (" %s:%f\n", tree->label, tree->length);

  if (active_node_order[indend_level-1] == 2) active_node_order[indend_level-1] = 0;

  if (tree->next)
  {
    active_node_order[indend_level] = 1;
    print_tree_recurse(tree->next->back,       indend_level+1, active_node_order);
    active_node_order[indend_level] = 2;
    print_tree_recurse(tree->next->next->back, indend_level+1, active_node_order);
  }

}

static int tree_indend_level(tree_node_t * tree, int indend)
{
  if (!tree->next) return indend+1;

  return MAX(tree_indend_level(tree->next->back,       indend+1),
             tree_indend_level(tree->next->next->back, indend+1));

}

void show_ascii_tree(tree_node_t * tree)
{
  int indend_max = MAX(tree_indend_level(tree->back,1),tree_indend_level(tree,0));
  int * active_node_order = (int *)malloc((indend_max+1) * sizeof(int));
  active_node_order[0] = 1;
  active_node_order[1] = 1;

  //printf (" %s:%f\n", tree->label, tree->length);
  print_tree_recurse(tree->back,             1, active_node_order);
  print_tree_recurse(tree->next->back,       1, active_node_order);
  active_node_order[0] = 2;
  print_tree_recurse(tree->next->next->back, 1, active_node_order);
  free(active_node_order);
}
