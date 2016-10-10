#include "pgt_test.h"


int main() {
    enum tt {
        hello,
        world,
        earth,
        dream
    };
    //int world = 6; // error: previous declaration of ‘main()::tt world’
    printf("tt is %d\n", hello);
    printf("tt.world is %d\n", world);
    printf("tt.earth is %d\n", earth);
    printf("tt.dream is %d\n", dream);
}
