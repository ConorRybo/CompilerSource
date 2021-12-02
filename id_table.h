/*
 *  idtable.h
 *
 *  Created on: Jun 18, 2020
 *      Author: Michael Oudshoorn
 */

#ifndef ID_TABLE_H_
#define ID_TABLE_H_

#include <iostream>
#include <string>
#include <vector>

#include "token.h"
#include "error_handler.h"
#include "id_table_entry.h"

using namespace std;

class id_table
{
private:
    struct node
    {
        node *left;                 // left node
        node *right;                // right node
        id_table_entry *entry_info; // holds all the information about table entry
    };

    error_handler *error;
    vector<node *> scope_man; // where we will manage the scope levels
    int scope_lvl;            // holds the current scope level

    node *search_tree(string s, node *p);                     // method to search a tree in level order
    node *insert(id_table_entry *idt_entry, node *p);         // insert in tree starting with node p
    void add_table_entry(id_table_entry *idt_entry, node *p); // method to add an entry to the tree
    void dump_tree(node *ptr);                                // method to dump all the contents of the tree? null out all the nodes??

public:
    id_table(error_handler *err);
    void enter_new_scope();                          // called when we are entering a new region of scope
    void exit_scope();                               // when we are exiting a level of scope
                                                     // what must be done on the exit?
    int scope();                                     // return the level of scope
    id_table_entry *lookup(string s);                // our search method to find out if the
    id_table_entry *lookup(token *tok);              // our search method to find out if the
                                                     // identifier is located in the tree
    void add_table_entry(id_table_entry *idt_entry); // add to the id table - create node then pass node to same function name
    id_table_entry *enter_id(token *id,
                             lille_type typ = lille_type::type_unknown,
                             lille_kind kind = lille_kind::unknown,
                             lille_type return_tipe =
                                 lille_type::type_unknown); // this is the main adding function that will return a pointer to the node
                                                            // as well as set and enter the id into the table
    void dump_id_table(bool dump_all = true);
};

#endif /* ID_TABLE_H_ */
