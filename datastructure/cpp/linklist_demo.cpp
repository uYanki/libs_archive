#include "linklist.hpp"

using namespace std;

int _main() {
    LinkList<int> L;

    cout << "-------PUSH&POP------------" << endl;
    for (uint32_t i = 0; i < 5; i++) L.push_back(i);
    for (uint32_t i = 0; i < 5; i++) L.push_front(i);
    L.print_front_to_back();
    L.print_back_to_front();
    cout << "len:" << L.length() << endl;
    cout << "pop_front:" << L.front() << endl;
    cout << "pop_back:" << L.back() << endl;
    L.print_front_to_back();
    L.print_back_to_front();
    cout << "--------INSERT-----------" << endl;
    L.insert_after(0, 999);
    L.insert_before(0, 999);
    L.insert_after(1, 666);
    L.insert_before(1, 666);
    L.insert_before(-1, 999);
    L.insert_after(-1, 999);
    L.print_front_to_back();
    L.print_back_to_front();
    cout << "--------ADD-----------" << endl;
    9 + (7 + (8 + L)) + 7 + 8 + 9;  // 注:前加要添加括号
    L.print_front_to_back();
    L.print_back_to_front();
    cout << "--------REVERSE-----------" << endl;
    cout << "origin:" << endl;
    L.print_front_to_back();
    L.print_back_to_front();
    L.reverse();
    cout << "reverse:" << endl;
    L.print_front_to_back();
    L.print_back_to_front();
    cout << "--------REMOVE-----------" << endl;
    cout << "origin:" << endl;
    L.print_front_to_back();
    L.remove(1);
    cout << "remove:1" << endl;
    L.print_front_to_back();
    L.remove(0);
    cout << "remove:0" << endl;
    L.print_front_to_back();
    L.remove(0);
    cout << "remove:0" << endl;
    L.print_front_to_back();
    cout << "--------CLEAR-----------" << endl;
    L.clear();
    L.print_front_to_back();
    L.push_front(1)->push_back(2);
    L.print_front_to_back();

    return 0;
}
