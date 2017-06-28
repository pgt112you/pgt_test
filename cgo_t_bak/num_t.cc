#include "num_t.h"

extern "C" {

int add(int a, int b) {
    //return a+b;
    T t = T();
    t.add(a, b);
    return t.show();
}

int substra(int big, int small) {
    return big-small;
}

}
