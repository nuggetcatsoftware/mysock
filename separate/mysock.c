#include "mysock.h"

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
        return -1;
    }
    
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
    	return -1;
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
