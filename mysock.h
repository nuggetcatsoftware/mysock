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
 *		    multiple packets because I can't be bothered to figure out how to
 * 		    make it work :)
 *      - strings returned by mysock_recv should be freed
*/

#ifdef MYSOCK_IMPLEMENT
#define MYSOCK_RECV_UNIT 512

int mysock_init (void)
{
	WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        return 1;
    }
}

SOCKET mysock_connect (char *ip, unsigned short port)
{
	SOCKET s;
	struct sockaddr_in server;
    
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        //return -1;
        exit (120);
    }
    
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
    	//return -1;
    	exit (120);
    }
	return s;
}

int mysock_send (SOCKET s, const char *message)
{
	if (send(s, message, strlen(message), 0) < 0) {
        return 1;
    }
    return 0;
}

char * mysock_recv (SOCKET s)
{
	int recv_size;
	char *ixy_ = NULL, *end_marker;
	
	for (size_t times_grown = 1;;times_grown++) {
		if (! (ixy_ = realloc (ixy_, times_grown * MYSOCK_RECV_UNIT))) {
			return NULL;
		}
		end_marker = ixy_ + (times_grown - 1) * MYSOCK_RECV_UNIT;
		
		if ((recv_size = recv(s, end_marker - 1, MYSOCK_RECV_UNIT + (times_grown == 1), 0)) == SOCKET_ERROR) {
			return NULL;
    	}
    	if (recv_size == 0) {
			return NULL;
		}
    	if (recv_size < MYSOCK_RECV_UNIT + (times_grown == 1)) {
			break;	/* exit loop if all the stuff is received */
		}
	}
    end_marker[recv_size] = '\0';
    
	return ixy_;
}

char * mysock_resolve (char *address)
{
	struct hostent *he;
	struct in_addr **addr_list;
	
	if (! (he = gethostbyname (address))) {
		return NULL;
	}
    
	return inet_ntoa (*((struct in_addr **)he -> h_addr_list)[0]);
}
#endif /* ifdef MYSOCK_IMPLEMENT */

#endif /* ifndef MYSOCK_H__ */
