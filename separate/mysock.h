/* mysock.h - wrapper for the winsock2.h so i don't have to learn in */
#ifndef MYSOCK_H__
#define MYSOCK_H__

#include <winsock2.h>

#define mysock_cleanup() WSACleanup()
int mysock_init (void);
SOCKET mysock_connect (char *ip, unsigned short port);
int mysock_send (SOCKET s, const char *message);
char * mysock_recv (SOCKET s);
char *mysock_resolve (char *address);
/* note - mysock_recv only returns part of the response if it was sent over
 *		  multiple packets because I can't be bothered to figure out how to
 * 		  make it work :)
*/

#endif /* ifndef MYSOCK_H__ */
