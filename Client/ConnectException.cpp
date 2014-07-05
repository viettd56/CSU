/*
 * ConnectException.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: rio094
 */

#include "ConnectException.h"

ConnectException::ConnectException() {
	des = new char[15];
	strcpy(des, "Cannot connect!");
}
ConnectException::ConnectException(const char* s) {
	int length = strlen(s);
	des = new char[length];
	strcpy(des, s);
}
ConnectException::~ConnectException() throw() {
	delete des;
	des = NULL;
}
const char* ConnectException::what() const throw () {
	return des;
}
