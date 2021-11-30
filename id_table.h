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

struct node
{
    string ident_key;           // the key for the search is the identifier value
    node *left;                 // left node
    node *right;                // right node
    id_table_entry *entry_info; // holds all the information about table entry
};

class id_table
{
private:
    error_handler *error;
    node *root;               // this is the starting node for the tree
    vector<node *> scope_man; // where we will manage the scope levels
    int scope_lvl;

public:
    id_table(error_handler *err);
    node *new_node(token *id_token);                 // creates a new node to be added to the tree
    node *insert_ident(node *node, token *id_token); // insert a node into the tree
    void enter_new_scope();                          // called when we are entering a new region of scope
    void exit_scope();                               // when we are exiting a level of scope
                                                     // what must be done on the exit?
    void lookup();                                   // our search method to find out if the
                                                     // identifier is located in the tree
    void dump_id_table(bool dump_all = true);
};

#endif /* ID_TABLE_H_ */
