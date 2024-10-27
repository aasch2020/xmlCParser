#include "stateTree.h"
#include "xmlParse.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
typedef union Extstr // Union for Ext item
{
    struct Node node;
    struct Block blk;
} Extstr;
typedef struct Ext // Temp ext object to create treeified XML
{
    int blk_nod; // is block
    int is_cond; // is cond
    int nest_dep; // nesting depth
    int next_ind_L; // left for child
    int next_ind_R; // right for child
    int blockEnd; //  is end of block
    union Extstr item; // item
} Ext;

StateTree *makeTreefromXML(XMLLine **xlines, int numlines); // xlines to a state tree