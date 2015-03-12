# RF-Opt

## Introduction

The aim of the project is to implement a fast tree correction algorithm for
reducing the RF distance between a large taxonomy and a large set of smaller
but more accurate phylogenies.
The implementation should:

* have an open source code with an appropriate source license.
* 64-bit multi-threaded design that handles very large datasets.
* multi-thread support.

## RF-Opt license and third party licenses

The code is currently licensed under the GNU Affero General Public License version 3.

## Code

The code is currently written in C.

    File     | Description
-------------|------
**rfopt.c** | Main file handling command-line parameters and executing corresponding parts.
**tree.c** | Functions on the tree structure.
**util.c** | Various common utility functions.
**Makefile** | Makefile
**unrooted.y** | Bison grammar file for newick binary unrooted tree parsing.
**lex.l** | Flex lexical analyzer.

## Bugs

The source code has not been tested comprehensively yet. All bug reports are highly appreciated.

## The RF-Opt team

The following people have contributed to RF-Opt. In alphabetic order:

* David Dao
* Tom&aacute;&scaron; Flouri
* Kassian Kobert
* Alexandros Stamatakis
