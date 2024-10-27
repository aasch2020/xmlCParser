// C program to to implement binary tree
#include "xmlParse.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// #include <linux/string.h>
#include <string.h>
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
XMLLine *processXMLLine(char *buf, int buflen)
{
    XMLLine *xline = (XMLLine *)malloc(sizeof(XMLLine));
    int cleanbreak = 0;
    int intinbuf = 0;
    while(buf[intinbuf] == ' '){
        intinbuf++;
    }
    if (buf[intinbuf] != '<')
    {
        printf("exiting xline bad 1 \n");
        return NULL;
    }
    intinbuf++;
    if (buf[intinbuf] == '/')
    {
        xline->ending = 1;
        intinbuf++;
    }
    else
    {
        xline->ending = 0;
    }
    int namctr = 0;
    while (buf[intinbuf] != ' ' && buf[intinbuf] != '>' && intinbuf < buflen)
    {
        xline->name[namctr] = buf[intinbuf];
        intinbuf++;
        namctr++;
    }
    intinbuf++;
    xline->name[namctr] = '\0';
    if (xline->ending == 1)
    {
        xline->argAr = NULL;
        xline->argsCount = 0;
        printf("ending line found\n");
        printXline(xline);
        return xline;
    }
    XArg **argArray = (XArg **)malloc(sizeof(XArg *) * 10); // Up to 10 args, smarten this later
    int argct = 0;
    printf("pre attr intinbuf%d\n", intinbuf);
    while ((buf[intinbuf] != '/' && buf[intinbuf] != '>') && intinbuf + 1 < buflen)
    {
        printf("can we iter args?\n");

        XArg *arg = (XArg *)malloc(sizeof(XArg));
        char *argn = (char *)malloc(30 * sizeof(char)); // Smarten this later
        int toCtr = 0;
        while (buf[intinbuf] != '=' && intinbuf < buflen)
        {
            argn[toCtr] = buf[intinbuf];
            toCtr++;
            intinbuf++;
        }
        argn[toCtr] = '\0';
        arg->name = argn;
        intinbuf++;
        printf("arg name is %s\n", argn);

        if ((buf[intinbuf] != '"'))
        {
            return NULL;
        }
        intinbuf++;

        char *argv = (char *)malloc(30 * sizeof(char)); // Smarten this later
        toCtr = 0;
        while ((buf[intinbuf] != '\"') && intinbuf < buflen)
        {
            argv[toCtr] = buf[intinbuf];
            toCtr++;
            intinbuf++;
        }
        argv[toCtr] = '\0';
        arg->val = argv;
        printf("arg val is %s\n", argv);
        intinbuf++;
        argArray[argct] = arg;
        argct++;
        printf("foundan arg\n");
        printXArg(arg);

        if (buf[intinbuf] != ' ')
        {
            return NULL;
        }
        intinbuf++;
    }
    if (buf[intinbuf] == '/')
    {
        xline->ending = 1;
    }
    else
    {
        xline->ending = 0;
    }

    xline->argAr = argArray;
    xline->argsCount = argct;
    printf("happe exit xline\n");
    return xline;
}

void pretty_print_block(const Block *block) {
    printf("Block Struct:\n");
    printf("  currind : %d\n", block->currind);
}
void printExt(Ext *ext){
    printf("Ext Struct:\n");
    printf("  blk_nod    : %d\n", ext->blk_nod);
    printf("  is_cond    : %d\n", ext->is_cond);
    printf("  nest_dep   : %d\n", ext->nest_dep);
    printf("  next_ind_L : %d\n", ext->next_ind_L);
    printf("  next_ind_R : %d\n", ext->next_ind_R);
    printf("  blockEnd   : %d\n", ext->blockEnd);
    if(ext->blk_nod ==1 ){
        pretty_print_block(&(ext->item.blk));
    }
    
}


void readBlock(char *buf, int buflen, int *nestdep, Ext *myext, XMLLine* xline)
{
    Extstr retval = myext->item;

    myext->item.blk.currind =  strtol(xline->argAr[0]->val, NULL, 10);
    *nestdep+=1;
    myext->is_cond= 0;
    myext->nest_dep = *nestdep;
    myext->next_ind_L = 0;
    myext->next_ind_R = 0;
    myext->blk_nod = 1;
}
void readNodeAttrLine(){

}
// void readNode(char *buf, int buflen, int nestdep, Ext *myext, FILE *fptr, int *readed, XMLLine* xline)
// {
//     Extstr node = myext->item;
//     if (xline->argsCount != 2){
//         return NULL;
//     }
//     myext->blk_nod  = 0;
//     myext->blockEnd = 0;
//     myext->nest_dep = nestdep;
//     myext->next_ind_L = 0;
//     myext->next_ind_R = 0;
//     myext->is_cond= 0;
//     myext->item.node.currind = strtol(xline->argAr[1]->val, NULL, 10);
//     if(strncmp(xline->argAr[1]->val, "Read", 4)){
        

//     }else if (strncmp(xline->argAr[1]->val, "Write", 5)){

        
//     }else if (strncmp(xline->argAr[1]->val, "Create", 6)){


//     }else if (strncmp(xline->argAr[1]->val, "Delete", 6)){

//     }else if (strncmp(xline->argAr[1]->val, "Append", 6)){

//     }else{
//         printf("unimpl error\n");
//         return NULL;
//     }
//     // xline->

//     XMLLine* xline = processXMLLine(buf, buflen);


// }

// Ext *readExt(FILE *fptr, int *nestdep, int *readed)
// {
//     char bufs[1024];
//     int doneExt = 0;
//     Extstr *unionext;
//     Ext *newExt = (Ext *)malloc(sizeof(Ext));
//     int buflen = readalem(bufs, fptr);
//     *readed += buflen;
//     if (bufs[0] != '<')
//     {
//         printf("badly formed xml\n");
//         return NULL;
//     }
//     if (strncmp(bufs, "<Node", 5))
//     {
//         readNode(bufs, buflen, newExt, fptr, readed);
//     }
//     else if (strncmp(bufs, "<Block", 6))
//     {
//         readBlock(bufs, buflen, nestdep, newExt);
//     }
//     else if (strncmp(bufs, "</Block", 7))
//     {
//         *nestdep -= 1;
//         return NULL;
//     }
// }

int readintoXMLstruct(FILE* fptr, XMLLine** xlines){
    struct stat fileStat;
    fstat(fileno(fptr), &fileStat);
    int readed = 0;
    long long fsize = fileStat.st_size;
    char buf[4096];
    int xddd= 0;
    int xnow = 0;
    int totes = 0;
    while (!feof(fptr) )
    {
        printf("we need to read %d, we have read %d\n", fileStat.st_size, readed);
        xddd =  readalem(buf, fptr);
        readed += xddd;

        xlines[xnow] =processXMLLine(buf, xddd);
                xnow++;
        totes++;
    }
    return totes;
}

StateTree *readFile(FILE *fptr)
{
//     Ext *extL[24];
//     int nestdep = 0;
//     int extct = 0;
//     int readed = 0;
//     struct stat fileStat;
//     fstat(fileno(fptr), &fileStat);

//     long long fsize = fileStat.st_size;
//     while (readed < fileStat.st_size)
//     {
//         int predep = nestdep;
//         extL[extct] = readExt(fptr, &nestdep, &readed);
//         if (predep > nestdep)
//         {
//             extct -= 1;
//         }
//     }
// }
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

    printf("test Xline\n\n\n\n\n");
    XMLLine *xline = processXMLLine("<Name />", 8);
    char *swag = "<someother swag=\"asdf\" arg2=\"money\" />";
    XMLLine *x1line = processXMLLine(swag, strlen(swag));

    XMLLine *x2line = processXMLLine("</endingtag>", 8);

    printXline(xline);
    printf("test Xline\n\n\n\n\n");
    printXline(x1line);
    printf("test Xline\n\n\n\n\n");
    printXline(x2line);



    printf("test blocks\n\n\n\n\n\n");
    

    char *swag2 = "<Block num=\"89\" >";
    int nedstep = 0;



    printf("test files\n\n\n\n\n\n");
    Ext *newExt = (Ext *)malloc(sizeof(Ext));
    FILE* testf = fopen("testf.xml", "r");
    XMLLine** xlines = (XMLLine **)malloc(sizeof(XMLLine*));
    int readlines = readintoXMLstruct(testf, xlines);


    printf("test results\n\n\n\n\n\n");
    printf("read %d lines\n", readlines);
    for(int i = 0; i < readlines; i++){
        printf("prehappy\n");
        printXline(xlines[i]);
        printf("happy\n");
    }
    


}