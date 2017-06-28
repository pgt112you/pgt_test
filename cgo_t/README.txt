g++ -c num_t.cc num_t_1.cc
ar rvs num.a num_t.o num_t_1.o
go build --ldflags '-extldflags "-static-libstdc++"' ./

num_t.h里面是不含c++语法的，num_t.cc里面才含有c++的语法。
golang编译的时候，只是使用num_t.h的头文件，里面有extern "C"，所以是能编译过的，然后头文件里的函数的定义都是在num.a里面的。这样就能编译过这个目录了
