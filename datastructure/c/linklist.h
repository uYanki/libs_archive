
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ElemType int

#define LINKLIST_PRINT

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

static inline unsigned char linklist_is_empty(pLinkList linklist);

static inline ElemType linklist_back(pLinkList linklist);
static inline ElemType linklist_front(pLinkList linklist);

pLinkNode linklist_insert_after(pLinkList linklist, pLinkNode linknode, ElemType data);
pLinkNode linklist_insert_before(pLinkList linklist, pLinkNode linknode, ElemType data);

pLinkNode linklist_push_back(pLinkList linklist, ElemType data);
pLinkNode linklist_push_front(pLinkList linklist, ElemType data);

ElemType linklist_pop_back(pLinkList linklist);
ElemType linklist_pop_front(pLinkList linklist);

int linklist_length(pLinkList linklist);

void linklist_remove(pLinkList linklist, pLinkNode node);

#ifdef LINKLIST_PRINT
void linklist_print(pLinkList linklist);
#endif