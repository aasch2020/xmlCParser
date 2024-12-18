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
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->uniontype = 0;
    newNode->node.fac.currind = index;
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
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->uniontype = 1;
    newNode->node.cond.currind = index;
    newNode->node.cond.trueChild = lchild;
    newNode->node.cond.falseChild = rchild;
    return newNode;
}

Node *createSent(int isHead, Node *next)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->uniontype = 2;
    newNode->node.sent.isHead = isHead;
    newNode->node.sent.next = next;
    return newNode;
}

StateTree *init_state_tree()
{
    StateTree *sTree = (StateTree *)malloc(sizeof(StateTree));
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
    Node *base = tree->current;
    if (tree->current == NULL)
    {
        // default to insert after head
        insertAfter(tree->root, insert);
        return 0;
    }
    else
    {
        insertAfter(tree->current, insert);
    }
}

void fulliter(StateTree *tree)
{
    Node *iter = tree->root;
    int iterct = 0;
    while (iter != NULL)
    {
        printf("iter->uniontype is %d, num in  = %d\n", iter->uniontype, iterct);
        iterct++;
        if (iter->uniontype == 0)
        {
            printf("node index %d, type fileAccess, Access level %c, filename: %s\n", iter->node.fac.currind, iter->node.fac.accessType, iter->node.fac.filename);
            iter = iter->node.fac.next;
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
            iter = iter->node.sent.next;
        }
        else
        {
            printf("this is a conditional, we should never get here\n");
            break;
        }
    }
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
    return 1;
}

int main()
{
    StateTree *testTree = init_state_tree();
    fulliter(testTree);

    char *filename = "swagfile.txt";

    insertAfterCurrent(testTree, createFileAccess(0, filename, 'R'));
    fulliter(testTree);

    //
    //

    freetree(testTree);
}