#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


//#ifndef CONF_FILE
//#define CONF_FILE "/home/guangtong/pgt_test/sync_file/conf/syncfile.conf"
//#endif


struct sync_conf {
  char *servaddr;
  int servport;
  int statusport;
  char *syncpath;
};


int get_config(const char *confpath, struct sync_conf *pconf); 
void del_config(struct sync_conf *pconf); 
