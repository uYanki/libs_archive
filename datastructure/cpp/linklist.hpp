#pragma once

#include <assert.h>
#include <iostream>

// 双向链表 (by uyk, 2022.04.16)

template <typename T>
struct Node {
    T data;
    Node* pNext;
    Node* pPrev;

    Node(T data, Node* prev, Node* next) {
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

    // for(auto node=LinkList.begin();node!=LinkList.end();node++)
    Node<T>* operator--() { return pPrev; }     // 前置减减
    Node<T>* operator--(int) { return pPrev; }  // 后置减减
    Node<T>* operator++() { return pNext; }     // 前置加加
    Node<T>* operator++(int) { return pNext; }  // 后置加加
    // 区分前置和后置的本质: val++ -> val+0
};

template <typename T>
class LinkList {
private:
    uint32_t len;
    Node<T>* pFirst;
    Node<T>* pLast;

public:
    LinkList() { pFirst = pLast = NULL, len = 0; }
    ~LinkList() { clear(); }

    Node<T>* begin() { return pFirst; }
    Node<T>* end() { return pLast; }

    // get length
    uint32_t length() { return len; }

    // add one
    LinkList<T>* push_back(T data);
    LinkList<T>* push_front(T data);

    // delete one
    T pop_back();
    T pop_front();

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
    pLast = new Node<T>(data, pLast, NULL);
    if (pFirst == NULL) pFirst = pLast;
    ++len;
    return this;
}

template <typename T>
LinkList<T>* LinkList<T>::push_front(T data) {
    pFirst = new Node<T>(data, NULL, pFirst);
    if (pLast == NULL) pLast = pFirst;
    ++len;
    return this;
}

// delete one
template <typename T>
T LinkList<T>::pop_back() {
    assert(len);
    T data = pLast->data;

    if (len == 1) {
        delete pLast;
        pFirst = pLast = NULL;
    } else {
        Node<T>* p = pLast;
        pLast = pLast->pPrev;
        delete p;
    }

    --len;
    return data;
}

template <typename T>
T LinkList<T>::pop_front() {
    assert(len);
    T data = pFirst->data;

    if (len == 1) {
        delete pFirst;
        pFirst = pLast = NULL;
    } else {
        Node<T>* p = pFirst;
        pFirst = pFirst->pNext;
        delete p;
    }

    --len;
    return data;
}

// get node from index
template <typename T>
Node<T>* LinkList<T>::node_from_index(int index /*<0:back_to_front;>=0:front_to_back*/) {
    if (index < 0) {
        // back_to_front
        index = abs(index) - 1;
        assert(index < len);
        Node<T>* p = pLast;
        while (index--) { p = p->pPrev; }
        return p;
    } else {
        // front_to_back
        assert(index < len);
        Node<T>* p = pFirst;
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
    if (p == pFirst) pFirst = pInsert;
    ++len;
    return this;
}
template <typename T>
LinkList<T>* LinkList<T>::insert_after(uint32_t index, T data) {
    assert(len);  // can't insert if len is zero.
    Node<T>* p = node_from_index(index);
    Node<T>* pInsert = new Node<T>(data, p, p->pNext);
    if (p == pLast) pLast = pInsert;
    ++len;
    return this;
}

// delete one
template <typename T>
T LinkList<T>::remove(uint32_t index) {
    Node<T>* p = node_from_index(index);
    T data = p->data;
    if (p == pFirst) pFirst = p->pNext;
    if (p == pLast) pLast = p->pPrev;
    delete p;
    --len;
    return data;
}
// delete all
template <typename T>
LinkList<T>* LinkList<T>::clear() {
    Node<T>* p;
    while (p = pFirst) {
        pFirst = pFirst->pNext;
        delete p;
    }
    len = 0;
    pFirst = pLast = NULL;
    return this;
}
template <typename T>

// reverse linklist
LinkList<T>* LinkList<T>::reverse() {
    Node<T>*p = pFirst, *tmp;
    while (p) {
        tmp = p->pPrev;
        p->pPrev = p->pNext;
        p->pNext = tmp;
        p = p->pPrev;
    }
    p = pFirst;
    pFirst = pLast;
    pLast = p;
    return this;
}

template <typename T>
void LinkList<T>::print_front_to_back() {
    Node<T>* p = pFirst;
    while (p) {
        std::cout << p->data << " ";
        p = p->pNext;
    }
    std::cout << std::endl;
}
template <typename T>
void LinkList<T>::print_back_to_front() {
    Node<T>* p = pLast;
    while (p) {
        std::cout << p->data << " ";
        p = p->pPrev;
    }
    std::cout << std::endl;
}
