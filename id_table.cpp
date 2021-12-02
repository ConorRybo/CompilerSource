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
	return NULL;
}

id_table::node *id_table::insert(id_table_entry *idt_entry, node *p)
{
	// the exit case: if the root is null then insert at that node
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
	// this is how we "back out" of the recurison
	return p;
}

// first of the overloaded two
void id_table::add_table_entry(id_table_entry *idt_entry, node *p)
{
	// my interpretation is that p is a pointer to a node that
	// the new leaf is being added at

	// create a new leaf with the entry
	node *leaf;
	leaf = new node;
	leaf->left = NULL;
	leaf->right = NULL;
	leaf->entry_info = idt_entry;

	// point to the new leaf now
	p = leaf;
}

// perform a lookup of the datastructer with just a string (identifier => key)
id_table_entry *id_table::lookup(string s)
{
	// we want to make sure that we start at the current scope level and then work backwards
	int lscop = scope_lvl; // keep a local v so we can decrement
	// create a result node and run the search on the current level of scope we are at
	node *result = search_tree(s, scope_man[lscop]);
	lscop--;
	while (result == NULL && lscop >= 0) // if and while there is nothing found
	{
		result = search_tree(s, scope_man[lscop]);
	}

	if (result == NULL)
	{				 // if it gets to the end and result is still NULL
		return NULL; // return the null value
	}
	else
	{
		return result->entry_info; // return the entry of the node
	}
}

// lookup function if its passed a token
id_table_entry *id_table::lookup(token *tok)
{
	// get the identifier value (string, key to search)
	string key = tok->get_identifier_value();
	return lookup(key); // just call the version that takes the string arg
}

// method to dump only the contents of a single tree
void id_table::dump_tree(node *ptr)
{
	// prt is a pointer to the head node of the tree
	if (ptr != NULL)
	{
		dump_tree(ptr->left); // always start with left bc that is the least
		cout << "ident name: " << ptr->entry_info->token_value()->get_identifier_value() << endl;
		cout << "value: " << ptr->entry_info->to_string() << endl;
		dump_tree(ptr->right);
	}
}

void id_table::dump_id_table(bool dump_all)
{
	if (!dump_all)
	{
		cout << "Dump of idtable for current scope only." << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "current scope level: " << scope_lvl << endl;
		dump_tree(scope_man[scope_lvl]); // just dump at the current scope level
	}
	else
	{
		cout << "Dump of the entire symbol table." << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		int scolev = scope_lvl;
		while (scolev >= 0) // dump all the scope levels
		{
			cout << endl;
			cout << "current scope level: " << scolev << endl;
			dump_tree(scope_man[scolev]); // dump the tree at the current level
			scolev--;					  // move up a level and then print it
		}
	}
}
