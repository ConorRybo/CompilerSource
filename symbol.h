/*
 * symbol.h
 *
 *  Created on: Jun 11, 2020
 *      Author: Michael Oudshoorn
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <iostream>
#include <string>

using namespace std;

class symbol {
public:

	enum symbol_type { // Lists all the possible symbols in the Language DO that can be returned by the scanner.
		nul, // This list is used by the scanner, parser, symbol table and error modules.
		identifier,
		strng,
		real_num,
		integer,
		end_of_program,
		semicolon_sym,
		comma_sym,
		colon_sym,
		equals_sym,
		not_equals_sym,
		less_than_sym,
		greater_than_sym,
		less_or_equal_sym,
		greater_or_equal_sym,
		plus_sym,
		minus_sym,
		slash_sym,
		asterisk_sym,
		power_sym,
		ampersand_sym,
		left_paren_sym,
		right_paren_sym,
		range_sym,
		becomes_sym,
		and_sym,
		begin_sym,
		boolean_sym,
		constant_sym,
		else_sym,
		elsif_sym,
		end_sym,
		eof_sym,
		exit_sym,
		false_sym,
		for_sym,
		function_sym,
		if_sym,
		in_sym,
		integer_sym,
		is_sym,
		loop_sym,
		not_sym,
		null_sym,
		odd_sym,
		or_sym,
		pragma_sym,
		procedure_sym,
		program_sym,
		read_sym,
		real_sym,
		ref_sym,
		return_sym,
		reverse_sym,
		string_sym,
		then_sym,
		true_sym,
		value_sym,
		when_sym,
		write_sym,
		writeln_sym,
		while_sym,
		invalid_sym
	};


	symbol();
	symbol(symbol_type s);
	symbol(const symbol& s2);

    bool operator==(const symbol& s2);
    symbol operator=(const symbol& sym2);

	symbol::symbol_type get_sym();
	void set_sym(symbol_type s);

	string symtostr();

private:

	symbol_type sym;

	enum {arrsize = int(symbol_type::invalid_sym) + 1};
		//string symbol_string[arrsize]; // used to help output the symbols in the enumerated type above. Helpful for debugging.

	string symbol_string[arrsize];

	void establish_symbol_map();

}; /* class symbol */

#endif /* SYMBOL_H_ */

