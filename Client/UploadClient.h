/*
 * UploadClient.h
 *
 *  Created on: Jul 4, 2014
 *      Author: rio094
 */

#ifndef UPLOADCLIENT_H_
#define UPLOADCLIENT_H_

#include <cstddef>
#include "Connection.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace client {

using connection::Connection;
using std::cerr;
using std::endl;

class UploadClient {
public:
	UploadClient(const char* host, const int pos);
	virtual ~UploadClient();

	int upload(const char* filename);
	int resume(const char* filename);
private:
	Connection* connect;
};

}
#endif /* UPLOADCLIENT_H_ */
