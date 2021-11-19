/*
 * symbol.cpp
 *
 *  Created on: Jun 11, 2020
 *      Author: Michael Oudshoorn
 */


#include <iostream>
#include <string>
#include <cctype>
#include <cmath>

#include "symbol.h"

using namespace std;


symbol::symbol()
{
	sym = invalid_sym;
	establish_symbol_map();
}


symbol::symbol(symbol::symbol_type s) : symbol()
{
	sym = s;
}


symbol::symbol(const symbol& s2)
{
	symbol::sym = s2.sym;

}


bool symbol::operator==(const symbol& s2)
{
	return (sym == s2.sym);
}


symbol symbol::operator=(const symbol& sym2)
{
	symbol::sym = sym2.sym;
	return *this;
}


symbol::symbol_type symbol::get_sym()
{
	return sym;
}


void symbol::set_sym(symbol_type s)
{
	sym = s;
}


string symbol::symtostr()
{
	return symbol_string[sym];
}


void symbol::establish_symbol_map()
{

	// Set up the mapping of symbol_type to a string for printing purposes.

	symbol::symbol_string[nul] = "Nul";
	symbol::symbol_string[identifier] = "Identifier";
	symbol::symbol_string[strng] = "Strng";
	symbol::symbol_string[real_num] = "Real_num";
	symbol::symbol_string[integer] = "Integer";
	symbol::symbol_string[end_of_program] = "End_of_program";
	symbol::symbol_string[semicolon_sym] = "Semicolon_sym";
	symbol::symbol_string[comma_sym] = "Comma_sym";
	symbol::symbol_string[colon_sym] = "Colon_sym";
	symbol::symbol_string[equals_sym] = "Equals_sym";
	symbol::symbol_string[not_equals_sym] = "Not_equals_sym";
	symbol::symbol_string[less_than_sym] = "less_than_sym";
	symbol::symbol_string[greater_than_sym] = "Greater_than_sym";
	symbol::symbol_string[less_or_equal_sym] = "Less_or_equal_sym";
	symbol::symbol_string[greater_or_equal_sym] = "Greater_or_equal_sym";
	symbol::symbol_string[plus_sym] = "Plus_sym";
	symbol::symbol_string[minus_sym] = "Minus_sym";
	symbol::symbol_string[slash_sym] = "Slash_sym";
	symbol::symbol_string[asterisk_sym] = "Asterisk_sym";
	symbol::symbol_string[power_sym] = "Power_sym";
	symbol::symbol_string[ampersand_sym] = "Ampersand_sym";
	symbol::symbol_string[left_paren_sym] = "Left_paren_sym";
	symbol::symbol_string[right_paren_sym] = "Right_paren_sym";
	symbol::symbol_string[range_sym] = "Range_sym";
	symbol::symbol_string[becomes_sym] = "Becomes_sym";
	symbol::symbol_string[and_sym] = "And_sym";
	symbol::symbol_string[begin_sym] = "Begin_sym";
	symbol::symbol_string[boolean_sym] = "Boolean_sym";
	symbol::symbol_string[constant_sym] = "Constant_sym";
	symbol::symbol_string[else_sym] = "Else_sym";
	symbol::symbol_string[elsif_sym] = "Elsif_sym";
	symbol::symbol_string[end_sym] = "End_sym";
	symbol::symbol_string[eof_sym] = "Eof_sym";
	symbol::symbol_string[exit_sym] = "Exit_sym";
	symbol::symbol_string[false_sym] = "False_sym";
	symbol::symbol_string[for_sym] = "For_sym";
	symbol::symbol_string[function_sym] = "Function_sym";
	symbol::symbol_string[if_sym] = "If_sym";
	symbol::symbol_string[in_sym] = "In_sym";
	symbol::symbol_string[integer_sym] = "Integer_sym";
	symbol::symbol_string[is_sym] = "Is_sym";
	symbol::symbol_string[loop_sym] = "Loop_sym";
	symbol::symbol_string[not_sym] = "Not_sym";
	symbol::symbol_string[null_sym] = "Null_sym";
	symbol::symbol_string[odd_sym] = "Odd_sym";
	symbol::symbol_string[or_sym] = "Or_sym";
	symbol::symbol_string[pragma_sym] = "Pragma_sym";
	symbol::symbol_string[procedure_sym] = "Procedure_sym";
	symbol::symbol_string[program_sym] = "Program_sym";
	symbol::symbol_string[read_sym] = "Read_sym";
	symbol::symbol_string[real_sym] = "Real_sym";
	symbol::symbol_string[ref_sym] = "Ref_sym";
	symbol::symbol_string[return_sym] = "Return_sym";
	symbol::symbol_string[reverse_sym] = "Reverse_sym";
	symbol::symbol_string[string_sym] = "String_sym";
	symbol::symbol_string[then_sym] = "Then_sym";
	symbol::symbol_string[true_sym] = "True_sym";
	symbol::symbol_string[value_sym] = "Value_sym";
	symbol::symbol_string[when_sym] = "When_sym";
	symbol::symbol_string[write_sym] = "Write_sym";
	symbol::symbol_string[writeln_sym] = "Writeln_sym";
	symbol::symbol_string[while_sym] = "While_sym";
	symbol::symbol_string[invalid_sym] = "Invalid_sym";
}
