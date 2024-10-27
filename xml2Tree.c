
#include "xml2Tree.h"

Node *rcrMak(Ext **extl, int madexts, int cntr, Node *endtinel);

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
        pretty_print_block(ext->item.blk);
    }
    else
    {
        pretty_print_node(ext->item.node);
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

int parseCondHead(XMLLine *xline, Node *nod)
{
    printXline(xline);
    int reval = -1;
    for (int i = 0; i < xline->argsCount; i++)
    {
        if (strncmp(xline->argAr[i]->name, "num", 3) == 0)
        {
            printf("NUM OK\n");
            nod->currind = strtol(xline->argAr[i]->val, NULL, 10);
            reval = 0;
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
    Node *ode = (Node *)calloc(1, sizeof(Node));
    retext->item.node = ode;
    retext->blk_nod = 0;
    retext->is_cond = 0;
    retext->blockEnd = 0;
    retext->nest_dep = nestrdepth;
    retext->next_ind_L = -1;
    retext->next_ind_R = -1;

    Node *nod = ode;
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

Ext *parseCond(XMLLine **xlines, int numlines, int *cntr, int nestrdepth)
{
    Ext *retext = (Ext *)calloc(1, sizeof(Ext));
    Node *rlnode = (Node *)calloc(1, sizeof(Node));
    retext->item.node = rlnode;
    retext->blk_nod = 0;
    retext->is_cond = 1;
    retext->blockEnd = 0;
    retext->nest_dep = nestrdepth;

    Node *nod = rlnode;
    int sycalltype = parseCondHead(xlines[*cntr], nod);
    printf("incr counter for head %d\n", *cntr);
    *(cntr) += 1;
    if (sycalltype == 0)
    {

        nod->uniontype = 1;
        int linkctr = 0;
        printXline(xlines[*cntr]);
        while (strncmp(xlines[*cntr]->name, "Link", 4) == 0)
        {

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
                    if (strncmp(xlines[*cntr]->argAr[0]->name, "to", 2) == 0)
                    {
                        if (linkctr == 0)
                        {
                            printf(" jto %s\n", xlines[*cntr]->argAr[0]->val);
                            linkctr++;
                            retext->next_ind_L = strtol(xlines[*cntr]->argAr[0]->val, NULL, 10);
                        }
                        else
                        {
                            printf(" jto2 %s\n", xlines[*cntr]->argAr[0]->val);
                            retext->next_ind_R = strtol(xlines[*cntr]->argAr[0]->val, NULL, 10);
                        }
                    }
                    else
                    {
                        printf("bad cond\n");
                    }
                }
            }
            *(cntr) += 1;
        }
    }
    else
    {
        printf("bad optype\n");
    }
    if (!(strncmp(xlines[*cntr]->name, "Cond", 4) == 0 && xlines[*cntr]->ending == 1))
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

int extListFromXML(XMLLine **xlines, int numlines, Ext **extl)
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
        if (strncmp(xline->name, "Block", 5) == 0)
        {
            printf("parsing a block\n");
            if (xline->ending == 0)
            {
                nestrdepth++;
                Ext *extr = (Ext *)calloc(1, sizeof(Ext));
                Block *blk = (Block *)calloc(1, sizeof(Block));
                extr->item.blk = blk;
                extr->blk_nod = 1;
                extr->is_cond = 0;
                extr->blockEnd = 0;
                extr->nest_dep = nestrdepth;
                extr->next_ind_L = -1;
                extr->next_ind_R = -1;
                extl[nodectr] = extr;
                extr->item.blk->currind = strtol(xline->argAr[0]->val, NULL, 10);
                nodectr++;
                cntr += 1;
            }
            else
            {
                extl[nodectr - 1]->blockEnd = 1;
                nestrdepth--;
                cntr += 1;
            }
        }
        else if (strncmp(xline->name, "Node", 4) == 0)
        {
            printf("parsing nodeskis\n");
            Ext *extr = parseNode(xlines, numlines, &cntr, nestrdepth);
            extl[nodectr] = extr;
            nodectr++;
        }
        else if (strncmp(xline->name, "Cond", 4) == 0)
        {
            printf("unsup conds for now\n");
            Ext *extr = parseCond(xlines, numlines, &cntr, nestrdepth);
            extl[nodectr] = extr;
            nodectr++;
        }
        else
        {
            printf("dude...\n");
            return -1;
        }
    }
    return nodectr;
}
Node *condSeek(Ext **extl, int numlines, int start, Node *endtinel)
{
    int deplevel = extl[start]->nest_dep;
    Node *seekr = endtinel;
    for (int i = start + 1; i < numlines; i++)
    {
        printf("entering from condmake on index %d\n", i);
        if (extl[i]->nest_dep == deplevel)
        {

            seekr = rcrMak(extl, numlines, i, endtinel);
        }
    }
    return seekr;
}
Node *rcrMak(Ext **extl, int madexts, int cntr, Node *endtinel)
{

    if (cntr >= madexts)
    {
        return endtinel;
    }
    Ext *on = extl[cntr];
    if (on->blk_nod == 1)
    {
        printf("linking on a block\n");
        Node *returnge = rcrMak(extl, madexts, cntr + 1, endtinel);
        pretty_print_node(returnge);
        printf("  I am: %p\n", (void *)returnge);
        return returnge;
    }
    else if (on->is_cond == 1)
    {
        printf("linking on a cond %d\n", cntr);
        printExt(extl[cntr]);
        Node *returnge = extl[cntr]->item.node;
        returnge->node.fac.next = condSeek(extl, madexts, cntr, endtinel);
        pretty_print_node(returnge);
        returnge->node.cond.trueChild = rcrMak(extl, madexts, extl[cntr]->next_ind_L, endtinel);
        returnge->node.cond.falseChild = rcrMak(extl, madexts, extl[cntr]->next_ind_R, endtinel);

        pretty_print_node(returnge);
        printf("  I am: %p\n", (void *)returnge);
        return returnge;
    }
    else
    {
        if (on->blockEnd == 1)
        {
            printf("linking on a blocknder cond\n");
            Node *returnge = extl[cntr]->item.node;
            returnge->node.fac.next = endtinel;
            pretty_print_node(returnge);
            printf("  I am: %p\n", (void *)returnge);
            return returnge;
        }
        else
        {
            printf("linking on a normal cond\n");
            Node *returnge = extl[cntr]->item.node;
            returnge->node.fac.next = rcrMak(extl, madexts, cntr + 1, endtinel);
            pretty_print_node(returnge);
            printf("  I am: %p\n", (void *)returnge);
            return returnge;
        }
    }
}

void freeXT(Ext *ext)
{
    if (ext->blk_nod == 1)
    {
        free(ext->item.blk);
    }
    free(ext);
}
StateTree *makeTreefromXML(XMLLine **xlines, int numlines)
{
    Ext **extl = (Ext **)calloc(100, sizeof(Ext *));

    int madeexts = extListFromXML(xlines, numlines, extl);
    StateTree *stree = init_state_tree();

    Node *start = rcrMak(extl, madeexts, 0, stree->end);
    for (int i = 0; i < madeexts; i++)
    {
        freeXT(extl[i]);
    }
    free(extl);
    stree->root->node.sent.next = start;
    return stree;
}

int main()
{
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

    StateTree *teed = makeTreefromXML(xlines, readlines);
    printf("\n\n\nbigreadtest\n\n\n\n\n\n");
    fulliter(teed);

    fflush(stdout);
    freetree(teed);
    fclose(testf);

    for (int i = 0; i < readlines; i++)
    {
        freeXMLLine(xlines[i]);
    }
    free(xlines);
}