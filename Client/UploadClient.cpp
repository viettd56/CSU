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

const int RBUFFER_LENGTH = 255;

UploadClient::UploadClient(const char* host, const int port) {
	connect = new Connection(host,port);
	//connect->setConnection(host, port);
}

UploadClient::~UploadClient() {
	delete connect;
	connect = NULL;
}

int UploadClient::upload(const char* filename) {
	//send ma bao upload
	char* buffer = new char[RBUFFER_LENGTH];
	//check file name
	ifstream is;
	is.open(filename, std::ios::binary);
	if (!is.is_open()) {
		cerr << "No such file or directory " << endl;
		return 0;
	}
	char * message = new char[RBUFFER_LENGTH + 4];
	strcpy(message, "STOR ");
	strcat(message, filename);
	connect->Open(); //open connect
	connect->send(message, strlen(message));
	connect->receive(buffer, RBUFFER_LENGTH);
	if (0 == strcmp(buffer, "125")) {
		//send data
		// get length of file:
		is.seekg(0, std::ios::end);
		int length = is.tellg();
		is.seekg(0, std::ios::beg);
		while ((length > RBUFFER_LENGTH)) {
			bzero(buffer, RBUFFER_LENGTH);
			// read data as a block:
			is.seekg(0, std::ios::cur);
			is.read(buffer, RBUFFER_LENGTH);
			length -= RBUFFER_LENGTH;
			connect->send(buffer, RBUFFER_LENGTH);
		}
		bzero(buffer, RBUFFER_LENGTH);
		is.seekg(0, std::ios::cur);
		is.read(buffer, length);
		connect->send(buffer, length);
	} else {
		// Ma 533 khong the gui du lieu
		cerr << "Error : cannot upload data " << endl;
		return 0;
	}
	connect ->Close();
	delete buffer;
	delete message;
	message = NULL;
	buffer = NULL;
	return 1; //server thong bao that bai
}
int UploadClient::resume(const char* filename) {
	//send ma bao upload
	char* buffer = new char[RBUFFER_LENGTH];
	//check file name
	ifstream is;
	is.open(filename, std::ios::binary);
	if (!is.is_open()) {
		cerr << "No such file or directory " << endl;
		return 0;
	}
	//send signal resume
	char * message = new char[RBUFFER_LENGTH + 4];
	strcpy(message, "SIZE ");
	strcat(message, filename);
	connect->Open(); //open connect
	connect->send(message, strlen(message));
	connect->receive(buffer, RBUFFER_LENGTH);
	int pos = atoi(buffer);
	//check file name
	is.open(filename, std::ios::binary);
	if (!is.is_open()) {
		cerr << "No such file or directory " << endl;
		return 0;
	}
	//send signal resume
	strcpy(message, "APPE ");
	strcat(message, filename);
	connect->send(message, strlen(message));
	connect->receive(buffer, RBUFFER_LENGTH);
	if (0 == strcmp(buffer, "125")) {
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
			connect->send(buffer, RBUFFER_LENGTH);
		}
		bzero(buffer, RBUFFER_LENGTH);
		is.seekg(0, std::ios::cur);
		is.read(buffer, RBUFFER_LENGTH);
		connect->send(buffer, length);

	} else {
		// Ma 533 khong the gui du lieu
		cerr << "Error : cannot upload data " << endl;
		return 0;
	}
	connect ->Close();
	delete buffer;
	delete message;
	message=NULL;
	buffer = NULL;
	return 1; //server thong bao that bai
}
int UploadClient::quit(){
	const char* message = "QUIT";
	connect->Open();
	connect->send(message,strlen(message));
	connect->Close();
}
}
