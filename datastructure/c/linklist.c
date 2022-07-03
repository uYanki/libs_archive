#include "linklist.h"

pLinkList linklist_create(void) {
    pLinkList linklist = malloc(sizeof(LinkList));
    linklist->pBack = linklist->pFront = (void*)0;
    return linklist;
}

void linklist_destroy(pLinkList linklist) {
    assert(linklist);

    pLinkNode node = linklist->pFront, t;
    while (node) {
        t = node->pNext;
        free(node);
        node = t;
    }
    free(linklist);
}

static inline unsigned char linklist_is_empty(pLinkList linklist) {
    return linklist->pFront == (void*)0 || linklist->pBack == (void*)0;
}

static inline ElemType linklist_front(pLinkList linklist) { return linklist->pFront->data; }

static inline ElemType linklist_back(pLinkList linklist) { return linklist->pBack->data; }

pLinkNode linklist_insert_after(pLinkList linklist, pLinkNode linknode, ElemType data) {
    assert(linklist);
    assert(linknode);

    pLinkNode node = malloc(sizeof(LinkNode));

    if (linklist->pBack == linknode)
        linklist->pBack = node;

    node->data  = data;
    node->pPrev = linknode;
    node->pNext = linknode->pNext;

    if (linknode->pNext)
        linknode->pNext->pPrev = node;

    return linknode->pNext = node;
}

pLinkNode linklist_insert_before(pLinkList linklist, pLinkNode linknode, ElemType data) {
    assert(linklist);
    assert(linknode);

    pLinkNode node = malloc(sizeof(LinkNode));

    if (linklist->pFront == linknode)
        linklist->pFront = node;

    node->data  = data;
    node->pNext = linknode;
    node->pPrev = linknode->pPrev;

    if (linknode->pPrev)
        linknode->pPrev->pNext = node;

    return linknode->pPrev = node;
}

pLinkNode linklist_push_front(pLinkList linklist, ElemType data) {
    assert(linklist);
    pLinkNode node = malloc(sizeof(LinkNode));
    node->data     = data;
    node->pPrev    = (void*)0;
    if ((node->pNext = linklist->pFront) == (void*)0)
        return linklist->pFront = linklist->pBack = node;
    return linklist->pFront = linklist->pFront->pPrev = node;
}

pLinkNode linklist_push_back(pLinkList linklist, ElemType data) {
    assert(linklist);
    pLinkNode node = malloc(sizeof(LinkNode));
    node->data     = data;
    node->pNext    = (void*)0;
    if ((node->pPrev = linklist->pBack) == (void*)0)
        return linklist->pFront = linklist->pBack = node;
    return linklist->pBack = linklist->pBack->pNext = node;
}

ElemType linklist_pop_front(pLinkList linklist) {
    assert(linklist);
    assert(linklist->pFront);
    pLinkNode node     = linklist->pFront;
    ElemType  data     = node->data;
    node->pNext->pPrev = (void*)0;
    linklist->pFront   = node->pNext;
    free(node);
}

ElemType linklist_pop_back(pLinkList linklist) {
    assert(linklist);
    assert(linklist->pBack);
    pLinkNode node     = linklist->pBack;
    ElemType  data     = node->data;
    node->pPrev->pNext = (void*)0;
    linklist->pBack    = node->pPrev;
    free(node);
}

void linklist_remove(pLinkList linklist, pLinkNode linknode) {
    assert(linklist);
    assert(linknode);
    if (linknode->pPrev) linknode->pPrev->pNext = linknode->pNext;
    if (linknode->pNext) linknode->pNext->pPrev = linknode->pPrev;
    if (linklist->pFront == linknode) linklist->pFront = linknode->pNext;
    if (linklist->pBack == linknode) linklist->pBack = linknode->pPrev;
    free(linknode);
}

int linklist_length(pLinkList linklist) {
    assert(linklist);
    int length = 0;

    pLinkNode node = linklist->pFront;
    if (node) {
        do { ++length; } while (node = node->pNext);
    }

    return length;
}

#ifdef LINKLIST_PRINT
void linklist_print(pLinkList linklist) {
    assert(linklist);
    assert(linklist->pFront);
    pLinkNode node = linklist->pFront;
    do { printf("%d,", node->data); } while (node = node->pNext);
    printf("\n");
}
#endif