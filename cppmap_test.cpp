#include <map>
#include <iostream>
#include "pgt_test.h"

/*
 * 测试cpp的map的iterator
 * 在map中有数据变化的时候
 * 是否是稳定的，即不弱iterator指向的元素本身没有被删除
 * iterator能不能继续指向正确
*/

int main() {
  std::map<int, char> t_map;
  t_map[1] = 'a';
  t_map[2] = 'b';
  t_map[3] = 'c';
  t_map[4] = 'd';
  std::map<int, char>::iterator it;
  it = t_map.find(3);
  t_map.erase(4);
  t_map[6] = 'f';
  std::cout<< it->second << std::endl;
}


