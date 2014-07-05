/*
 * UploadClient.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: rio094
 *  Lop uploadClient : upload hoac resume 1 file den server thong qua ket noi Connection
 */

#include "UploadClient.h"
namespace client {
using std::ifstream;

const int RBUFFER_LENGTH = 260;
const int BUFFER_LENGTH = 1024;

UploadClient::UploadClient(const char* host, const int port) {
	connect = new Connection;
	connect->setConnection(host, port);
}

UploadClient::~UploadClient() {
	delete connect;
	connect = NULL;
}

int UploadClient::upload(const char* filename) {
	//send ma bao upload
	char* buffer = new char[RBUFFER_LENGTH];
	bzero(buffer, RBUFFER_LENGTH);
	strcpy(buffer, "STOR");
	strcat(buffer, filename);

	int n = connect->send(buffer, strlen(buffer));
	if (n < 0)
		throw ConnectException("Exception: cannot send data !");
	bzero(buffer, RBUFFER_LENGTH);
	n = connect->receive(buffer, RBUFFER_LENGTH);
	if (n < 0)
		throw ConnectException("Exception : cannot receive data !");
	if (strcpy(buffer, "125")) {
		//ok send data
		ifstream is;
		is.open(filename, std::ios::binary);
		if (!is.is_open()) {
			cerr << "No such file or directory " << endl;
			return 0;
		}
		// get length of file:
		is.seekg(0, std::ios::end);
		int length = is.tellg();
		std::stringstream strs;
		strs << length;
		string temp_str = strs.str();
		char* length_mess = (char*) temp_str.c_str();

		connect->send(length_mess, strlen(length_mess)); // gui dung luong file
		bzero(buffer, RBUFFER_LENGTH);
		connect->receive(buffer, RBUFFER_LENGTH);
		if (strcpy(buffer, "126")) {
			//send data
			int length_temp = length;
			is.seekg(0, std::ios::beg);
			do {
				// allocate memory:
				bzero(buffer, RBUFFER_LENGTH);
				// read data as a block:
				is.seekg(0, std::ios::cur);
				is.read(buffer, RBUFFER_LENGTH);
				n = connect->send(buffer, RBUFFER_LENGTH);
				if (n < 0) {
					cerr << "Error : cannot upload data " << endl;
					return 0;
				}
				length_temp -= RBUFFER_LENGTH;
			} while (length_temp >= RBUFFER_LENGTH);
		}
	} else {
		//cannot send data
		cerr << "Error : cannot upload data " << endl;
		return 0;
	}
	delete buffer;
	buffer = NULL;
	return 1;
}

}
