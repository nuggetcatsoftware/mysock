mysock is a wrapper for winsock because learning winsock is too much work

to use it, add these two lines somewhere near the start of your file:
```
#define MYSOCK_IMPLEMENT
#include "mysock.h"
```
the definitions are also in the header because windows does weird things when you have files including other files.
if you want the header and the implementation in separate files, check out the separate folder. its contents are 
useless if you use the file outside the folder
