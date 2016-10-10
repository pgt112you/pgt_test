#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


int main() {
DIR *dir;
struct dirent *ent;
if ((dir = opendir("/usr/local/sinawatch_agent/lib/")) != NULL) {
  /* print all the files and directories within directory */
  while ((ent = readdir (dir)) != NULL) {
    printf ("%s\n", ent->d_name);
  }
  closedir (dir);
} 
else {
  /* could not open directory */
  perror ("");
}
  return 0;
}
