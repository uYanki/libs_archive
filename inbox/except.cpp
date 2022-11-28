#include <iostream>

using namespace std;

class out_range_error {
public:
    enum error {
        LESS_THAN,
        GREATER_THAN,
    };
    void what() const {
        switch (_e) {
            case LESS_THAN: cout << "less"; break;
            case GREATER_THAN: cout << "greater"; break;
        }
    }

private:
    error _e;

public:
    out_range_error(error e) : _e(e) {}
    ~out_range_error() {}
};

int handle(int i) {
    if (-1 <= i) {
        if (i <= 1) {
            return i;
        } else {
            // 抛出类型为对象的异常（数值、字符串也可以作为异常标识）
            throw out_range_error(out_range_error::LESS_THAN);
        }
    } else {
        throw out_range_error(out_range_error::GREATER_THAN);
    }
}

void foo() {
    try {
        handle(-2);
        cout << "ok";
    } catch (out_range_error& e) {
        e.what();
    }
}

int main() {
    foo();
    return 0;
}