#ifndef _UDPSOCKET_HPP_
#define _UDPSOCKET_HPP_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024

class udpSocket
{
	public:
	udpSocket(int port);
	~udpSocket();

	void receive(char *buf, struct sockaddr *clientAddress ,uint32_t *len);
	void send(const char *buf, const struct sockaddr *clientAddress ,uint32_t *len);
	void send(const double *buf, const struct sockaddr *clientAddress , uint32_t *len);

	private:
	int m_sockfd;
	char m_buffer[MAXLINE];
	struct sockaddr_in m_servaddr;
};


#endif
