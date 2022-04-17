#include "ring_fifo.hpp"

int main() {
    ring_fifo<uint32_t> ring(11);
    for (uint32_t i = 0; i < 5; ++i) { ring.push(i)->push(i * 2); }
    ring.print();
    ring.pop();
    ring.push(666);
    ring.print();
    *(ring.begin()) = 999;
    ring.print();
    ring.pop();
    ring.push(666);
    ring.push(666);
    ring.print();
    return 0;
}
