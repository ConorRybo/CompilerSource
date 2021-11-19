/*
 * error_handler.cpp
 *
 *  Created on: May 28, 2020
 *      Author: Michael Oudshoorn
 */

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <iomanip>

#include "token.h"
#include "lille_exception.h"
#include "error_handler.h"

using namespace std;

error_handler::error_handler()
// Constructor with no source file name or listing file name identified.
{
	error_num = 0;
	err_list = NULL;
	listing_required = false;
	initialize_error_messages();
	error_limit = 10000;
	listing_filename = "";
	if (filesystem::exists(string(default_source_file_name)))
	{												// Check file exists
		source_file.open(default_source_file_name); // Open source file for reading.
	}
	else
	{
		cerr << "File named \"" + default_source_file_name + "\" does not exist." << endl;
		throw("File named \"" + default_source_file_name + "\" does not exist.");
	}
	listing_file.open(default_listing_file_name);
}

error_handler::error_handler(string source_file_name)
// Constructor. No listing file needed
{
	error_num = 0;
	err_list = NULL;
	listing_required = false;
	initialize_error_messages();
	error_limit = 10000;
	listing_filename = "";

	if (filesystem::exists(source_file_name))
	{										// Check file exists
		source_file.open(source_file_name); // Open source file for reading.
	}
	else
		throw("File named \"" + source_file_name + "\" does not exist.");
	// listing_file.open(default_listing_file_name);
}

error_handler::error_handler(string source_file_name, string list_file_name)
// Constructor. Specifies name of listing file.
{
	listing_required = true;
	error_num = 0;
	listing_required = true;
	listing_filename = list_file_name;
	initialize_error_messages();
	error_limit = 10000;
	if (filesystem::exists(string(source_file_name)))
	{										// Check file exists
		source_file.open(source_file_name); // Open source file for reading.
	}
	else
		throw("File named \"" + source_file_name + "\" does not exist.");
}

void error_handler::initialize_error_messages()
// Array of error messages is initialized. This allows the language of the error messages to be changed easily.
{
	error_message[0] = "Identifier expected.";
	error_message[1] = "A string is expected.";
	error_message[2] = "A real number is expected.";
	error_message[3] = "An integer is expected.";
	error_message[4] = "End of program expected.";
	error_message[5] = "A semicolon (;) is expected.";
	error_message[6] = "A colon (:) is expected.";
	error_message[7] = "A comma (,) is expected.";
	error_message[8] = "An equals (=) sign is expected.";
	error_message[9] = "A not equals (<>) sign is expected.";
	error_message[10] = "A less than (<) symbol expected.";
	error_message[11] = "A greater than (>) symbol expected.";
	error_message[12] = "A less then or equal (<=) symbol expected.";
	error_message[13] = "A greater than or equal )>=) symbol expected.";
	error_message[14] = "A plus (+) sign is expected.";
	error_message[15] = "A minus (-) sign is expected.";
	error_message[16] = "A slash (/) sign is expected.";
	error_message[17] = "An asterisk (*) is expected.";
	error_message[18] = "A power (**) sign is expected.";
	error_message[19] = "An ampersand (&) is expected.";
	error_message[20] = "A left parenthesis (() is expected.";
	error_message[21] = "A right parenthesis ()) is expected.";
	error_message[22] = "A range symbol (..) is expected.";
	error_message[23] = "A becomes (:=) symbol is expected.";
	error_message[24] = "An AND symbol is expected.";
	error_message[25] = "A BEGIN symbol is expected.";
	error_message[26] = "A BOOLEAN symbol is expected.";
	error_message[27] = "A CONSTANT symbol is expected.";
	error_message[28] = "An ELSE symbol is expected.";
	error_message[29] = "An ELSIF symbol is expected.";
	error_message[30] = "An END symbol is expected.";
	error_message[31] = "An EOF symbol is expected.";
	error_message[32] = "An EXIT symbol is expected.";
	error_message[33] = "A FALSE symbol is expected.";
	error_message[34] = "A FOR symbol is expected.";
	error_message[35] = "A FUNCTION symbol is expected.";
	error_message[36] = "An IF symbol is expected.";
	error_message[37] = "An IN symbol is expected.";
	error_message[38] = "An INTEGER symbol is expected.";
	error_message[39] = "An IS symbol is expected.";
	error_message[40] = "A LOOP symbol is expected.";
	error_message[41] = "A NOT symbol is expected.";
	error_message[42] = "A NULL symbol is expected.";
	error_message[43] = "An ODD symbol is expected.";
	error_message[44] = "An OR symbol is expected.";
	error_message[45] = "A PRAGMA symbol is expected.";
	error_message[46] = "A PROCEDURE symbol is expected.";
	error_message[47] = "A PROGRAM symbol is expected";
	error_message[48] = "A READ symbol is expected.";
	error_message[49] = "A REAL symbol is expected.";
	error_message[50] = "A REF symbol is expected.";
	error_message[51] = "A RETURN symbol is expected.";
	error_message[52] = "A REVVERSE symbol is expected.";
	error_message[53] = "A STRING symbol is expected.";
	error_message[54] = "A THEN symbol is expected.";
	error_message[55] = "A TRUE symbol is expected.";
	error_message[56] = "A VALUE symbol is expected.";
	error_message[57] = "A WHILE symbol is expected.";
	error_message[58] = "A WRITE statement is expected.";
	error_message[59] = "A WRITELN symbol is expected.";
	error_message[60] = "String must be terminated before the end of line is encountered.";
	error_message[61] = "Illegal underscore in identifier.";
	error_message[62] = "Number too large.";
	error_message[63] = "Real number must have digits after the dot/period.";
	error_message[64] = "Must have digits after exponent symbol.";
	error_message[65] = "Too many digits in the exponent.";
	error_message[66] = "Floating point number too large or malformed.";
	error_message[67] = "An integer can only have a positive exponent.";
	error_message[68] = "Integer number too large or malformed.";
	error_message[69] = "Malformed pragma.";
	error_message[70] = "Illegal pragma name.";
	error_message[71] = "Pragma ERROR_LIMIT requires a numeric argument.";
	error_message[72] = "Variable name expected.";
	error_message[73] = "ON or OFF expected.";
	error_message[74] = "illegal character.";
	error_message[75] = "Identifier name must match program name.";
	error_message[76] = "Block expected.";
	error_message[77] = "End of program expected. No symbols permitted after end of program.";
	error_message[78] = "Declaration or 'begin' expected.";
	error_message[79] = "Error in statement.";
	error_message[80] = "Statement expected.";
	error_message[81] = "Identifier not previously declared.";
	error_message[82] = "Identifier declared multiple times in same block.";
	error_message[83] = "A simple statement expected.";
	error_message[84] = "Integer, real, or string expression expected.";
	error_message[85] = "Identifier is not assignable. Must be a variable or reference parameter.";
	error_message[86] = "Integer or real variable expected.";
	error_message[87] = "Type of expression does not match function return type.";
	error_message[88] = "Return statement only valid in a procedure or a function.";
	error_message[89] = "Exit statement is only valid inside a loop.";
	error_message[90] = "Identifier must be a procedure name in this context.";
	error_message[91] = "Identifier illegal in this context.";
	error_message[92] = "String or expression expected.";
	error_message[93] = "LHS and RHS of assignment are not type compatible.";
	error_message[94] = "Parameter mode expected.";
	error_message[95] = "Parameter list terminated abnormally.";
	error_message[96] = "Type name integer, real, string or boolean expected.";
	error_message[97] = "Number of actual and formal parameters does not match.";
	error_message[98] = "Actual and formal parameter types do not match.";
	error_message[99] = "Actual and formal parameter kinds do not match.";
	error_message[100] = "Too many actual parameters.";
	error_message[101] = "Compound statement expected.";
	error_message[102] = "Expression must be of type integer.";
	error_message[103] = "Boolean expression expected.";
	error_message[104] = "Ranges of integers only are permitted.";
	error_message[105] = "Relational operator expected.";
	error_message[106] = "Variable, procedure or function declaration expected.";
	error_message[107] = "Identifier must match name of the block.";
	error_message[108] = "Type expected.";
	error_message[109] = "Functions must have at least 1 return statement.";
	error_message[110] = "Expected value for constant declaration.";
	error_message[111] = "Constant expression does not match type declaration.";
	error_message[112] = "Literal expected.";
	error_message[113] = "Illegal symbol follows expression.";
	error_message[114] = "Types of expressions must match.";
	error_message[115] = "Both expressions must be strings.";
	error_message[116] = "Arithmetic expression expected.";
	error_message[117] = "Boolean expression expected.";
	error_message[118] = "Integer or real expression expected.";
	error_message[119] = "Integer expression expected.";
	error_message[120] = "Boolean expression expected.";
	error_message[121] = "Function call expected.";
	error_message[122] = "Formal and actual parameter types do not match.";
	error_message[123] = "Functions can only have value parameters.";
	error_message[124] = "Primary epxected.";
}

void error_handler::add_error_to_list(int line, int pos, int err)
// Add error details to list so it can be added to listing file later.
{
	error_list *next_err = new error_list;
	next_err->line_no = line;
	next_err->pos_no = pos;
	next_err->err_no = err;
	next_err->next = NULL;

	error_list *temp1;
	error_list *temp2;

	if (err_list == NULL)
		err_list = next_err;
	else
	{
		// check if new error is to become the head of the error_list
		if ((err_list->line_no >= next_err->line_no) and (err_list->pos_no > next_err->pos_no))
		{
			// insert at head of list
			next_err->next = err_list;
			err_list = next_err;
		}
		else
		{
			// insert the next_err into its correct place in the list
			temp1 = err_list;
			temp2 = temp1->next;
			while ((temp2 != NULL) and (temp2->line_no < next_err->line_no))
			{
				temp1 = temp1->next;
				temp2 = temp2->next;
			}
			// either at the end of the list, or there are multiple errors on this line
			while ((temp2 != NULL) and (temp2->pos_no <= next_err->pos_no))
			{
				temp1 = temp1->next;
				temp2 = temp2->next;
			}
			// now at the insertion point
			next_err->next = temp2;
			temp1->next = next_err;
		}
	}
}

void error_handler::flag(int line_number, int pos_on_line, int error_no)
// Error detected by scanner at specified position.
{
	error_num++;
	if (error_num <= error_limit)
	{
		cerr << "ERROR: " << error_message[error_no] << " Error at (" << line_number << ", " << pos_on_line << ")." << endl;
		add_error_to_list(line_number, pos_on_line, error_no);
	}
}

void error_handler::flag(token *tok, int error_no)
// Error detected at token tok.
{
	// Generate an error message and retain the token and message in an appropriate data structure
	// so that a listing file can be generated at the completion of the compilation.
	error_num++;
	if (error_num <= error_limit)
	{
		cerr << "*** ERROR: " << error_message[error_no] << " Error #" << error_no << " at (" << tok->get_line_number() << ", " << tok->get_pos_on_line() << ")." << endl;
		add_error_to_list(tok->get_line_number(), tok->get_pos_on_line(), error_no);
	}
}

void error_handler::set_error_limit(int i)
{
	error_limit = i;
}

void error_handler::generate_listing()
// generate a listing file.
{
	error_list *error_this_line;
	int line_number{1};
	int err_count = 0;
	string source_line;
	const int no_width = 4;
	const int space = 1;
	bool enter_loop{false};

	if (listing_required)
	{
		// GENERATE THE LISTING FILE.

		// INSERT CODE HERE.
	}
	// else do nothing since no listing file name was provided.
}

int error_handler::error_count()
// Return number of errors found to date
{
	return error_num;
}
