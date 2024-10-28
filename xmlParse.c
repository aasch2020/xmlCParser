// C program to to implement binary tree
#include "xmlParse.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// #include <linux/string.h>
#include <string.h>

// This is something that will be edited, as we will need a kernel read. TLDR, this is just to read the next line and increment some sort of seek counter
int readalem(char *buf, FILE *fptr)
{
    fgets(buf, 1024, fptr);
    return strlen(buf);
}
void printXArg(XArg *arg)
{
    printf("arg: %s=%s\n", arg->name, arg->val);
}
void printXline(XMLLine *xline)
{
    printf("xline is ending %d\n", xline->ending);
    printf("xline has args %d\n", xline->argsCount);
    printf("xline has name %s\n", xline->name);
    printf("lsing args\n");
    for (int x = 0; x < xline->argsCount; x++)
    {
        printXArg(xline->argAr[x]);
    }
}

void freeXArg(XArg *xarg)
{
    free(xarg->name);
    free(xarg->val);
    free(xarg);
}
void freeXMLLine(XMLLine *xline)
{
    for (int x = 0; x < xline->argsCount; x++)
    {
        freeXArg(xline->argAr[x]);
    }
    free(xline->argAr);

    free(xline);
}

XMLLine *processXMLLine(char *buf, int buflen)
{
    XMLLine *xline = (XMLLine *)calloc(1, sizeof(XMLLine));

    int cleanbreak = 0;
    int intinbuf = 0;
    while (buf[intinbuf] == ' ') // eat whitespaces
    {
        intinbuf++;
    }
    if (buf[intinbuf] != '<') // eat leading caret
    {
        printf("exiting xline bad 1 \n");
        freeXMLLine(xline);
        return NULL;
    }
    intinbuf++;
    if (buf[intinbuf] == '/') // eat dash to check if its a closing tag
    {
        xline->ending = 1;
        intinbuf++;
    }
    else
    {
        xline->ending = 0;
    }
    int namctr = 0;
    while (buf[intinbuf] != ' ' && buf[intinbuf] != '>' && intinbuf < buflen) // eat the name, until a whitespace or a closing caret
    {
        xline->name[namctr] = buf[intinbuf];
        intinbuf++;
        namctr++;
    }
    intinbuf++;
    xline->name[namctr] = '\0';
    if (xline->ending == 1) // closing tags have no attributes
    {
        xline->argAr = NULL;
        xline->argsCount = 0;
        printf("ending line found\n");
        printXline(xline);
        return xline;
    }
    XArg **argArray = (XArg **)calloc(1, sizeof(XArg *) * 10); // Up to 10 args, smarten this later
    int argct = 0;
    printf("pre attr intinbuf%d\n", intinbuf);
    while ((buf[intinbuf] != '/' && buf[intinbuf] != '>') && intinbuf + 1 < buflen) // until we hit the end of the string
    {
        printf("can we iter args?\n");

        XArg *arg = (XArg *)calloc(1, sizeof(XArg));
        char *argn = (char *)calloc(1, 30 * sizeof(char)); // Smarten this later
        int toCtr = 0;
        while (buf[intinbuf] != '=' && intinbuf < buflen) // until we his the equals eat the argname
        {
            argn[toCtr] = buf[intinbuf];
            toCtr++;
            intinbuf++;
        }
        argn[toCtr] = '\0';
        arg->name = argn;
        intinbuf++;
        printf("arg name is %s\n", argn);

        if ((buf[intinbuf] != '"')) // eat opening quote
        {
            freeXMLLine(xline);
            return NULL;
        }
        intinbuf++;

        char *argv = (char *)calloc(1, 30 * sizeof(char)); // Smarten this later
        toCtr = 0;
        while ((buf[intinbuf] != '\"') && intinbuf < buflen) // until closing quote eat chars for argval
        {
            argv[toCtr] = buf[intinbuf];
            toCtr++;
            intinbuf++;
        }
        argv[toCtr] = '\0';
        arg->val = argv;
        printf("arg val is %s\n", argv);
        intinbuf++; // eat the quote
        argArray[argct] = arg;
        argct++;
        printf("foundan arg\n");
        printXArg(arg);

        if (buf[intinbuf] != ' ') // check for space (well formatted)
        {
            freeXMLLine(xline);
            return NULL;
        }
        intinbuf++;
    }
    if (buf[intinbuf] == '/') // check if closing back tag
    {
        xline->ending = 1;
    }
    else
    {
        xline->ending = 0;
    }

    xline->argAr = argArray;
    xline->argsCount = argct;
    return xline;
}
int readintoXMLstruct(FILE *fptr, XMLLine **xlines) // loop through a file reading lines to make the xlines
{
    struct stat fileStat;
    fstat(fileno(fptr), &fileStat);
    int readed = 0;
    long long fsize = fileStat.st_size;
    char buf[4096];
    int xddd = 0;
    int xnow = 0;
    int totes = 0;
    while (!feof(fptr))
    {
        xddd = readalem(buf, fptr);
        readed += xddd;

        xlines[xnow] = processXMLLine(buf, xddd);
        xnow++;
        totes++;
    }
    return totes;
}
