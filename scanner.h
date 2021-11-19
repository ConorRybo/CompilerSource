/*
 * scanner.h
 *
 *  Created on: May 26, 2020
 *      Author: Michael Oudshoorn
 */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <iostream>
#include <fstream>
#include <string>

#include "symbol.h"
#include "token.h"
#include "error_handler.h"
#include "id_table.h"

using namespace std;

class scanner {
private:
	bool debugging {true};			// Set debugging to true to execute statement to help debug the scanner, otherwise set to false.

	const char end_marker = char(7);	// BELL character. Not typically in the source file and it is a control character < SPACE
	token* current_token;
	ifstream source_file;			// Source file to be compiled.
	error_handler* error;			// Error handler for the scanner.
	id_table* id_tab;

	int pos_on_line;				// position on current line
	int line_number;				// current line number
	bool eoln_flag;					// flag to indicate of whole string (line) has been processed

	string input_buffer;			// line from source file that is currently being processed
	char next_char;					// next character to be processed

	symbol* current_symbol;
	int current_line_number;		// current line number of the start of the token we are handling
	int current_pos_on_line;		// position on line of the start of the token we are handling
	int current_integer_value;		// value if the token is an integer value
	float current_real_value;;		// value if the token is a floating point number.
	string current_string_value;
	string current_identifier_name;

	void scan_string();				// scan in a string
	void scan_alpha();				// scan in a token beginning with a letter
	void scan_digit();				// scan in a token beginning with a digit
	void scan_special_symbol();		// scan in a token beginning with a special character
	void parse_pragma();			// parse the pragma identified by the scanner.

	scanner();						// default constructor for the scanner.
	void get_line();				// Get a line from the source file and store in the input buffer.
	void get_char();				// get the next character from the input_buffer
	char following_char();			// peek at the next character on the line. Helpful for dealing with compound symbols such as :=
	void fill_buffer();				// Call get_line() and set next_char

public:
    bool eof_flag;
    // Boolean flag to indicate if scanner has reached eof marker.

    scanner(string source_file, id_table* id_t, error_handler* e);
    // Opens the specified source file to read tokens from.
    // Id_t is the identifier table for the compiler. It is only used by the scanner to implement the behavior
    // of pragmas.
    // E is the error handler for the scanner to use.

    token* get_token();
    // Gets the next token from the input stream and returns it. The token is held in the private variable
    // current_token which is returned by the function this_token() if requested by the parser.

    bool have(symbol::symbol_type s);
    // Returns true if the current token is an s symbol, false otherwise.

    void must_be(symbol::symbol_type s);
    // The current token must be an s symbol otherwise it is a syntax error. If the current token matches
    // the symbol s, then the scanner discards the token and advances to the next token in the source file.

    token* this_token();
    // Returns the current token, without advancing to the next token in the input stream.
};

#endif /* SCANNER_H_ */
