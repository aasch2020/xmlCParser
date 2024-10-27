
#include "xml2Tree.h"

void pretty_print_block(const Block *block)
{
    printf("Block Struct:\n");
    printf("  currind : %d\n", block->currind);
}
void printExt(Ext *ext)
{
    printf("Ext Struct:\n");
    printf("  blk_nod    : %d\n", ext->blk_nod);
    printf("  is_cond    : %d\n", ext->is_cond);
    printf("  nest_dep   : %d\n", ext->nest_dep);
    printf("  next_ind_L : %d\n", ext->next_ind_L);
    printf("  next_ind_R : %d\n", ext->next_ind_R);
    printf("  blockEnd   : %d\n", ext->blockEnd);
    if (ext->blk_nod == 1)
    {
        pretty_print_block(&(ext->item.blk));
    }
    else
    {
        pretty_print_node(&(ext->item.node));
    }
}

int parseNodeHead(XMLLine *xline, Node *nod)
{
    printf("parsing as node head\n");
    printXline(xline);
    int reval = -1;
    for (int i = 0; i < xline->argsCount; i++)
    {
        if (strncmp(xline->argAr[i]->name, "type", 4) == 0)
        {
            if (strncmp(xline->argAr[i]->val, "RWACD", 5) == 0)
            {
                printf("RWACD OK\n");
                nod->uniontype = 0;
                reval = 0;
            }
            else
            {
                printf("bad call\n");
            }
        }
        else if (strncmp(xline->argAr[i]->name, "num", 3) == 0)
        {
            printf("NUM OK\n");
            nod->currind = strtol(xline->argAr[i]->val, NULL, 10);
        }
        else
        {
            printf("wut are u sending me\n");
            reval = -1;
        }
    }
    return reval;
}

int parseArgsNum(XMLLine *xline)
{

    if (!strncmp(xline->name, "ListArgs", 4) == 0)
    {
        return -1;
    }
    for (int i = 0; i < xline->argsCount; i++)
    {
        if (strncmp(xline->argAr[i]->name, "count", 5) == 0)
        {
            int reval = strtol(xline->argAr[i]->val, NULL, 10);
            return reval;
        }
        else
        {
            printf("wut are u sending me\n");
            return -1;
        }
    }
    return -1;
}

Ext *parseNode(XMLLine **xlines, int numlines, int *cntr, int nestrdepth)
{
    Ext *retext = (Ext *)calloc(1, sizeof(Ext));
    retext->blk_nod = 0;
    retext->is_cond = 0;
    retext->blockEnd = 0;
    retext->nest_dep = nestrdepth;
    retext->next_ind_L = -1;
    retext->next_ind_R = -1;

    Node *nod = &(retext->item.node);
    int sycalltype = parseNodeHead(xlines[*cntr], nod);
    printf("incr counter for head %d\n", *cntr);
    *(cntr) += 1;
    int argsnum = parseArgsNum(xlines[*cntr]);

    printf("incr counter for argsnum %d\n", *cntr);
    *(cntr) += 1;
    switch (sycalltype)
    {
    case 0: // The fac case. This is the only 1 for now;

        nod->uniontype = 0;
        if (argsnum != 2)
        {
            printf("possibly unsupported argstring\n");
        }
        while (strncmp(xlines[*cntr]->name, "Arg", 3) == 0)
        {
            printf("parsing a RWACD\n");
            if (!(xlines[*cntr]->ending == 1))
            {
                printf("malformed XML\n");
            }
            else
            {
                if (xlines[*cntr]->argsCount != 1)
                {
                    printf("malform args XML\n");
                }
                else
                {
                    if (strncmp(xlines[*cntr]->argAr[0]->name, "filename", 8) == 0)
                    {
                        strncpy(nod->node.fac.filename, xlines[*cntr]->argAr[0]->val, 4096);
                    }
                    else if (strncmp(xlines[*cntr]->argAr[0]->name, "access", 6) == 0)
                    {
                        nod->node.fac.accessType = xlines[*cntr]->argAr[0]->val[0];
                    }
                }
            }
            printf("incr counter for args %d\n", *cntr);
            *(cntr) += 1;
        }
        if (!(strncmp(xlines[*cntr]->name, "ListArg", 7) == 0 && xlines[*cntr]->ending == 1))
        {
            printf("bad arg ender");
        }
        else
        {
            printf("good arg ender\n");
            *(cntr) += 1;
        }
        break;
    default:
        printf("bad optype\n");
        break;
    }
    if (!(strncmp(xlines[*cntr]->name, "Node", 4) == 0 && xlines[*cntr]->ending == 1))
    {
        printf("bad node ender\n");
    }
    else
    {
        printf("good node ender\n");
        *(cntr) += 1;
    }
    pretty_print_node(nod);
    printf("counter after node %d\n", *cntr);
    return retext;
}

int extListFromXML(XMLLine **xlines, int numlines, Ext **nodel)
{
    int topassr = numlines;
    int cntr = 0;
    int nestrdepth = 0;

    // Cases: name block, pop in a level, wait until we can pop out, flag the previous element if we find a closing block tag.
    // name node: pop in and parse lines until EON, parse argskis
    // name cond: not fully impl in state tree yet? double check this later, but tldr is when cond re-block into depht and pop out of depth to cond block correctly sepr from nodes
    XMLLine *xline;
    int nodectr = 0;
    while (cntr < topassr)
    {

        xline = xlines[cntr];
        printf("parsing line at %d\n", cntr);
        printXline(xline);
        printf("truth values %d, %d\n", strncmp(xline->name, "Block", 5) == 0, strncmp(xline->name, "Node", 4) == 0);
        if (strncmp(xline->name, "Block", 5) == 0)
        {
            printf("parsing a block\n");
            if (xline->ending == 0)
            {
                nestrdepth++;
                Ext *extr = (Ext *)calloc(1, sizeof(Ext));
                extr->blk_nod = 1;
                extr->is_cond = 0;
                extr->blockEnd = 0;
                extr->nest_dep = nestrdepth;
                extr->next_ind_L = -1;
                extr->next_ind_R = -1;
                nodel[nodectr] = extr;
                nodectr++;
                cntr += 1;
            }
            else
            {
                nodel[nodectr - 1]->blockEnd = 1;
                nestrdepth--;
                cntr += 1;
            }
        }
        else if (strncmp(xline->name, "Node", 4) == 0)
        {
            printf("parsing nodeskis\n");
            Ext *extr = parseNode(xlines, numlines, &cntr, nestrdepth);
            nodel[nodectr] = extr;
            nodectr++;
        }
        else if (strncmp(xline->name, "Cond", 4) == 0)
        {
            printf("unsup conds for now\n");
        }
        else
        {
            printf("dude...\n");
            return -1;
        }
    }
    return nodectr;
}

StateTree *makeTreefromXML(XMLLine **xlines, int numlines)
{
    return NULL;
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
    freeXMLLine(xline);
    freeXMLLine(x1line);
    XMLLine *x2line = processXMLLine("</endingtag>", 8);

    freeXMLLine(x2line);
    printXline(xline);
    printf("test Xline\n\n\n\n\n");
    printXline(x1line);
    printf("test Xline\n\n\n\n\n");
    printXline(x2line);

    printf("test files\n\n\n\n\n\n");
    FILE *testf = fopen("testf.xml", "r");
    XMLLine **xlines = (XMLLine **)calloc(100, sizeof(XMLLine *));
    int readlines = readintoXMLstruct(testf, xlines);

    printf("test results\n\n\n\n\n\n");
    printf("read %d lines\n", readlines);
    for (int i = 0; i < readlines; i++)
    {
        printf("\nline no %d\n", i);
        printXline(xlines[i]);
    }

    printf("\n\n\nbigreadtest\n\n\n\n\n\n");
    Ext **nodel = (Ext **)calloc(100, sizeof(Ext *));
    int madeexts = extListFromXML(xlines, readlines, nodel);
    for (int i = 0; i < madeexts; i++)
    {
        printExt(nodel[i]);
    }

    fclose(testf);

    for (int i = 0; i < readlines; i++)
    {
        freeXMLLine(xlines[i]);
    }
    free(xlines);

    for (int i = 0; i < madeexts; i++)
    {
        free(nodel[i]);
    }
    free(nodel);
}