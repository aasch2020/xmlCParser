#define MAX_NEST = 6

#include <stdio.h> 
#include "stateTree.h"
#include <stdlib.h>
typedef struct Parser
{
    struct Node *root;
    struct Node *current;
    struct Node *end;
} Parser;
typedef struct Block{
    int currind;
} Block;


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



StateTree* readFile(FILE* fptr);