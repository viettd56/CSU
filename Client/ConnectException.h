/*
 * ConnectException.h
 *
 *  Created on: Jul 4, 2014
 *      Author: rio094
 */

#ifndef CONNECTEXCEPTION_H_
#define CONNECTEXCEPTION_H_

#include <exception>
#include <iostream>
#include <cstring>

using std::string;
using std::exception;

class ConnectException: public exception {
public:
	ConnectException();
	ConnectException(const char* s);
	virtual ~ConnectException() throw ();
	virtual const char* what() const throw ();
private:
	char *des;
};

#endif /* CONNECTEXCEPTION_H_ */
