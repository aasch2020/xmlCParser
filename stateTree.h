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
    int uniontype; // 0=fac, 2=sent, 1= cond
    int currind;
    union NodeUnion node;
} Node;

// Function to create a new node

Node *createFileAccess(int index, char *filename, char accessType);

Node *returnAndStep(StateTree *tree);

int insertAfter(Node *base, Node *child);

int insertAfterCurrent(StateTree *tree, Node *insert);

int freetree(StateTree *tree);

StateTree *init_state_tree();

void fulliter(StateTree *tree);

void pretty_print_conditions(const Conditions *cond);
void pretty_print_sentinel(const Sentinel *sent);
void pretty_print_file_access(const FileAccess *fac);
void pretty_print_node(const Node *node);