/* 
  stat and fstat are defined in lcc, but not lstat
*/

int lstat(const char *file_name, struct stat *buf);
