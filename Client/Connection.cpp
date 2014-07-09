/*
 * Connection.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: rio094
 */
#include "Connection.h"

using std::cout;
using std::endl;
using std::cerr;
using std::string;

const int BUFFER_SIZE = 256;

namespace connection {

Connection::Connection() {
	//cout << "Connection created !" << endl;
	/* Do nothing*/
}

Connection::Connection(const char* host, int port) {
	setConnection(host, port);
}

Connection::~Connection() {
	close(sockfd);
	//cout << "Connection detroyed !" << endl;
}

int Connection::setConnection(const char* host, const int iPort)
		throw (ConnectException) {
	port = iPort;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		throw ConnectException("Exception : opening socket");
	}
	server = gethostbyname(host);
	if (server == NULL) {
		throw ConnectException("Exception : no such host");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(port);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0) {
		throw ConnectException("Exception : cannot connect to server !");
	}

	return 1;
}

int Connection::send(const char* buffer, const int n) {
	int result = write(sockfd, buffer, n);
	return result;
}
int Connection::receive(char* buffer, const int size) {
	bzero(buffer, size);
	int n = read(sockfd, buffer, size);
	return n;
}
void Connection::Close() {
	close(sockfd);
}
}

