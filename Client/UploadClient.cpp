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

const int RBUFFER_LENGTH = 256;
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
	char* buffer = new char[RBUFFER_LENGTH + 5];
	bzero(buffer, RBUFFER_LENGTH);
	//check file name
	ifstream is;
	is.open(filename, std::ios::binary);
	if (!is.is_open()) {
		cerr << "No such file or directory " << endl;
		return 0;
	}
	//send signal upload
	strcpy(buffer, "STOR ");
	strcat(buffer, filename);
	std::cout << "Send file : " << buffer << std::endl;
	int n = connect->send(buffer, strlen(buffer));
	if (n < 0)
		throw ConnectException("Exception: cannot send data !");
	bzero(buffer, RBUFFER_LENGTH);
	n = connect->receive(buffer, RBUFFER_LENGTH);
	std::cout << "Received : " << buffer << std::endl;
	if (n < 0)
		throw ConnectException("Exception : cannot receive data !");
	if (0 == strcmp(buffer, "125")) {
		std::cout << "sending " << std::endl;
		//send data
		// get length of file:
		is.seekg(0, std::ios::end);
		int length = is.tellg();
		is.seekg(0, std::ios::beg);
		while (length > RBUFFER_LENGTH) {
			bzero(buffer, RBUFFER_LENGTH);
			// read data as a block:
			is.seekg(0, std::ios::cur);
			is.read(buffer, RBUFFER_LENGTH);
			length -= RBUFFER_LENGTH;
			n = connect->send(buffer, RBUFFER_LENGTH);
		}
		bzero(buffer, RBUFFER_LENGTH);
		is.seekg(0, std::ios::cur);
		is.read(buffer, RBUFFER_LENGTH);
		n = connect->send(buffer, length);

	} else {
		// Ma 533 khong the gui du lieu
		cerr << "Error : cannot upload data " << endl;
		return 0;
	}

	//check when send complete
	bzero(buffer, RBUFFER_LENGTH);
	n = connect->receive(buffer, RBUFFER_LENGTH);
	std::cout << "Received : " << buffer << std::endl;
	if (n < 0)
		throw ConnectException("Exception : cannot receive data !");
	if (0 == strcmp(buffer, "250")) {
		delete buffer;
		buffer = NULL;
		return 1; // server thong bao upload thanh cong
	}
	delete buffer;
	buffer = NULL;
	return 0; //server thong bao that bai
}
int UploadClient::resume(const char* filename) {
	//send ma bao upload
	char* buffer = new char[RBUFFER_LENGTH + 5];
	bzero(buffer, RBUFFER_LENGTH);
	//check file name
	ifstream is;
	is.open(filename, std::ios::binary);
	if (!is.is_open()) {
		cerr << "No such file or directory " << endl;
		return 0;
	}
	//send signal resume
	strcpy(buffer, "SIZE ");
	strcat(buffer, filename);
	std::cout << "resume file : " << buffer << std::endl;
	int n = connect->send(buffer, strlen(buffer));
	if (n < 0)
		throw ConnectException("Exception: cannot send data !");
	bzero(buffer, RBUFFER_LENGTH);
	n = connect->receive(buffer, RBUFFER_LENGTH);
	std::cout << "Received : " << buffer << std::endl;
	int pos = atoi(buffer);
	//check file name
	is.open(filename, std::ios::binary);
	if (!is.is_open()) {
		cerr << "No such file or directory " << endl;
		return 0;
	}
	//send signal resume
	strcpy(buffer, "SIZE ");
	strcat(buffer, filename);
	n = connect->send(buffer, strlen(buffer));
	if (n < 0)
		throw ConnectException("Exception: cannot send data !");
	if (0 == strcmp(buffer, "125")) {
		std::cout << "sending " << std::endl;
		//send data
		// get length of file:
		is.seekg(0, std::ios::end);
		int length = is.tellg();
		length -= pos; // so luong byte chua gui
		is.seekg(pos, std::ios::beg); // chuyen den vi tri bat dau gui
		while (length > RBUFFER_LENGTH) {
			bzero(buffer, RBUFFER_LENGTH);
			// read data as a block:
			is.seekg(0, std::ios::cur);
			is.read(buffer, RBUFFER_LENGTH);
			length -= RBUFFER_LENGTH;
			n = connect->send(buffer, RBUFFER_LENGTH);
		}
		bzero(buffer, RBUFFER_LENGTH);
		is.seekg(0, std::ios::cur);
		is.read(buffer, RBUFFER_LENGTH);
		n = connect->send(buffer, length);

	} else {
		// Ma 533 khong the gui du lieu
		cerr << "Error : cannot upload data " << endl;
		return 0;
	}

	//check when send complete
	bzero(buffer, RBUFFER_LENGTH);
	n = connect->receive(buffer, RBUFFER_LENGTH);
	std::cout << "Received : " << buffer << std::endl;
	if (n < 0)
		throw ConnectException("Exception : cannot receive data !");
	if (0 == strcmp(buffer, "250")) {
		delete buffer;
		buffer = NULL;
		return 1; // server thong bao upload thanh cong
	}
	delete buffer;
	buffer = NULL;
	return 0; //server thong bao that bai
}
}
