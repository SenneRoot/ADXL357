#include "udpSocket.hpp"

udpSocket::udpSocket(int port)
{
	if ( (m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
      	perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

		m_servaddr.sin_family    = AF_INET; // IPv4
    m_servaddr.sin_addr.s_addr = INADDR_ANY;
    m_servaddr.sin_port = htons(port);

		if ( bind(m_sockfd, (const struct sockaddr *) &m_servaddr, sizeof(m_servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

		printf("Connected to udp on port: %d\n", port);
}

udpSocket::~udpSocket()
{

}

void udpSocket::receive(char *buf, struct sockaddr *clientAddress , uint32_t *len)
{
	uint n = recvfrom(m_sockfd, buf, MAXLINE, MSG_WAITALL, clientAddress, len);
	buf[n] = '\0';
}

void udpSocket::send(const char *buf, const struct sockaddr *clientAddress , uint32_t *len)
{
	sendto(m_sockfd, buf, strlen(buf), MSG_CONFIRM, clientAddress, *len);
}

void udpSocket::send(double *buf, const struct sockaddr *clientAddress , uint32_t *len)
{
	const char buffer[MAXLINE];
	sprintf(buffer, "%f", *buf);
	sendto(m_sockfd, buffer, sizeof(buf), MSG_CONFIRM, clientAddress, *len);
}
