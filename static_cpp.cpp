#include "pgt_test.h"

/*
 * g++ static_cpp.cpp -o static_cpp
 * ldd static_cpp
    linux-vdso.so.1 =>  (0x00007ffc08ff8000)
    libstdc++.so.6 => /usr/local/lib64/libstdc++.so.6 (0x00007f78b8cf0000)
    libm.so.6 => /lib64/libm.so.6 (0x00007f78b89ee000)
    libgcc_s.so.1 => /usr/local/lib64/libgcc_s.so.1 (0x00007f78b87d6000)
    libc.so.6 => /lib64/libc.so.6 (0x00007f78b8409000)
    /lib64/ld-linux-x86-64.so.2 (0x00007f78b9074000)
 * 
 * g++ -static-libstdc++ static_cpp.cpp -o static_cpp
 * ldd static_cpp
    linux-vdso.so.1 =>  (0x00007fffed1ed000)
    libm.so.6 => /lib64/libm.so.6 (0x00007f1de6123000)
    libgcc_s.so.1 => /usr/local/lib64/libgcc_s.so.1 (0x00007f1de5f0b000)
    libc.so.6 => /lib64/libc.so.6 (0x00007f1de5b3e000)
    /lib64/ld-linux-x86-64.so.2 (0x00007f1de6425000)
*/

int main() {
  printf("hello world\n");
  return 0;
}
