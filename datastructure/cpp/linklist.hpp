#pragma once

#include <assert.h>
#include <iostream>

// 双向链表 (by uyk, 2022.04.16)

template <typename T>
struct Node {
    T data;
    Node<T>* pNext;
    Node<T>* pPrev;

    Node(T data, Node<T>* prev, Node<T>* next) {
        if (prev) prev->pNext = this;
        if (next) next->pPrev = this;
        this->data = data;
        this->pPrev = prev;
        this->pNext = next;
    }
    ~Node() {
        if (pPrev) pPrev->pNext = pNext;
        if (pNext) pNext->pPrev = pPrev;
    }
};

template <typename T>
class LinkList {
private:
    uint32_t len;
    Node<T>* pFront;
    Node<T>* pBack;

public:
    LinkList() { pFront = pBack = NULL, len = 0; }
    ~LinkList() { clear(); }

    Node<T>* begin() { return pFront; }
    Node<T>* end() { return pBack; }
    // for (auto p = L.begin(); p != NULL; p = p->pNext) {}
    // for (auto p = L.end(); p != NULL; p = p->pPrev) {}

    T front() { return pFront->data; }
    T back() { return pBack->data; }

    // get length
    uint32_t length() { return len; }

    // add one
    LinkList<T>* push_back(T data);
    LinkList<T>* push_front(T data);

    // delete one
    LinkList<T>* pop_back();
    LinkList<T>* pop_front();

    // get node from index
    Node<T>* node_from_index(int index /*<0:back_to_front;>=0:front_to_back*/);

    // insert one
    LinkList<T>* insert_before(uint32_t index, T data);
    LinkList<T>* insert_after(uint32_t index, T data);

    // delete one
    T remove(uint32_t index);
    // delete all
    LinkList<T>* clear();

    // reverse linklist
    LinkList<T>* reverse();

    // display node's data
    void print_front_to_back();
    void print_back_to_front();
};

template <typename T>
LinkList<T>& operator+(const T& data, LinkList<T>& linklist) {
    linklist.push_front(data);
    return linklist;
}
template <typename T>
LinkList<T>& operator+(LinkList<T>& linklist, const T& data) {
    linklist.push_back(data);
    return linklist;
}

template <typename T>
LinkList<T>* LinkList<T>::push_back(T data) {
    pBack = new Node<T>(data, pBack, NULL);
    if (pFront == NULL) pFront = pBack;
    ++len;
    return this;
}

template <typename T>
LinkList<T>* LinkList<T>::push_front(T data) {
    pFront = new Node<T>(data, NULL, pFront);
    if (pBack == NULL) pBack = pFront;
    ++len;
    return this;
}

// delete one
template <typename T>
LinkList<T>* LinkList<T>::pop_back() {
    assert(len);

    if (len == 1) {
        delete pBack;
        pFront = pBack = NULL;
    } else {
        Node<T>* p = pBack;
        pBack = pBack->pPrev;
        delete p;
    }

    --len;
    return this;
}

template <typename T>
LinkList<T>* LinkList<T>::pop_front() {
    assert(len);

    if (len == 1) {
        delete pFront;
        pFront = pBack = NULL;
    } else {
        Node<T>* p = pFront;
        pFront = pFront->pNext;
        delete p;
    }

    --len;
    return this;
}

// get node from index
template <typename T>
Node<T>* LinkList<T>::node_from_index(int index /*<0:back_to_front;>=0:front_to_back*/) {
    if (index < 0) {
        // back_to_front
        index = abs(index) - 1;
        assert(index < len);
        Node<T>* p = pBack;
        while (index--) { p = p->pPrev; }
        return p;
    } else {
        // front_to_back
        assert(index < len);
        Node<T>* p = pFront;
        while (index--) { p = p->pNext; }
        return p;
    }
}

// insert one
template <typename T>
LinkList<T>* LinkList<T>::insert_before(uint32_t index, T data) {
    assert(len);  // can't insert if len is zero.
    Node<T>* p = node_from_index(index);
    Node<T>* pInsert = new Node<T>(data, p->pPrev, p);
    if (p == pFront) pFront = pInsert;
    ++len;
    return this;
}
template <typename T>
LinkList<T>* LinkList<T>::insert_after(uint32_t index, T data) {
    assert(len);  // can't insert if len is zero.
    Node<T>* p = node_from_index(index);
    Node<T>* pInsert = new Node<T>(data, p, p->pNext);
    if (p == pBack) pBack = pInsert;
    ++len;
    return this;
}

// delete one
template <typename T>
T LinkList<T>::remove(uint32_t index) {
    Node<T>* p = node_from_index(index);
    T data = p->data;
    if (p == pFront) pFront = p->pNext;
    if (p == pBack) pBack = p->pPrev;
    delete p;
    --len;
    return data;
}
// delete all
template <typename T>
LinkList<T>* LinkList<T>::clear() {
    Node<T>* p;
    while (p = pFront) {
        pFront = pFront->pNext;
        delete p;
    }
    len = 0;
    pFront = pBack = NULL;
    return this;
}
template <typename T>

// reverse linklist
LinkList<T>* LinkList<T>::reverse() {
    Node<T>*p = pFront, *tmp;
    while (p) {
        tmp = p->pPrev;
        p->pPrev = p->pNext;
        p->pNext = tmp;
        p = p->pPrev;
    }
    p = pFront;
    pFront = pBack;
    pBack = p;
    return this;
}

template <typename T>
void LinkList<T>::print_front_to_back() {
    for (auto p = pFront; p != NULL; p = p->pNext) std::cout << p->data << " ";
    std::cout << std::endl;
}
template <typename T>
void LinkList<T>::print_back_to_front() {
    for (auto p = pBack; p != NULL; p = p->pPrev) std::cout << p->data << " ";
    std::cout << std::endl;
}
