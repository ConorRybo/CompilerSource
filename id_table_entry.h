/*
    File: id_table_entry.h
    Conor Rybacki
    11.29.2021
    The purpose of this data type is to hold information regarding an individual id table entry
*/

#ifndef ID_TABLE_ENTRY_H_
#define ID_TABLE_ENTRY_H_

#include <iostream>
#include <string>

#include "lille_type.h"
#include "lille_kind.h"
#include "token.h"
#include "error_handler.h"

using namespace std;

class id_table_entry
{
private:
    token *id_token;
    lille_kind kind_entry; // the kind of the entry variable, (constant, value_param, ref_param, for_ident, unknown)
    lille_type type_entry; // the type of the entrty
    int i_val_entry;       // if it is an integer store its value here
    float r_val_entry;     // only storing info where appropriate, maintain NULL in all the others
    string s_val_entry;
    bool b_val_entry;
    id_table_entry *p_list_entry; // points to the head paramenter
    int n_par_entry;              // the number of parameters
    lille_type r_ty_entry;        // no idea

public:
    id_table_entry();
    id_table_entry(token *id,
                   lille_type typ = lille_type::type_unknown,
                   lille_kind kind = lille_kind::unknown,
                   lille_type return_tipe = lille_type::type_unknown);
    lille_kind kind();    // returns the kind of the desired entry
    lille_type tipe();    // returns the type of the desired entry
    token *token_value(); // returns the current token
    int integer_value();
    float real_value();
    bool bool_value();
    string string_value();
    lille_type return_tipe();
    void fix_const(int integer_value = 0,
                   float real_value = 0,
                   string string_value = "",
                   bool bool_value = false); // fixes the value of a constant after entry
    void add_param(id_table_entry *param_entry);
    void fix_return_type(lille_type new_ret);
    int number_of_params();
    // if there is an identifier list then we
    // must fix all the types of the ones after we enter them
    void fix_type(lille_type ident_type);
    string to_string();
};
#endif