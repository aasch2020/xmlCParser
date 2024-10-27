#include "stateTree.h"
#include "xmlParse.h"
#include <stdio.h> 
#include <stdlib.h>

#include <string.h>
typedef union Extstr{
    struct Node node;
    struct Block blk;
} Extstr;
typedef struct Ext{
    int blk_nod;
    int is_cond;
    int nest_dep;
    int next_ind_L;
    int next_ind_R;
    int blockEnd;
    union Extstr item;
} Ext;

StateTree* makeTreefromXML(XMLLine **xlines, int numlines);