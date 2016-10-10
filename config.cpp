#include "config.h"


#define MAX_CONF_LEN 256


char *get_value(char *pos) {
  while(isspace(*pos)) ++pos;
  char *value_start = pos;
  while(!isspace(*pos)) ++pos;
  char *temp = (char *)malloc(pos - value_start + 1);
  memset(temp, '\0', pos-value_start+1);
  strncpy(temp, value_start, pos-value_start);
  return temp;
}

int get_config(const char *confpath, struct sync_conf *pconf) {
  FILE *fp = fopen(confpath, "r");
  if (fp == NULL) {
    printf("get config error\n");
	return -1;
  }
  char *buf = (char *)malloc(MAX_CONF_LEN);
  memset(buf, '\0', MAX_CONF_LEN);
  while(fgets(buf, MAX_CONF_LEN, fp) != NULL) {
    char *pos = buf;
	while(isspace(*pos)) ++pos;
	if (*pos == '#' || *pos == '\0' || *pos == '\n') {
      continue;
	}
    char *name_start = pos;
	char *value_start;
	while(!isspace(*pos)) ++pos; 
	if (strncmp("servaddr", name_start, (pos-name_start)) == 0) {
	  pconf->servaddr = get_value(pos);
	}
	else if (strncmp("servport", name_start, (pos-name_start)) == 0) {
	  char *temp = get_value(pos);
	  pconf->servport = atoi(temp);
	  free(temp);
	}
	else if (strncmp("statusport", name_start, (pos-name_start)) == 0) {
	  char *temp = get_value(pos);
	  pconf->statusport = atoi(temp);
	  free(temp);

	}
	else if (strncmp("syncpath", name_start, (pos-name_start)) == 0) {
      pconf->syncpath = get_value(pos);
	}
	else {
      ;
	}
	memset(buf, '\0', MAX_CONF_LEN); 
  }
  return 0;
}


void del_config(struct sync_conf *pconf) {
  free(pconf->servaddr);
  free(pconf->syncpath);
}
