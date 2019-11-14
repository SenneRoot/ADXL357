#ifndef _TCPSOCKET_HPP_
#define _TCPSOCKET_HPP_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024

class tcpSocket
{
	public:
	tcpSocket(int port);
	~tcpSocket();

	void receive(char *buf, uint32_t *len);
	void send(const char *buf, uint32_t *len);
	void send(double *buf, uint32_t *len);

	private:
	int m_sockfd;
	int m_connfd;
	char m_buffer[MAXLINE];
	struct sockaddr_in m_servaddr;
	struct sockaddr_in m_cliaddr;
};


#endif
