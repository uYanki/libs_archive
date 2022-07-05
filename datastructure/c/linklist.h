
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ElemType int

#define LINKLIST_DEBUG
#define LINKLIST_TEST

typedef struct LinkNode LinkNode, *pLinkNode;
typedef struct LinkList LinkList, *pLinkList;

struct LinkNode {
    ElemType  data;
    pLinkNode pNext, pPrev;
};

struct LinkList {
    pLinkNode pFront, pBack;
};

pLinkList linklist_create(void);
void      linklist_destroy(pLinkList linklist);

unsigned char linklist_is_empty(pLinkList linklist);

ElemType linklist_back(pLinkList linklist);
ElemType linklist_front(pLinkList linklist);

pLinkNode linklist_insert_after(pLinkList linklist, pLinkNode linknode, ElemType data);
pLinkNode linklist_insert_before(pLinkList linklist, pLinkNode linknode, ElemType data);

pLinkNode linklist_push_back(pLinkList linklist, ElemType data);
pLinkNode linklist_push_front(pLinkList linklist, ElemType data);

ElemType linklist_pop_back(pLinkList linklist);
ElemType linklist_pop_front(pLinkList linklist);

int linklist_length(pLinkList linklist);

ElemType linklist_remove(pLinkList linklist, pLinkNode node);

#ifdef LINKLIST_DEBUG
void linklist_print_elems(pLinkList linklist);
void linklist_print_sider(pLinkList linklist);
#endif