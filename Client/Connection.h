/*
 * Connection.h
 *
 *  Created on: Jul 3, 2014
 *      Author: rio094
 *  Lop connection : tao ket noi socket tu client den server
 *  co the gui va nhan du lieu qua ket noi
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ConnectException.h"

namespace connection {

class Connection {
public:
	Connection();
	Connection(const char* host, const int port);
	~Connection();

	int setConnection(const char* host, const int iPort)
			throw (ConnectException);

	int send(const char* buffer, const int n);
	int receive(char* buffer, const int n);

	void Close();

private:

	int sockfd, port;
	struct sockaddr_in serv_addr;
	struct hostent *server;
};

}
#endif // CONNECTION_H

