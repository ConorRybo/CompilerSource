/*
 * idtable.cpp
 *
 *  Modified on: November 29, 2020
 *      Author: Conor Rybacki
 */

#include <iostream>
#include <string>

#include "token.h"
#include "error_handler.h"
#include "id_table.h"
#include "id_table_entry.h"

using namespace std;

/*
	NEEDS:
		- Private variable to refer to an entry in the id table
		- private struct id_entry
			... what information should it contain?
		- enter_new_scope() : used when we are entering a new level of scope...
		... what does this need to do?
		- exit_scope() : exiting (moving up?) a scope level...
		... how do we manage this?
		- lookup() : used to look up if an identifier exists in the table
		... if it doesn't - insert() : insert the identifier into the table
		- fix_up() : if there are multiple variables being declared at the same time
			we must then go back and set their types after they are all recognized
*/

id_table::id_table(error_handler *err)
{
	// initialize to defaults or passed vals
	error = err;
	scope_man.clear();
	scope_lvl = 0;
}

void id_table::enter_new_scope()
{
	// what needs to be adjusted when the scope is incremented
	scope_man.push_back(NULL); // push a new slot onto the array filled by a null ptr
	scope_lvl++;			   // keep track of scope level
}

void id_table::exit_scope()
{
	// when exiting what really only needs to be done?
	scope_man.pop_back(); // remove the last element (leave that scope)
	scope_lvl--;		  // keep track of the scope
}

void id_table::dump_id_table(bool dump_all)
{
	if (!dump_all)
	{
		cout << "Dump of idtable for current scope only." << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

		// INSERT CODE HERE
	}
	else
	{
		cout << "Dump of the entire symbol table." << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

		// INSERT CODE HERE
	}
}
