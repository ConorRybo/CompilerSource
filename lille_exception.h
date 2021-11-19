/*
 * lille_exception.h
 *
 *  Created on: May 27, 2020
 *      Author: Michael oudshoorn
 */

#ifndef LILLE_EXCEPTION_H_
#define LILLE_EXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

class lille_exception : public exception
{
private:
	string problem;

public:
	lille_exception();

	lille_exception(string s);

	virtual const char* what() const throw();
};

#endif /* LILLE_EXCEPTION_H_ */
