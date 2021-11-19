/*
 * lille_exception.cpp
 *
 *  Created on: May 27, 2020
 *      Author: Michael Oudshoorn
 */

#include <exception>
#include <iostream>
#include <string>

#include "lille_exception.h"

using namespace std;

lille_exception::lille_exception() : exception ()
{
	problem = "Unspecified lille exception detected.";
}


lille_exception::lille_exception(string s) : exception (){
	problem = s;
}


const char* lille_exception::what() const throw()
{
	return &lille_exception::problem[0];
}


