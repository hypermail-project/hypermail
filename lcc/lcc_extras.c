/* 
  stat and fstat are defined in lcc, but not lstat
*/

#include <sys/stat.h>

/*
* The way we're handling this now is to assume that we'll never try this
* on a symlink
*/
int lstat(const char *file_name, struct stat *buf)
{
    return stat(file_name, buf);
}
