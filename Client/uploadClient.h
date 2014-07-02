#ifndef UPLOADCLIENT_H
#define UPLOADCLIENT_H

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

using namespace std;

namespace client{
	class UploadClient
	{
	public:
		UploadClient();

		int setConnection(const char* host, const int iPort);

		bool upload(const char* filename);
		bool resume(const char* filename,int pos);

		~UploadClient();
	private:

		int sockfd, port;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
	};
}
#endif 
