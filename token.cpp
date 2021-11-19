/*
 * token.cpp
 *
 *  Created on: May 27, 2020
 *      Author: Michael Oudshoorn
 */

#include <iostream>
#include <string>

#include "symbol.h"
#include "token.h"
#include "lille_exception.h"

using namespace std;


token::token()
// Constructor
{
	token::sym = new symbol(symbol::nul);
	token::line_number = 0;
	token::pos_on_line = 0;
	token::real_value = 0.0;
	token::integer_value = 0;
	token::string_value = "";
	token::identifier_value = "";
}


token::token(symbol *s, int line, int pos)
// Constructor
{
	token::sym = s;
	token::line_number = line;
	token::pos_on_line = pos;
	token::real_value = 0.0;
	token::integer_value = 0;
	token::string_value = "";
	token::identifier_value = "";
}


token::token(const token& t)
// copy constructor
{
	token::sym = t.sym;
	token::line_number = t.line_number;
	token::pos_on_line = t.pos_on_line;
	token::real_value = t.real_value;
	token::integer_value = t.integer_value;
	token::string_value = t.string_value;
	token::identifier_value = t.identifier_value;
}


token& token::operator=(const token& t)
// copy assignment
{
	token::sym = t.sym;
	token::line_number = t.line_number;
	token::pos_on_line = t.pos_on_line;
	token::real_value = t.real_value;
	token::integer_value = t.integer_value;
	token::string_value = t.string_value;
	token::identifier_value = t.identifier_value;
	return *this;
}


symbol::symbol_type token::get_sym()
// returns the symbol.
{
	return sym->get_sym();
}


symbol * token::get_symbol()
// returns the symbol.
{
	return sym;
}


int token::get_line_number()
// returns the line number
{
	return line_number;
}


int token::get_pos_on_line()
// returns the position on the line.
{
	return pos_on_line;
}


float token::get_real_value()
// returns the real only if the symbol is a real_number. Raises a lille_exceeption otherwise.
{
	if (sym->get_sym() == symbol::real_num)
		return real_value;
	else
		throw lille_exception("Illegal access to real_value in token.");
}


int token::get_integer_value()
// returns the integer_value only if the symbol is a integer_number. Raises a lille_exceeption otherwise.
{
	if (sym->get_sym() == symbol::integer)
		return integer_value;
	else
		throw lille_exception("Illegal access to integer_value in token.");
}


string token::get_string_value()
// returns the string_value only if the symbol is a string. Raises a lille_exceeption otherwise.
{
	if (sym->get_sym() == symbol::strng)
		return string_value;
	else
		throw lille_exception("Illegal access to string_value in token.");
}


string token::get_identifier_value()
// returns the string_value only if the symbol is an identifier. Raises a lille_exceeption otherwise.
{
	if (sym->get_sym() == symbol::identifier)
		return identifier_value;
	else
		return "";
		//throw lille_exception("Illegal access to identifier_value in token.");
}


void token::set_real_value(float f)
// Set the real_value to f only if the token represents a real_value. Raise an exception otherwise.
{
	if (sym->get_sym() == symbol::real_num)
		real_value = f;
	else
		throw lille_exception("Illegal attempt to set real_value in token");
}


void token::set_integer_value(int i)
// Set the ingteger_value to i only if the token represents a integer_value. Raise an exception otherwise.
{
	if (sym->get_sym() == symbol::integer)
		integer_value = i;
	else
		throw lille_exception("Illegal attempt to set integer_value in token");
}


void token::set_string_value(string s)
// Set the string_value to s only if the token represents a string_value. Raise an exception otherwise.
{
	if (sym->get_sym() == symbol::strng)
		string_value = s;
	else
		throw lille_exception("Illegal attempt to set string_value in token");
}


void token::set_identifier_value(string s)
// Set the identifier_value to s only if the token represents an identifier. Raise an exception otherwise.
{
	if (sym->get_sym() == symbol::identifier)
		identifier_value = s;
	else
		throw lille_exception("Illegal attempt to set identifier_value in token");
}


void token::print_token()
{
	cout << "TOKEN: " << sym->symtostr();
	if (sym->get_sym() == symbol::real_num)
		cout << "  Value: " << real_value;
	else if (sym->get_sym() == symbol::integer)
		cout << "  Value: " << integer_value;
	else if (sym->get_sym() == symbol::strng)
		cout << "  Value: " << string_value;
	else if (sym->get_sym() == symbol::identifier)
		cout << "  Value: " << identifier_value;
	cout << "  Line No: " << line_number << " Pos on line: " << pos_on_line << endl;
}


string token::to_string()
{
	string s = "";
	s += "Name: " + this->identifier_value;
	s += " Line No: " + ::to_string(line_number);
	s += " Position: " + ::to_string(pos_on_line);
	return s;
}
