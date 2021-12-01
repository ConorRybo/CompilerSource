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
	scope_man[scope_lvl] = NULL; // just to be sure set the root pointer to null
	scope_man.pop_back();		 // remove the last element (leave that scope)
	scope_lvl--;				 // keep track of the scope
}

// search tree algoritbm for the node
id_table::node *id_table::search_tree(string s, node *p)
{
	// if there isnt a matching entry then return null
	if (p == NULL)
	{
		return NULL;
	}
	// check for a match
	if (p->entry_info->token_value()->get_identifier_value() == s)
	{
		return p;
	}

	// if the desired is less than current node then search left
	if (p->entry_info->token_value()->get_identifier_value() > s)
	{
		search_tree(s, p->left);
	}
	else // look to the right node
	{
		search_tree(s, p->right);
	}
}

id_table::node *id_table::insert(id_table_entry *idt_entry, node *p)
{
	// the base case: if the root is null then insert at that node
	if (p == NULL)
	{
		add_table_entry(idt_entry, p); // add the actual entry to the node
		return p;					   // return a pointer to the new node
	}

	// compare current key to node keys
	if (idt_entry->token_value()->get_identifier_value() > p->entry_info->token_value()->get_identifier_value())
	{
		// if the value is greater then we go to the right node
		p->right = insert(idt_entry, p->right);
	}
	else
	{
		// if it is less then we go to the left side
		p->left = insert(idt_entry, p->left);
	}

	// after an insertion return the root
	return p;
}

// first of the overloaded two
void id_table::add_table_entry(id_table_entry *idt_entry, node *p)
{
	// my interpretation is that p is a pointer to a node that
	// the new leaf is being added

	// create a new leaf with the entry
	node *leaf;
	leaf = new node;
	leaf->left = NULL;
	leaf->right = NULL;
	leaf->entry_info = idt_entry;

	// point to the new leaf now
	p = leaf;
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
