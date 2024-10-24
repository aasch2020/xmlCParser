#define MAX_PATH = 4096

typedef struct StateTree
{
    struct Node *root;
    struct Node *current;
    struct Node *end;
} StateTree;
// File for state "Tree", due to no conditional support yet is essentially a linked list

typedef struct Conditions
{
    int currind;
    struct Node *trueChild;
    struct Node *falseChild;
} Conditions;

// IMPORTANT pending support of
typedef struct Sentinel
{
    int isHead;
    struct Node *next;
} Sentinel;
typedef struct FileAccess
{
    int currind;
    char filename[4096];
    char accessType;
    struct Node *next;
} FileAccess;
typedef union NodeUnion
{
    struct Conditions cond;
    struct FileAccess fac;
    struct Sentinel sent;
} NodeUnion;
typedef struct Node
{
    int uniontype;
    NodeUnion node;
} Node;

// Function to create a new node

Node *createFileAccess(int index, char *filename, char accessType);

Node *returnAndStep(StateTree *tree);

int insertAfter(Node *base, Node *child);

int insertAfterCurrent(StateTree *tree, Node *insert);