#define MAX_PATH = 4096

typedef struct StateTree  // Tree struct
{
    struct Node *root;
    struct Node *current;
    struct Node *end;
} StateTree;

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

Node *createFileAccess(int index, char *filename, char accessType); // create a file access node

Node *returnAndStep(StateTree *tree); // for the state machine consumer, 

int insertAfter(Node *base, Node *child); // insert after a noe

int insertAfterCurrent(StateTree *tree, Node *insert); // insert after the current node

int freetree(StateTree *tree); // free a tree

StateTree *init_state_tree(); // initialize a tree with sentinels

void fulliter(StateTree *tree); // iterate the entire tree

//prints

void pretty_print_conditions(const Conditions *cond);
void pretty_print_sentinel(const Sentinel *sent);
void pretty_print_file_access(const FileAccess *fac);
void pretty_print_node(const Node *node);