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
    i_val_entry = 0;                       // if it is an integer store its value here
    r_val_entry = 0.0;                     // only storing info where appropriate, maintain NULL in all the others
    s_val_entry = "";
    b_val_entry = false;
    p_list_entry = NULL; // points to the head paramenter
    n_par_entry = 0;     // the number of parameters
    r_ty_entry = lille_type::type_unknown;
}

id_table_entry::id_table_entry(token *id,
                               lille_type typ,
                               lille_kind kind,
                               lille_type return_tipe) // parameterized constructor with appropriate defaults
{
    id_token = id;
    kind_entry = kind;
    type_entry = typ;
    i_val_entry = 0;   // if it is an integer store its value here
    r_val_entry = 0.0; // only storing info where appropriate, maintain NULL in all the others, only for CONSTANTS
    s_val_entry = "";
    b_val_entry = false;
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
    return id_token;
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

int id_table_entry::number_of_params()
{
    return n_par_entry;
}

// assume that we are indexing the parameters starting with 1
id_table_entry *id_table_entry::nth_parameter(int n){
    id_table_entry *temp1;
    if(n <= n_par_entry) // make sure that the parameter that we are searching for will exist
    {
        temp1 = this->p_list_entry; // get the current entry parameter info (place: 1)
        // loop until the n parameter
        for(int x=2; x<=n; x++) // if n > 1 then keep going until n is satisfied
        {
            temp1 = temp1->p_list_entry; // get the entry that is pointed to
        }

        return temp1;
    }
    else{
        throw lille_exception("Compiler error - Trying to access parameters that don't exist");
        return NULL;
    }
}

void id_table_entry::fix_const(int integer_value,
                               float real_value,
                               string string_value,
                               bool bool_value)
{
    // make sure that the kind entry is set to constant
    // kind_entry = lille_kind::constant;

    if (type_entry.is_type(lille_type::type_integer))
    {
        i_val_entry = integer_value;
    }
    else if (type_entry.is_type(lille_type::type_real))
    {
        r_val_entry = real_value;
    }
    else if (type_entry.is_type(lille_type::type_string))
    {
        s_val_entry = string_value;
    }
    else if (type_entry.is_type(lille_type::type_boolean))
    {
        b_val_entry = bool_value;
    }
}

void id_table_entry::add_param(id_table_entry *param_entry) // how do we handle function or procedure parameters?
{
    if (type_entry.is_type(lille_type::type_proc) || type_entry.is_type(lille_type::type_func))
    {
        id_table_entry *p = this->p_list_entry; // get pointer to the current parameter list
        id_table_entry *q;                      // needed to look through parameter list

        if (p == NULL) // if there are no parameters than just point to the parameter entry
        {
            this->p_list_entry = param_entry;
        }
        else // if theres a parameter in the spot, get it, then check its parameter spot
        {
            while (p != NULL)
            {                        // check all things in parameter list until there is a spot
                q = p;               // swap the pointer to an entry
                p = p->p_list_entry; // get the param list ent of next entry (if it exists)
            }
            // if the past node points to a null then enter the
            q->p_list_entry = param_entry; // pointer to the next parameter in its param spot
        }
        n_par_entry++; // increment the count of parameters
    }
    else
    {
        throw lille_exception("Internal compiler error - trying to add parameters to something other than a procedure or function.");
    }
}

// returns the value of the entry (based on its type) as a string
string id_table_entry::to_string()
{
    if (type_entry.is_type(lille_type::type_integer)) // int
    {
        return std::to_string(i_val_entry);
    }
    else if (type_entry.is_type(lille_type::type_real)) // real
    {
        return std::to_string(r_val_entry);
    }
    else if (type_entry.is_type(lille_type::type_string)) // string
    {
        return s_val_entry;
    }
    else if (type_entry.is_type(lille_type::type_boolean)) // if its a bool check val then return
    {
        if (b_val_entry)
        {
            return "True";
        }
        else
        {
            return "False";
        }
    }
    return "unknown";
}

// fix up to fix up the types (and vals if constants) of a list of identifiers
void id_table_entry::fix_type(lille_type ident_type)
{
    type_entry = ident_type;
}

// fix up the kind of the table entry
void id_table_entry::fix_kind(lille_kind ident_kind)
{
    kind_entry = ident_kind;
}

void id_table_entry::fix_return_type(lille_type new_ret) // with functions we dont know their type
{
    r_ty_entry = new_ret;
}