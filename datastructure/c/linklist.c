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

unsigned char linklist_is_empty(pLinkList linklist) {
#ifdef LINKLIST_DEBUG
    printf("%s\n", (linklist->pFront == (void*)0 || linklist->pBack == (void*)0) ? "empty" : "not empty");
#endif
    return linklist->pFront == (void*)0 || linklist->pBack == (void*)0;
}

ElemType linklist_front(pLinkList linklist) {
    assert(linklist);
    assert(linklist->pFront);
#ifdef LINKLIST_DEBUG
    printf("%d\n", linklist->pFront->data);
#endif
    return linklist->pFront->data;
}

ElemType linklist_back(pLinkList linklist) {
    assert(linklist);
    assert(linklist->pBack);
#ifdef LINKLIST_DEBUG
    printf("%d\n", linklist->pBack->data);
#endif
    return linklist->pBack->data;
}

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
    pLinkNode node = linklist->pFront->pNext;
    ElemType  data = linklist->pFront->data;
    free(linklist->pFront);
    (linklist->pFront = node) ? (node->pPrev = (void*)0) : (linklist->pBack = (void*)0);
    return data;
}

ElemType linklist_pop_back(pLinkList linklist) {
    assert(linklist);
    assert(linklist->pBack);
    pLinkNode node = linklist->pBack->pPrev;
    ElemType  data = linklist->pBack->data;
    free(linklist->pBack);
    (linklist->pBack = node) ? (node->pNext = (void*)0) : (linklist->pFront = (void*)0);
    return data;
}

ElemType linklist_remove(pLinkList linklist, pLinkNode linknode) {
    assert(linklist);
    assert(linknode);
    ElemType data = linknode->data;
    if (linknode->pPrev) linknode->pPrev->pNext = linknode->pNext;
    if (linknode->pNext) linknode->pNext->pPrev = linknode->pPrev;
    if (linklist->pFront == linknode) linklist->pFront = linknode->pNext;
    if (linklist->pBack == linknode) linklist->pBack = linknode->pPrev;
    free(linknode);
    return data;
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

#ifdef LINKLIST_DEBUG

void linklist_print_elems(pLinkList linklist) {
    assert(linklist);
    assert(linklist->pFront);
    pLinkNode node = linklist->pFront;
    do { printf("%d,", node->data); } while (node = node->pNext);
    printf("\n");
}
void linklist_print_sider(pLinkList linklist) {
    printf("%p", linklist->pFront);
    if (linklist->pFront) {
        printf(":%d", linklist->pFront->data);
        printf(",%p", linklist->pFront->pPrev);
        printf(",%p", linklist->pFront->pNext);
    }
    printf("\n");
    printf("%p", linklist->pBack);
    if (linklist->pBack) {
        printf(":%d", linklist->pBack->data);
        printf(",%p", linklist->pFront->pPrev);
        printf(",%p", linklist->pFront->pNext);
    }
    printf("\n");
}

#endif

#ifdef LINKLIST_TEST

int main() {
    pLinkList list = linklist_create();

    linklist_push_back(list, 1);
    linklist_push_back(list, 2);
    linklist_print_sider(list);

    linklist_push_back(list, 3);
    linklist_push_front(list, 1);
    linklist_push_front(list, 2);
    linklist_push_front(list, 3);

    linklist_print_elems(list);

    linklist_pop_front(list);
    linklist_pop_front(list);
    linklist_pop_back(list);
    linklist_pop_back(list);
    linklist_pop_front(list);

    linklist_print_elems(list);
    linklist_print_sider(list);

    linklist_back(list);
    linklist_front(list);

    linklist_pop_back(list);

    linklist_print_sider(list);
    // linklist_print_elems(list);

    linklist_back(list);
    linklist_front(list);
    linklist_is_empty(list);

    linklist_destroy(list);
    return 0;
}

#endif
