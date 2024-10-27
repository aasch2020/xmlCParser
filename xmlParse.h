#define MAX_NEST = 6

#include <stdio.h> 
#include <stdlib.h>
typedef struct Block{
    int currind;
} Block;

typedef struct XArg
{
    char *name;
    char *val;
} XArg;
typedef struct XMLLine
{
    int ending;
    int argsCount;
    char name[20];
    XArg **argAr;

} XMLLine;
int readalem(char *buf, FILE *fptr);
void printXArg(XArg *arg);
void printXline(XMLLine *xline);
XMLLine *processXMLLine(char *buf, int buflen);
int readintoXMLstruct(FILE *fptr, XMLLine **xlines);
void freeXArg(XArg *xarg);
void freeXMLLine(XMLLine *xline);