#include "num_t.h"
#include "num_t_1.h"

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
