/*
    File: id_table_entry.cpp
    Conor Rybacki
    11.29.2021
    The purpose of this data type is to hold information regarding an individual id table entry
*/

#include <iostream>
#include <string>

#include "id_table_entry.h"
#include "lille_type.h"
#include "lille_kind.h"
#include "token.h"
#include "error_handler.h"

using namespace std;

id_table_entry::id_table_entry() // default constructor should set everything to NULL
{
    id_token = NULL;
    kind_entry = lille_kind::unknown;      // the kind of the entry variable, (constant, value_param, ref_param, for_ident, unknown)
    type_entry = lille_type::type_unknown; // the type of the entrty
    i_val_entry = NULL;                    // if it is an integer store its value here
    r_val_entry = NULL;                    // only storing info where appropriate, maintain NULL in all the others
    s_val_entry = "";
    b_val_entry = NULL;
    p_list_entry = NULL; // points to the head paramenter
    n_par_entry = 0;     // the number of parameters
    r_ty_entry = lille_type::type_unknown;
}

id_table_entry::id_table_entry(token *id,
                               lille_type typ = lille_type::type_unknown,
                               lille_kind kind = lille_kind::unknown,
                               lille_type return_tipe = lille_type::type_unknown) // parameterized constructor with appropriate defaults
{
    id_token = id;
    kind_entry = kind;
    type_entry = typ;
    i_val_entry = NULL; // if it is an integer store its value here
    r_val_entry = NULL; // only storing info where appropriate, maintain NULL in all the others, only for CONSTANTS
    s_val_entry = "";
    b_val_entry = NULL;
    p_list_entry = NULL; // points to the head paramenter
    n_par_entry = 0;     // the number of parameters
    r_ty_entry = return_tipe;
}

lille_kind id_table_entry::kind() // retrun specidied -- continue down
{
    return kind_entry;
}

lille_type id_table_entry::tipe()
{
    return type_entry;
}

token *id_table_entry::token_value()
{
    return id_entry;
}

int id_table_entry::integer_value()
{
    return i_val_entry;
}

float id_table_entry::real_value()
{
    return r_val_entry;
}

bool id_table_entry::bool_value()
{
    return b_val_entry;
}

lille_type id_table_entry::return_tipe()
{
    return r_ty_entry;
}

void id_table_entry::add_param(id_table_entry *param_entry) // how do we handle function or procedure parameters?
{
}

void id_table_entry::fix_return_type(lille_type new_ret) // with functions we dont know their type
{
    r_ty_entry = new_ret;
}