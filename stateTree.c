// C program to to implement binary tree

#include <stdio.h>
#include <stdlib.h>
#include "stateTree.h"
// #include <linux/string.h>
#include <string.h>

// Define a structure for tree nodes

// Function to create a new node
Node *createFileAccess(int index, char *filename, char accessType)
{
    Node *newNode = (Node *)calloc(1, sizeof(Node));
    newNode->uniontype = 0;
    newNode->currind = index;
    newNode->node.fac.accessType = accessType;
    int len = strlen(filename);
    if (len > 4095)
    {
        free(newNode);
        return 0;
    }
    memcpy(newNode->node.fac.filename, filename, len);
    return newNode;
}

// bad spaghetti proto
Node *createConditions(int index, struct Node *lchild, struct Node *rchild)
{
    Node *newNode = (Node *)calloc(1, sizeof(Node));
    newNode->uniontype = 1;
    newNode->currind = index;
    newNode->node.cond.trueChild = lchild;
    newNode->node.cond.falseChild = rchild;
    return newNode;
}

Node *createSent(int isHead, Node *next)
{
    Node *newNode = (Node *)calloc(1, sizeof(Node));
    newNode->uniontype = 2;
    newNode->node.sent.isHead = isHead;
    newNode->node.sent.next = next;
    return newNode;
}

StateTree *init_state_tree()
{
    StateTree *sTree = (StateTree *)calloc(1, sizeof(StateTree));
    Node *endtinel = createSent(0, NULL);
    Node *begintinel = createSent(1, endtinel);
    sTree->root = begintinel;
    sTree->end = endtinel;
    sTree->current = NULL;
    return sTree;
}

int insertAfter(Node *base, Node *child)
{
    if (base->uniontype == 0)
    {
        Node *oldnext = base->node.fac.next;
        child->node.fac.next = oldnext;
        base->node.fac.next = child;
        return 1;
    }
    else if (base->uniontype == 2)
    {
        Node *oldnext = base->node.sent.next;
        child->node.fac.next = oldnext;
        base->node.sent.next = child;
        return 1;
    }
    else
    {
        return 0;
    }
}

int insertAfterCurrent(StateTree *tree, Node *insert)
{
    if (tree->current == NULL)
    {
        // default to insert after head
        insertAfter(tree->root, insert);
        return 0;
    }
    else
    {
        insertAfter(tree->current, insert);
        return 0;
    }

    return 0;
}


void rcriter(Node *iter )
{
    int iterct = 0;
    if(iter == NULL){
        return;
    }else{
        iterct++;
        if (iter->uniontype == 0)
        {
            printf("node index %d, type fileAccess, Access level %c, filename: %s, next is %p\n", iter->currind, iter->node.fac.accessType, iter->node.fac.filename, iter->node.fac.next);
            rcriter( iter->node.fac.next);
        }
        else if (iter->uniontype == 2)
        {
            if (!iter->node.sent.isHead)
            {

                printf("tail found\n");
            }
            else
            {
                printf("head found\n");
            }
            rcriter( iter->node.sent.next);
        }
        else
        {
            printf("hit a conditional at index %d children are %p, %p\n\n\n", iter->currind, (void*)iter->node.cond.trueChild, (void *)iter->node.cond.trueChild );
            rcriter(iter->node.cond.trueChild);
            rcriter(iter->node.cond.falseChild);

            printf("exiting the conditoin\n\n\n");
        }
    }
}
void fulliter(StateTree *tree)
{
    printf("tail next is (this is bad!!) %p\n", tree->end->node.sent.next);
    rcriter(tree->root);
}
Node *returnAndStep(StateTree *tree)
{
    Node *retr = tree->current;
    if (retr->uniontype == 0)
    {
        tree->current = retr->node.fac.next;
    }
    else if (retr->uniontype == 2)
    {
        if (!retr->node.sent.isHead)
        {
            return 0;
        }
        else
        {
            tree->current = retr->node.fac.next;
        }
    }
    else
    {

        return 0;
    }

    return retr;
}

int freetree(StateTree *tree)
{
    Node *iter = tree->root;
    while (iter != tree->end)
    {
        Node *todel = iter;
        if (iter->uniontype == 0)
        {
            iter = iter->node.fac.next;
            free(todel);
        }
        else if (iter->uniontype == 2)
        {

            iter = iter->node.sent.next;
            free(todel);
        }
        else
        {

            return 0;
        }
    }
    free(iter);
    free(tree);
    return 1;
}

void pretty_print_conditions(const Conditions *cond)
{
    printf("Conditions:\n");
    printf("  True Child: %p\n", (void *)cond->trueChild);
    printf("  False Child: %p\n", (void *)cond->falseChild);
}

void pretty_print_sentinel(const Sentinel *sent)
{
    printf("Sentinel:\n");
    printf("  Is Head: %d\n", sent->isHead);
    printf("  Next: %p\n", (void *)sent->next);
}

void pretty_print_file_access(const FileAccess *fac)
{
    printf("File Access:\n");
    printf("  Filename: %s\n", fac->filename);
    printf("  Access Type: %c\n", fac->accessType);
    printf("  Next: %p\n", (void *)fac->next);
}

void pretty_print_node(const Node *node)
{
    printf("Node:\n");
    printf("  Current Index: %d\n", node->currind);
    printf("  Union Type: %d\n", node->uniontype);

    switch (node->uniontype)
    {
    case 0:
        pretty_print_file_access(&node->node.fac);
        break;
    case 1:
        pretty_print_conditions(&node->node.cond);
        break;
    case 2:
        pretty_print_sentinel(&node->node.sent);
        break;
    default:
        printf("  Unknown union type\n");
        break;
    }
}