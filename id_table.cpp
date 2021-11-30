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
	root = NULL;
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

// INSERT IS ONLY CALLED WHEN WE KNOW WE NEED TO INSERT
// INITIAL SEARCH IS DONE IN THE LOOK_UP to varify if its there
// maybe have this return the inserted ident so we can keep track
node *id_table::insert_ident(node *node, token *id_token) // if we need to fixup vals in the future?
{
	// this is the logic for the insertion of a node into a binary search tree
	if (node == NULL) // if its empty create node and put it there
		return new_node(id_token);

	if (id_token->get_identifier_value() < node->ident_key)
	{
		node->left = insert_ident(node->left, id_token);
	}
	else if (id_token->get_identifier_value() > node->ident_key)
	{
		node->right = insert_ident(node->right, id_token);
	}

	return node;
}

node *id_table::new_node(token *id_token)
{
	id_table_entry *idt_new;				// new entry into table so get entry ready
	idt_new = new id_table_entry(id_token); // pass id token to entry to initialize
	node *temp = new node;					// create a temp node
	temp->ident_key = id_token->get_identifier_value();
	temp->entry_info = idt_new;
	temp->left = temp->right = NULL;
	return temp;
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
