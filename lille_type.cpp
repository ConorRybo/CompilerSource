/*
 * lilletype.cpp
 *
 *  Created on: Jun 18, 2020
 *      Author: Michael Oudshoorn
 */

#include "lille_type.h"

#include <iostream>
#include <string>

#include "lille_exception.h"

using namespace std;

lille_type::lille_type()
// constructor
{
	ty = lille_type::type_unknown;
}


lille_type::lille_type(lille_ty typ)
// constructor
{
	ty = typ;
}



lille_type::lille_ty lille_type::get_type()
// returns type associated with object
{
	return ty;
}


bool lille_type::is_type(lille_type typ)
// test to see if the object os a specific type
{
	return (this->ty == typ.ty);
}


bool lille_type::is_type(lille_ty typ)
// test to see if the object os a specific type
{
	return (this->ty == typ);
}


bool lille_type::is_equal(lille_type typ)
{
	if (this->ty == typ.ty)
		return true;
	else if (this->ty == type_arith_or_string)
	{
		if ((typ.ty == type_real)
				or (typ.ty == type_integer)
				or (typ.ty == type_string)
				or (typ.ty == type_arith)
				or (typ.ty == type_arith_or_string))
			return true;
		else
			return false;
	}
	else if (typ.ty == type_arith_or_string)
	{
		if ((this->ty == type_real)
				or (this->ty == type_integer)
				or (this->ty == type_string)
				or (this->ty == type_arith)
				or (this->ty == type_arith_or_string))
			return true;
		else
			return false;
	}
	else if (this->ty == type_arith)
	{
		if ((typ.ty == type_real)
				or (typ.ty == type_integer)
				or (typ.ty == type_string)
				or (typ.ty == type_arith))
			return true;
		else
			return false;
	}
	else if (typ.ty == type_arith)
	{
		if ((this->ty == type_real)
				or (this->ty == type_integer)
				or (this->ty == type_string)
				or (this->ty == type_arith))
			return true;
		else
			return false;
	}
	else
		return ((this->ty == type_unknown) or (typ.ty == type_unknown));	// default finding
}


int lille_type::size_of()
// returns size of the object so that space can be reserved appropriately.
// In the PAL machine, everything is of size 1, just to make little a little simpler.
{
	switch (this->ty)
	{
		case type_integer:
		case type_real:
		case type_string:
		case type_boolean:
		case type_arith:
		case type_arith_or_string:
			return 1;
			break;
		case type_proc:
		case type_func:
			throw lille_exception("Internal compiler error. Attempt to request size of a procedure or function.");
			return 0;
			break;
		case type_unknown:
			return 0;	// error already handled most likely
			break;
		case type_prog:
			return 0;
			break;
		default:
			return 0;	// all cases are covered in the switch so this is technically unreachable.
			break;
	}
}


string lille_type::to_string()
{
	switch (this->ty)
	{
		case type_integer:
			return "INTEGER";
			break;
		case type_real:
			return "REAL";
			break;
		case type_string:
			return "STRING";
			break;
		case type_boolean:
			return "BOOLEAN";
			break;
		case type_arith:
			return "ARITH";
			break;
		case type_arith_or_string:
			return "ARITH_OR_STRING";
			break;
		case type_proc:
			return "PROC";
			break;
		case type_func:
			return "FUNC";
			break;
		case type_unknown:
			return "UNKNOWN";
			break;
		case type_prog:
			return "PROG";
			break;
		default:
			return "ERROR";	// all cases are covered in the switch so this is technically unreachable.
			break;
	}
}
