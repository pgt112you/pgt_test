#include "tool.h"


fileinfo *allfile = NULL;
operatefile *of_list = NULL;  // operate file list此次要传输给对端操作fileinfo对象链表
operatefile *of_list_tail = NULL;

/*
 * 计算文件的md5值
*/
char *file_md5sum(const char *filename) {
  MD5_CTX c;
  MD5_Init(&c);
  FILE *f;
  f = fopen(filename, "r");
  if (f == NULL) return NULL;
  unsigned char md[MD5_DIGEST_LENGTH];
  void *pbuf = malloc(2048);
  size_t res;
  while (1) {
    res = fread(pbuf, 1, 2048, f); 
    if (res <= 0) {
      break;
    }   
    MD5_Update(&c, pbuf, res);
  }
  MD5_Final(md, &c);
  free(pbuf);
  fclose(f);
  char *result = (char *)malloc(sizeof(char)*32);
  memset(result, '\0', 32);
  int i;
  for(i = 0; i< MD5_DIGEST_LENGTH; i++) {
    sprintf(result+2*i, "%02x", md[i]);
  }
  return result;
}


/*
 * 生成新的allfile节点
*/
fileinfo *new_fileinfo(struct dirent *dp, const char *dirpath) {
  fileinfo *newf = (fileinfo *)malloc(sizeof(fileinfo));
  if (dp->d_type == DT_DIR) {
    newf->filetype == DT_DIR;
  }
  else {
    newf->filetype = 127;
  }
  newf->filepath = (char *)malloc(strlen(dirpath)+2+strlen(dp->d_name));
  memset(newf->filepath, '\0', strlen(dirpath)+2+strlen(dp->d_name));
  strcpy(newf->filepath, dirpath);
  strcpy(newf->filepath+strlen(dirpath), "/");
  strcpy(newf->filepath+strlen(dirpath)+1, dp->d_name);
  newf->md5value = file_md5sum(dp->d_name);
  newf->operate = 'C';
  newf->visit = 1;
  newf->left = NULL;
  newf->right = NULL;
  newf->parent = NULL;
  struct stat stat_var;
  stat(dp->d_name, &stat_var);
  newf->size = stat_var.st_size;
  return newf;
}


/*
 * 二叉树查找算法，按照fileinfo借口提的filepath来排序，
 * 节点为fileinfo结构体对象，
 * 查找的时候，如果有返回对象的指针，没有就将对象插入二叉树
 * fileinfo为二叉查找树，dp为文件的dirent对象指针，mark为是否是新增的mark，0为未找到，1为找到了，2为查找到了，但是md5值变了
 * 返回找到的节点的指针，如果是新增，返回新增节点的指针
*/
fileinfo *binarytree_s(fileinfo **p, fileinfo *p1, struct dirent *dp, int *mark, const char *dirpath) {   
  if (*p == NULL) {
    // 插入节点
    *p = new_fileinfo(dp, dirpath);
    *mark = 0;
    (*p)->parent = p1;
    return *p;
  }
  if (strcmp(dp->d_name, (*p)->filepath) < 0) {
    binarytree_s(&((*p)->left), *p, dp, mark, dirpath);
  }
  else if (strcmp(dp->d_name, (*p)->filepath) > 0) {
    binarytree_s(&((*p)->right), *p, dp, mark, dirpath);
  }
  else {
    // 查找到了，比较md5值
    char *md5value = file_md5sum(dp->d_name);
    if (strcmp((*p)->md5value, md5value) != 0) {
      // 文件改变了，fileinfo节点的md5值和operate
      free((*p)->md5value);
      (*p)->md5value = md5value;
      (*p)->operate = 'U';
    }
    *mark = 1;
    (*p)->visit = 1;
    return *p;
  }
}


/*
 * 遍历dirpath下的文件，和文件夹
 * 如果有新的文件或者文件夹，就加在全局的查找二叉树里面
 * 并且将dirpath下面的文件或文件夹对应的文件在二叉树中的节点
 * 的visit标识，标记为1
*/
void getdirfile(const char *dirpath) {
  DIR *dirp = opendir(dirpath);
  if (dirp == NULL) {
    return ;
  }

  struct dirent *dp;
  int mark;
  while ((dp = readdir(dirp)) != NULL) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
      continue;
    }
    if (dp->d_type == DT_DIR) {
      fileinfo *p = binarytree_s(&allfile, allfile, dp, &mark, dirpath);  
      char *temp = (char *)malloc(strlen(dirpath)+strlen(dp->d_name)+2);
      memset(temp, '\0', strlen(dirpath)+strlen(dp->d_name)+2);
      strcpy(temp, dirpath);
      strcpy(temp+strlen(dirpath), "/");
      strcpy(temp+strlen(dirpath)+1, dp->d_name);
      //getdirfile(dp->d_name);
      getdirfile(temp);
      free(temp);
    }
    else {
      fileinfo *p = binarytree_s(&allfile, allfile, dp, &mark, dirpath); 
    }
  }
  return;
}


/*
 * 遍历二叉查找树，得到新增的，修改的和删除的文件夹
*/
void ldr_binarytree(fileinfo *root) {
  if (root->left != NULL) {
    ldr_binarytree(root->left);
  }
  if (root->right != NULL) {
    ldr_binarytree(root->right);
  }
  if (root->visit == 0) {
    // 此文件要删除
    root->operate = 'D';
  }
  if (root->operate != 'R') {
    operatefile *temp = (operatefile *)malloc(sizeof(operatefile));
    temp->op = root;
    temp->next = NULL;
    if (of_list == NULL) {
      of_list = temp;
      of_list_tail = of_list;
    }
    else {
      of_list_tail->next = temp;
      of_list_tail = temp;
    }
  }
}


void generate_serv_result(fileinfo *pf, serv_result *sr) {
  sr->p = pf;
  strcpy(sr->md5value, pf->md5value);
  strcpy(sr->filepath, pf->filepath);
  sr->filetype = pf->filetype;
  sr->operate = pf->operate;
}


void add_epoll_event(int epollfd, uint32_t event, int fd) {
  struct epoll_event ev;
  ev.events = event;
  ev.data.fd = fd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
  }
}

void del_epoll_event(int epollfd, int fd) {
  if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
    perror("epoll_ctl: listen_sock");
  }
}


/* 
 * 从二叉排序树中删除节点 
*/
int binarytree_del(fileinfo *p) {
  //从二叉排序树中删除结点p，并重接它的左或右子树
  fileinfo *q;
  if(!p->right){       //右子树空则只需重接它的左子树
    printf("11\n");
    p->parent->left=p->left;
    free(p);
  }
  else if(!p->left){  //左子树空只需重接它的右子树
    printf("12\n");
    p->parent->right=p->right;
    free(p);
  }
  else{ //左右子树均不空
    printf("13\n");
    q=p; 
    fileinfo *s=p->left;
    while(s->right){ 
      q=s; 
      s=s->right;
    }   //转左，然后向右到尽头
    //p->data = s->data;  //s指向被删结点的“前驱”
    if(q!=p)    
      q->parent->right = s->right;    //重接*q的右子树
    else 
      q->parent->left = s->left;    //重接*q的左子树
    free(s);
  }
  return 1;
}


int deletebst(fileinfo *root, const char *path) {
  //若二叉排序树T中存在关键字等于key的数据元素时，则删除该数据元素，并返回
  //TRUE；否则返回FALSE
  if(root == NULL) {
    return -1;       //不存在关键字等于key的数据元素
  }
  else {
    if(strcmp(root->filepath, path) == 0) {    //  找到关键字等于key的数据元素
      return binarytree_del(root);
    }
    else if(strcmp(root->filepath, path) < 0)
      return deletebst(root->left, path);
    else {
      return deletebst(root->right, path);
    }
  }
}






fileinfo *searchbst(fileinfo *root, const char *path) {
  if(root == NULL) { //查找不成功
    return NULL;
  }
  else if (strcmp(root->filepath, path) == 0) { //查找成功
    return root;
  }
  else if (strcmp(root->filepath, path) < 0) {  //在左子樹中繼續查找
    return searchbst(root->left, path);
  }
  else {  //在右子樹中繼續查找
    return searchbst(root->right, path);
  }
}

