#include "stateTree.h"
#include "xmlParse.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
typedef union Extstr // ext unsion struct
{
    struct Node *node;
    struct Block *blk;
} Extstr;
typedef struct Ext // Exxt struct 
{
    int blk_nod; // is it a block
    int is_cond;// is it a cond
    int nest_dep; // nesting depth
    int next_ind_L;// lefrt child index
    int next_ind_R;// right child index
    int blockEnd; //  is it a block ender
    union Extstr item;
} Ext;

StateTree *makeTreefromXML(XMLLine **xlines, int numlines);
