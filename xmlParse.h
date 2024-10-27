#define MAX_NEST = 6

#include <stdio.h>
#include <stdlib.h>
// Temp struct for block struct
typedef struct Block
{
    int currind;
} Block;

// XML Argument
typedef struct XArg
{
    char *name;
    char *val;
} XArg;
//XML Line
typedef struct XMLLine
{
    int ending;
    int argsCount;
    char name[20];
    XArg **argAr;

} XMLLine;
int readalem(char *buf, FILE *fptr); // Abstraction to read an XML line from a block device. TBD for kernel world
void printXArg(XArg *arg); // Print helper
void printXline(XMLLine *xline); // Print helper
XMLLine *processXMLLine(char *buf, int buflen); // turn a buffer line into an XML line object. !! THIS IS SENSITIVE TO FORMATTING. 
int readintoXMLstruct(FILE *fptr, XMLLine **xlines); // Read a file into an aXML line list
void freeXArg(XArg *xarg); // Free Xarg
void freeXMLLine(XMLLine *xline); // Free Xline and underlying xarg