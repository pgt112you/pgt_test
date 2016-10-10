#include "tool.h"

extern fileinfo *allfile;

//#define FILE_DIR "/data0/watchagent_data"

void _ldr_binarytree(fileinfo *root) {
  if (root == NULL) {
    return;
  }
  if (root->left != NULL) {
    _ldr_binarytree(root->left);
  }
  if (root->right != NULL) {
    _ldr_binarytree(root->right);
  }
  printf("%s\n", root->filepath);
  return;
}


void test_getdirfile() {
  getdirfile(FILE_DIR);
  _ldr_binarytree(allfile);
}

fileinfo *test_searchbst() {
  const char *path = "/data0/watchagent_data/aaa.txt";
  fileinfo *p = searchbst(allfile, path);
  printf("%s\n", p->filepath);
  return p;
}

void test_binarytree_del(fileinfo *p) {
  int res = deletebst(allfile, "/data0/watchagent_data/aaa.txt");
  printf("res is %d\n", res);
  _ldr_binarytree(allfile);
}

int main() {
  test_getdirfile();
  printf("=====\n");
  fileinfo *p = test_searchbst();
  printf("=====\n");
  test_binarytree_del(p);

  return 0;
}
