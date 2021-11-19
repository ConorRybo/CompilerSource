/*
 * token.h
 *
 *  Created on: May 27, 2020
 *      Author: Michael Oudshoorn
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

#include "symbol.h"
#include "lille_exception.h"

using namespace std;


class token {
private:
	symbol* sym;				// Symbol identified.
	int line_number;			// Line number in source file where symbol is located.
	int pos_on_line;			// Position on line in source file where the symbol is located.
	float real_value;			// If symbol represents a real number, this is its value.
	int integer_value;			// If symbol represents an integer value, this is its value.
	string string_value;		// If symbol represents a string value, this is its value.
	string identifier_value;	// If the symbol represents an identifier, this is its name.

public:
	token();
	token(symbol* s, int line, int pos);	// create a token - constructor.

	token(const token& t);		// copy constructor
	token& operator=(const token& t);	// copy assignment

	symbol::symbol_type get_sym();			// returns the symbol.
	symbol* get_symbol();
	int get_line_number();		// returns the line number
	int get_pos_on_line();		// returns the position on the line;
	float get_real_value();		// returns the real only if the symbol is a real_number. Raises a DO_exceeption otherwise.
	int get_integer_value();	// returns the integer_value only if the symbol is a integer_number. Raises a DO_exceeption otherwise.
	string get_string_value();	// returns the string_value only if the symbol is a string. Raises a DO_exceeption otherwise.
	string get_identifier_value(); // returns the string_value only if the symbol is an identifier. Raises a Lille_exceeption otherwise.

	void set_real_value(float f); 	// Set the real_value to f only if the token represents a real_value. Raise an exception otherwise.
	void set_integer_value(int i);	// Set the ingteger_value to i only if the token represents a integer_value. Raise an exception otherwise.
	void set_string_value(string s);	// Set the string_value to s only if the token represents a string_value. Raise an exception otherwise.
	void set_identifier_value(string s);	// Set the identifier_value to s only if the token represents an identifier. Raise an exception otherwise.

	void print_token();			// print out the token. Helpful for debugging.

	string to_string();

};

#endif /* TOKEN_H_ */
