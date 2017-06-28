
/*
 * 下面的两个ifdef必须加
 * 不然的话，会报错
 * could not determine kind of name for C.add
 * 和
 * could not determine kind of name for C.substra
*/

#ifdef __cplusplus
extern "C" {
#endif


int add(int, int);
int substra(int, int);

#ifdef __cplusplus
}
#endif
