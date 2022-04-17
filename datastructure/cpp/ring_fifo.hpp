#pragma once

#include <assert.h>
#include <iostream>
using namespace std;

/* 环形队列
空满判断方法:
①使用标志位: head赶上tail时,队列空,置flag为0;tail赶上head时,队列满,置flag为1
②限制tail赶上head,即队尾和对首保留1个元素的空间: head=tail, 队列空; (tail+1)%size=head,队列满
以下实现的是用第二种方法来判断空满.
*/

template <typename T>
class ring_fifo {
private:
    T*       m_data;
    uint32_t m_maxsize;      /*最大储存数*/
    uint32_t m_head, m_tail; /*头尾索引*/
public:
    ring_fifo(uint32_t maxsize);
    ~ring_fifo() { delete m_data; }

    ring_fifo<T>* push(T data);
    ring_fifo<T>* pop();

    T front() { return m_data[m_head]; }
    T back() { return *item_from_index(-1); }

    T* begin() { return m_data + m_head; }
    T* end() { return item_from_index(-1); }

    T* item_from_index(int index);

    bool is_empty() { return m_head == m_tail; }
    bool is_full() { return m_head == (m_tail + 1) % m_maxsize; }

    uint32_t size() { return (m_maxsize + m_tail - m_head) % m_maxsize; }
    uint32_t maxsize() { return m_maxsize - 1; }

    ring_fifo<T>* print();
};

template <typename T>
ring_fifo<T>::ring_fifo(uint32_t maxsize) {
    assert(maxsize);
    m_head = m_tail = 0;
    // +1 预留个空元素,限制队尾追上队首
    m_data = new T[m_maxsize = ++maxsize];
    assert(m_data);
}

template <typename T>
ring_fifo<T>* ring_fifo<T>::push(T data) {
    assert(!is_full());
    m_data[m_tail] = data;
    if (++m_tail == m_maxsize) m_tail = 0;
    return this;
}
template <typename T>
ring_fifo<T>* ring_fifo<T>::pop() {
    assert(!is_empty());
    if (++m_head == m_maxsize) m_head = 0;
    return this;
}

template <typename T>
T* ring_fifo<T>::item_from_index(int index) {
    if (index >= 0) {
        assert(index < size());
        return m_data + (m_head + index) % m_maxsize;
    } else {
        assert(index >= -size());
        return m_data + (m_tail + index + m_maxsize) % m_maxsize;
    }
}

template <typename T>
ring_fifo<T>* ring_fifo<T>::print() {
    uint32_t len = size();
    std::cout << "[" << len << "]";
    for (int i = m_head; i < m_head + len; ++i)  //
        printf("%d  ", ((i < m_maxsize) ? (m_data[i]) : (m_data[i - m_maxsize])));
    std::cout << std::endl;
    return this;
}
