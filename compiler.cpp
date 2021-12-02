/*************************************************************************************************
 *
 * lille Compiler
 *
 * Author:     Michael Oudshoorn
 *             Webb School of Engineering
 *             High Point University
 *             High Point
 *             NC   27265
 *             USA
 *
 * Written:            1 June 2020
 * Last modified:      1 June 2020
 *
 * Open Source - free to distribute and modify. May not be used for profit.
 *
 * Written using C++20.
 *         g++ -std=c++2a -o lille compiler.ccp
 *
 *
 * Usage
 *        lille [flags] filename
 * where filename contains the source code to be compiler.
 *
 * Flags are:
 *		-l 				Generate a listing file
 *		-o filename  	Generate code file with the specified name
 *		-h	Help		Generate help instructions
 *
 **************************************************************************************************/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <map>
#include <iterator>

#include "lille_exception.h"
#include "scanner.h"
#include "parser.h"
#include "symbol.h"
#include "error_handler.h"
#include "id_table.h"
#include "id_table_entry.h"
//#include "code_gen.h"
//#include "id_table.h"

using namespace std;
using namespace std::chrono;

const string default_listing_file_name = "LISTING"; // Default listing file name
const string default_source_file_name = "SOURCE";	// Default source code file name
const string default_code_filename = "CODE";		// Default code file name if one not specified on command line

bool listing_required{false}; // Should a listing file be generated?

string source_filename;	 // Name of the source file containing DO code to be compiled.
string code_filename;	 // Name of the PAL output file to be generated.
string listing_filename; // name of the listing file to be generated if needed.

error_handler *err; // error handler object
scanner *scan;		// scanner object
parser *parse;		// parser object
id_table *id_tab;
// code_gen* code;									// code generator

bool process_command_line(int argc, char *argv[])
{
	// Process the command line and identify flags that are set and any filenames provided.
	// Usage
	//        do [flags] filename
	// where filename contains the source code to be compiler.
	//
	// Flags are:
	//		-l 				Generate a listing file
	//		-o filename  	Generate code file with the specified name
	//		-h				Generate help instructions

	bool hflag = false;	  // help flag set
	bool sflag = false;	  // source filename provided
	bool cflag = false;	  // code file specified
	string root_filename; // Root of the filename for the source and listing files.

	listing_required = false;
	code_filename = default_code_filename; // May be updated by a flag
	if (argc < 2)
	{
		// Insufficient arguments. Display helpful message and terminate the program
		cerr << "Usage: " << argv[0] << " filename" << endl;
		return false;
	}
	else
	{
		// There are several flags that need to be processed.
		for (int i = 1; i < argc; i++)
		{
			string arg = argv[i];
			if (arg == "-h")
			{
				// Help flag. Respond to display help information. Only do this once, even if flag is set multiple times
				if (!hflag)
				{
					hflag = true; // Note that help has already been given. Once present it once
					cout << "Usage: " << argv[0] << " [flags] filename" << endl;
					cout << "    where filename is the name of the file to be compiled into PAL code." << endl;
					cout << "    Valid flags are:" << endl;
					cout << "        -h              Print out this help message." << endl;
					cout << "        -l              Create a listing file showing source code and all errors" << endl;
					cout << "                        detected during the compilation process. Line numbers are" << endl;
					cout << "                        provided." << endl;
					cout << "        -o filename     The generated code file (PAL code) is named filename." << endl;
					cout << "                        If this flag is not present, then the default name of" << endl;
					cout << "                        of the code file is " << default_code_filename << endl;
				}
			}
			else if (arg == "-l")
			{
				// Generate a listing
				listing_required = true; // Set global flag to show that a listing is required.
										 // Name of listing file is based on the name of the source file. It is set up after the command line is processed.
			}
			else if (arg == "-o")
			{
				// Generate a named output file holding the PAL code.
				if (i + 1 < argc) // A file name is expected after the -o flag
				{
					code_filename = argv[i++]; // Increment i so we do not get the argument on the next loop iteration.
					cflag = true;
				}
				else // No file name provided
				{
					cerr << "Output filename expected." << endl;
					return false;
				}
			}
			else
			{
				// no flag, so this must be the name of the source file.
				// First check to make sure it isn;t an invalid flag
				if (arg.at(0) == '-')
				{
					cerr << "Illegal flag: " << arg << endl;
					return false;
				}
				else
				{
					sflag = true;
					source_filename = arg;
				}
			}
		}
		// Now we determine the name of the listing file and code file.
		// First check to make sure that a source filename was provided.
		if (!sflag)
		{
			// a source file name is required, so generate an error message and abort
			cout << "Source filename not provided. Using default files for Source code, listing and code." << endl;
			source_filename = default_source_file_name;
			listing_filename = default_listing_file_name;
			if (!cflag)
				code_filename = default_code_filename;
		}
		else
		{
			root_filename = source_filename.substr(0, source_filename.find(".")); // The root filename is up to the first "." character
																				  // or the whole name if not present.
			listing_filename = root_filename + ".lis";							  // Append ".lis" to the end of the root filename.
			if (!cflag)
				code_filename = root_filename + ".pal"; // Append ".pal" tp the end of the root filename.
		}

		return true;
	}
}

int main(int argc, char *argv[])
{
	// local variables used to measure and report elapsed time
	high_resolution_clock::time_point start; // start time
	high_resolution_clock::time_point stop;	 // stop time
	milliseconds time_span;					 // elapsed time

	bool status; // status after checking command line.

	// Start the timer
	start = high_resolution_clock::now();

	// Process any command line flags etc
	status = process_command_line(argc, argv);

	if (status)
	{
		try
		{
			if (!listing_required)
				err = new error_handler(source_filename);
			else
				err = new error_handler(source_filename, listing_filename);

			// THE FOLLOWIG CODE IS FOR TESTING PURPOSES ONLY.
			id_tab = new id_table(err);
			scan = new scanner(source_filename, id_tab, err);
			parse = new parser(scan, id_tab, err);

			// token *tok;
			// do
			// {
			// 	tok = scan->get_token();
			// } while (tok->get_sym() != symbol::end_of_program);
			// END OF CODE FOR TESTING PURPOSES

			// Create a symbol_table object

			// create a scanner object
			// scan = new scanner(source_filename, id_tab, err);

			// create the code generator

			// create a parser object
			// Compile the source code

			// Generate the PAL code file, if no errors were detected.

			// Generate a listing, if required.
			if (listing_required)
				err->generate_listing();

			stop = high_resolution_clock::now();
			time_span = duration_cast<milliseconds>(stop - start);

			cout << "Execution completed in " << time_span.count() << " milliseconds with " << err->error_count() << " errors found." << endl;
		}
		catch (lille_exception &e)
		{
			cerr << "Exception: " << e.what() << endl;
			return 1;
		}
		catch (exception &e)
		{
			cerr << "Exception: " << e.what() << endl;
			return 1;
		}
	}
	return 0;
}
