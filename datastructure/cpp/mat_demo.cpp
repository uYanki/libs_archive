
#include "mat.hpp"
using namespace std;

int _main() {
    mat<int> m(10, 10);
    for (uint32_t i = 0; i < 10; ++i) m[i][i] = i * i;
    m.set(1, 2, -1);
    cout << m.get(1, 2) << endl;
    m.print();
    return 0;
}
