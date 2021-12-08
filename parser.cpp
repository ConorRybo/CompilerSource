/*
    parser.cpp
    Conor Rybacki
*/

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "parser.h"
#include "symbol.h"
#include "token.h"
#include "error_handler.h"
#include "id_table.h"
#include "id_table_entry.h"
#include "scanner.h"
#include "lille_kind.h"
#include "lille_type.h"

using namespace std;

parser::parser()
{ // default constructor for the parser
    // set all the private pointers to NULL
    scan = NULL;
    error = NULL;
    simple = false;
    ident_name = "";
    currentHold = new id_table_entry();
}

parser::parser(scanner *s, id_table *idt, error_handler *e)
{ // parameterized constructor
    // set corresponding private variables to passed vals
    scan = s;
    error = e;
    idTable = idt;
    simple = false;
    ident_name = "";
    currentHold = new id_table_entry();
    PROG();
}

void parser::PROG()
{
    // // setup the predefined tokens before running the first scan
    token *predefined_func; //
    token *argument;
    symbol *predefined_sym;
    id_table_entry *func_id;
    id_table_entry *param_id;

    // create int2real predefined function
    // setup the token to be passed create entry
    predefined_sym = new symbol(symbol::identifier);   // create an identifier symbol
    predefined_func = new token(predefined_sym, 0, 0); // create a token based on  that identifier
    predefined_func->set_identifier_value("INT2REAL"); // set the identifier value
    func_id = idTable->enter_id(predefined_func, lille_type::type_func,
                                lille_kind::unknown, lille_type::type_real); // create id table entry and store in datastructure
    // Create a token for the parameter of the predefined function
    argument = new token(predefined_sym, 0, 0);         // create a new arguement for the predifined function
    argument->set_identifier_value("__int2real_arg__"); // setting token ident value
    param_id = new id_table_entry(argument,
                                  lille_type::type_integer,
                                  lille_kind::value_param,
                                  lille_type::type_unknown); // create a new table entry (but dont put into the tree)
    // Associate parameter with the function.
    func_id->add_param(param_id); // add the parameter to the associated function

    // repeat above with real2int
    predefined_sym = new symbol(symbol::identifier);   // create an identifier symbol
    predefined_func = new token(predefined_sym, 0, 0); // create a token based on  that identifier
    predefined_func->set_identifier_value("REAL2INT"); // set the identifier value
    func_id = idTable->enter_id(predefined_func, lille_type::type_func,
                                lille_kind::unknown, lille_type::type_integer); // create id table entry and store in datastructure
    // Create a token for the parameter of the predefined function
    argument = new token(predefined_sym, 0, 0);         // create a new arguement for the predifined function
    argument->set_identifier_value("__real2int_arg__"); // setting token ident value
    param_id = new id_table_entry(argument,
                                  lille_type::type_real,
                                  lille_kind::value_param,
                                  lille_type::type_unknown); // create a new table entry (but dont put into the tree)
    // Associate parameter with the function.
    func_id->add_param(param_id); // add the parameter to the associated function

    // repeat above with int2string
    predefined_sym = new symbol(symbol::identifier);     // create an identifier symbol
    predefined_func = new token(predefined_sym, 0, 0);   // create a token based on  that identifier
    predefined_func->set_identifier_value("INT2STRING"); // set the identifier value
    func_id = idTable->enter_id(predefined_func, lille_type::type_func,
                                lille_kind::unknown, lille_type::type_string); // create id table entry and store in datastructure
    // Create a token for the parameter of the predefined function
    argument = new token(predefined_sym, 0, 0);           // create a new arguement for the predifined function
    argument->set_identifier_value("__int2string_arg__"); // setting token ident value
    param_id = new id_table_entry(argument,
                                  lille_type::type_integer,
                                  lille_kind::value_param,
                                  lille_type::type_unknown); // create a new table entry (but dont put into the tree)
    // Associate parameter with the function.
    func_id->add_param(param_id); // add the parameter to the associated function

    // one final time with real2string
    predefined_sym = new symbol(symbol::identifier);      // create an identifier symbol
    predefined_func = new token(predefined_sym, 0, 0);    // create a token based on  that identifier
    predefined_func->set_identifier_value("REAL2STRING"); // set the identifier value
    func_id = idTable->enter_id(predefined_func, lille_type::type_func,
                                lille_kind::unknown, lille_type::type_string); // create id table entry and store in datastructure
    // Create a token for the parameter of the predefined function
    argument = new token(predefined_sym, 0, 0);            // create a new arguement for the predifined function
    argument->set_identifier_value("__real2string_arg__"); // setting token ident value
    param_id = new id_table_entry(argument,
                                  lille_type::type_real,
                                  lille_kind::value_param,
                                  lille_type::type_unknown); // create a new table entry (but dont put into the tree)
    // Associate parameter with the function.
    func_id->add_param(param_id); // add the parameter to the associated function

    // lets get logic started
    scan->get_token(); // get the first token from the scanner
    if (debugging)     // signaling
        cout << "Entering Prog" << endl;
    scan->must_be(symbol::program_sym); // checking the first scanned symbol
    if (scan->have(symbol::identifier)) // storing the identifier
    {
        ident_name = scan->this_token()->get_identifier_value(); // storing to check after end statement
        // enter the identifier into the table
        idTable->enter_id(scan->this_token(), lille_type::type_prog);
    }
    scan->must_be(symbol::identifier); // if it isn't an identifier throw err
    idTable->enter_new_scope();        // now we are entering the program so we change scope
    // idTable->dump_id_table(true);
    scan->must_be(symbol::is_sym); // looking for the is symbol
    BLOCK(ident_name);             // call block and pass the name of the proc
    scan->must_be(symbol::semicolon_sym);
    // idTable->dump_id_table(true);
    idTable->exit_scope();                 // exit to scope level 0
    scan->must_be(symbol::end_of_program); // catches any trash after program
}

void parser::BLOCK(string bName)
{
    if (debugging)
    {
        cout << "entering block " << bName << endl;
        // idTable->dump_id_table(true);
    }

    while (scan->have(symbol::identifier) || scan->have(symbol::procedure_sym) || scan->have(symbol::function_sym)) // if the current symbol is an identifier
    {
        DECLARATION();
    } // call declaration to check the semantics
    // can have multiple lines of identifiers and assignment statements

    scan->must_be(symbol::begin_sym); // if there isn't any identifiers then must be begin
    STATEMENT_LIST();                 // call the statement list function
    scan->must_be(symbol::end_sym);
    // idTable->dump_id_table(true);
    idTable->exit_scope();
    if (scan->have(symbol::identifier))
    {
        if (scan->this_token()->get_identifier_value() != bName)
        {
            error->flag(scan->this_token(), 107);
        }
        scan->get_token(); // should be at the end of the file
    }
}

void parser::DECLARATION()
{
    bool constant = false; // if identifiers are constants -> true
    bool prm = true;
    string bname = "";
    bool fun, proc = false;
    // create a list to hold multiple identifiers (jic)
    vector<id_table_entry *> id_list;
    vector<id_table_entry *> params;
    id_table_entry *profunHolder;
    lille_type type_fix; // need to fix variables with appropriate type
    lille_type varType;  // if its a constant we need to make sure dec val matches ident type
    lille_kind kindFix;  // to fix the types after a param list
    if (debugging)
        cout << "entering declaration" << endl;
    // if it is an identifier list
    if (scan->have(symbol::identifier))
    {
        // enter the token into the id table and push the resulting entry
        // into the list
        id_list.push_back(idTable->enter_id(scan->this_token(), lille_type::type_unknown, lille_kind::variable));

        if (debugging)
        {
            cout << "starting declaration ident logic" << endl;
        }
        // enters this function ON the identifier token
        scan->get_token();                    // get the next token
        while (scan->have(symbol::comma_sym)) // if theres a comma (identifier list)
        {
            scan->get_token();                  // advance on to next token
            if (scan->have(symbol::identifier)) // if it is another identifier
            // then enter it into the table and store the table entry in list
            {
                id_list.push_back(idTable->enter_id(scan->this_token(), lille_type::type_unknown, lille_kind::variable));
            }
            scan->must_be(symbol::identifier); // clarify that it's an identifier
        }
        // after ident list we are looking for a colon
        scan->must_be(symbol::colon_sym); // ident or ident list must be followed by colon
        if (scan->have(symbol::constant_sym))
        { // if theres a constant symbol
            constant = true;
            scan->get_token();
        }
        // needs to be a type declaration next
        if (scan->have(symbol::integer_sym) || scan->have(symbol::real_sym) || scan->have(symbol::string_sym) || scan->have(symbol::boolean_sym))
        {
            // we need to fix up the type of the identifier(s)
            if (scan->have(symbol::integer_sym))
            {
                type_fix = lille_type::type_integer;
            }
            else if (scan->have(symbol::real_sym))
            {
                type_fix = lille_type::type_real;
            }
            else if (scan->have(symbol::string_sym))
            {
                type_fix = lille_type::type_string;
            }
            else
            {
                type_fix = lille_type::type_boolean;
            }
            // fix the types of the variables
            for (int i = 0; i < id_list.size(); i++)
            {
                id_list[i]->fix_type(type_fix);
            }
            scan->get_token();
        }
        else // if there is no type declaration throw an error
        {
            error->flag(scan->get_token(), 96); // throw type name expected error
        }
        if (constant == true && scan->have(symbol::becomes_sym)) // if theres an expression we are allowed assignment
        {
            if (debugging)
            {
                cout << "constant detected" << endl;
            }
            scan->get_token(); // get off becomes symbol
            if ((scan->have(symbol::integer) || scan->have(symbol::real_num) || scan->have(symbol::strng) || scan->have(symbol::true_sym) || scan->have(symbol::false_sym)) != true)
            {
                // make sure that theres the correct data type after the
                error->flag(scan->this_token(), 84); // expecting number, string, or boolean
            }
            else
            {
                // if there is an assignment we must check the type to make sure that it matches the declaration type
                varType = id_list[0]->tipe();
                if (varType.is_type(lille_type::type_integer))
                {
                    if (!scan->have(symbol::integer))
                        error->flag(scan->this_token(), 3); // throw integer expected error
                    else
                    {
                        for (int i = 0; i < id_list.size(); i++)
                        {
                            id_list[i]->fix_const(scan->this_token()->get_integer_value());
                        }
                    }
                }
                else if (varType.is_type(lille_type::type_real))
                {
                    if (!scan->have(symbol::real_num) && !scan->have(symbol::integer))
                        error->flag(scan->this_token(), 86); // throw integer or real expected error
                    else
                    {
                        for (int i = 0; i < id_list.size(); i++)
                        {
                            id_list[i]->fix_const(0, scan->this_token()->get_real_value());
                        }
                    }
                }
                else if (varType.is_type(lille_type::type_string))
                {
                    if (!scan->have(symbol::strng))
                        error->flag(scan->this_token(), 1); // throw string expected error
                    else
                    {
                        for (int i = 0; i < id_list.size(); i++)
                        {
                            id_list[i]->fix_const(0, 0, scan->this_token()->get_string_value());
                        }
                    }
                }
                else
                {
                    if (scan->have(symbol::true_sym) || scan->have(symbol::false_sym))
                    {
                        bool sendV;
                        if (scan->have(symbol::true_sym))
                        {
                            sendV = true;
                        }
                        else
                        {
                            sendV = false;
                        }
                        for (int i = 0; i < id_list.size(); i++)
                        {
                            id_list[i]->fix_const(0, 0, "", sendV);
                        }
                    }
                    else
                    {
                        error->flag(scan->this_token(), 103); // throw bool expected error
                    }
                }
            }

            // fix up the identifiers with the value and type
            scan->get_token();
        }
        else if (constant == true)
        {
            // if the variables are constants and there is no becomes symbol
            // just run fix constant with nothing passed to it to set lille kind
            for (int i = 0; i < id_list.size(); i++)
            {
                id_list[i]->fix_const();
            }
        }
        else if (constant == false && scan->have(symbol::becomes_sym)) // if they try to assign value to non constant throw error
        {
            error->flag(scan->this_token(), 5);
        }
        // scan->must_be(symbol::semicolon_sym);
    }

    // if it is a procedure or function we have similar
    // logic so we can condense code
    else
    {
        if (scan->have(symbol::procedure_sym))
        {
            proc = true;
            if (debugging)
            {
                cout << "starting procedure logic" << endl;
            }
        }
        else
        {
            fun = true;
            if (debugging)
            {
                cout << "starting function logic" << endl;
            }
        }

        // we are currently on the proc or fun sym
        scan->get_token();
        if (scan->have(symbol::identifier))
        {
            // get the name of the identifier to check after the
            bname = scan->this_token()->get_identifier_value();
            if (proc == true) // enter the type into the id table based on if its a proc or func
            {
                type_fix = lille_type::type_proc;
            }
            else
            {
                type_fix = lille_type::type_func;
            }
            // enter the name of the proc/func into the id table
            profunHolder = idTable->enter_id(scan->this_token(), type_fix);
            idTable->enter_new_scope(); // after the procedure or function name we enter a new scope
        }
        scan->must_be(symbol::identifier);      // get rid of the identifier symbol
        if (scan->have(symbol::left_paren_sym)) // if theres a parameter list
        {
            while (prm)
            {
                params.clear();                     // clear at the beginning of each loop to make sure not building on last param list
                scan->get_token();                  // get the next token
                if (scan->have(symbol::identifier)) // if theres an identifier enter it into table
                {
                    params.push_back(idTable->enter_id(scan->this_token())); // push the first ident into the vector
                }
                scan->must_be(symbol::identifier); // check and throw out first ident
                while (scan->have(symbol::comma_sym))
                {                                       // if there is a list of identifiers
                    scan->get_token();                  // get rid of comma
                    if (scan->have(symbol::identifier)) // push the identifier
                    {
                        params.push_back(idTable->enter_id(scan->this_token())); // push the idents into the vector
                    }
                    scan->must_be(symbol::identifier); // must be list of identifiers
                }
                scan->must_be(symbol::colon_sym);                                   // there must be a colon following ident/ident list
                if (!scan->have(symbol::value_sym) && !scan->have(symbol::ref_sym)) // looking for parameter kind
                {
                    // error we expect parameter kind
                    error->flag(scan->this_token(), 94);
                }
                else // if we do have a kind of parameter
                {
                    // get the kind so we can update in the future
                    if (scan->have(symbol::value_sym))
                    {
                        kindFix = lille_kind::value_param;
                    }
                    else
                    {
                        kindFix = lille_kind::ref_param;
                    }
                    scan->get_token(); // correct so get next token
                }
                // next thing must be a type declaration
                if (scan->have(symbol::integer_sym) || scan->have(symbol::real_sym) || scan->have(symbol::string_sym) || scan->have(symbol::boolean_sym))
                {
                    // get the correct type so we can fix up the params later
                    if (scan->have(symbol::integer_sym))
                    {
                        varType = lille_type::type_integer;
                    }
                    else if (scan->have(symbol::real_sym))
                    {
                        varType = lille_type::type_real;
                    }
                    else if (scan->have(symbol::string_sym))
                    {
                        varType = lille_type::type_string;
                    }
                    else // boolean logic
                    {
                        varType = lille_type::type_boolean;
                    }
                    scan->get_token(); // get a new token
                }
                else
                {
                    error->flag(scan->get_token(), 96); // throw type name expected error
                }
                // logic to look over list and fix the types and kinds
                for (int i = 0; i < params.size(); i++)
                {
                    params[i]->fix_type(varType); // fix the type and kind at this level
                    params[i]->fix_kind(kindFix);
                    profunHolder->add_param(params[i]); // add the parameters to the func or proc
                }
                // if there is a semi then we know theres more params
                // so continue looping through param logic
                if (!scan->have(symbol::semicolon_sym))
                {
                    prm = false; // if theres no more params exit logic
                }
            }
            scan->must_be(symbol::right_paren_sym); // must end parameter list
                                                    // with right parenth
        }
        if (fun) // if it is a function we need return and type before is
        {
            scan->must_be(symbol::return_sym); // must have a return symbol
            // if it is a function we must then store its return value
            if (scan->have(symbol::integer_sym) || scan->have(symbol::real_sym) || scan->have(symbol::string_sym) || scan->have(symbol::boolean_sym))
            {
                // get the correct type so we can fix up the function later
                if (scan->have(symbol::integer_sym))
                {
                    varType = lille_type::type_integer;
                }
                else if (scan->have(symbol::real_sym))
                {
                    varType = lille_type::type_real;
                }
                else if (scan->have(symbol::string_sym))
                {
                    varType = lille_type::type_string;
                }
                else // boolean logic
                {
                    varType = lille_type::type_boolean;
                }
                profunHolder->fix_return_type(varType); // fix the return type of the function

                scan->get_token();
            }
            else
            {
                error->flag(scan->get_token(), 96); // throw type name expected error
            }
        }
        scan->must_be(symbol::is_sym);
        BLOCK(bname);
    }
    scan->must_be(symbol::semicolon_sym);
    if (debugging)
    {
        cout << "exiting declaration" << endl;
    }
}

void parser::STATEMENT_LIST()
{
    if (debugging)
    {
        cout << "entering STATEMENT_LIST()" << endl;
    }
    // start statement list
    STATEMENT();
    scan->must_be(symbol::semicolon_sym); // each statement must terminate with ;
    while (IS_STATEMENT())                // loop on is statement
    {
        STATEMENT();
        scan->must_be(symbol::semicolon_sym); // each statement must terminate with ;
    }
    if (debugging)
    {
        cout << "exiting STATEMENT_LIST()" << endl;
    }
}

void parser::STATEMENT()
{
    currentHold = NULL; // this is the "TOP" of the comparison totem pole
    if (debugging)
    {
        cout << "entering STATEMENT()" << endl;
    }
    // if it is a compound statement call
    if (scan->have(symbol::if_sym) || scan->have(symbol::loop_sym) || scan->have(symbol::for_sym) || scan->have(symbol::while_sym))
    {
        COMPOUND_STATEMENT(); // call the compound statement function
    }
    else // if theres no compound key words then call simple statement
    {
        SIMPLE_STATEMENT();
    }
    // scan->get_token();
}

void parser::SIMPLE_STATEMENT()
{
    bool tcot = true;
    bool sing = false; // only for where expression only evald once
    bool par = false;  // parenth checking
    bool expr = true;  // used to control entry into expression logic
    bool exWhen = false;
    vector<id_table_entry *> identList;
    id_table_entry *locHold; // just a spot to hold a table entry
    if (debugging)
    {
        cout << "entering simple statement logic" << endl;
    }
    if (scan->have(symbol::read_sym)) // if theres a read symbol
    {
        if (debugging)
        {
            cout << "in simple statement: read symbol logic" << endl;
        }
        scan->get_token(); // get next sym
        if (scan->have(symbol::left_paren_sym))
        {
            par = true; // me must now check for right parenth
            scan->get_token();
        }
        if (scan->have(symbol::identifier)) // if theres an identifier it must be arith type
        {
            locHold = idTable->lookup(scan->this_token()); // lookup the current token
            if (locHold == NULL)                           // if there isn't a match
            {
                error->flag(scan->this_token(), 81);
            }
            else if (!locHold->tipe().is_type(lille_type::type_integer) && !locHold->tipe().is_type(lille_type::type_real))
            {
                // if the variable passed to read isnt an integer or real number
                error->flag(scan->this_token(), 86); // throw int or real expected
            }
        }
        // get rid of token
        scan->must_be(symbol::identifier);
        while (scan->have(symbol::comma_sym)) // if there are multiple identifiers
        {                                     // loop until no more idents
            scan->get_token();
            if (scan->have(symbol::identifier)) // if theres an identifier it must be arith type
            {
                locHold = idTable->lookup(scan->this_token()); // lookup the current token
                if (locHold == NULL)                           // if there isn't a match
                {
                    error->flag(scan->this_token(), 81);
                }
                else if (!locHold->tipe().is_type(lille_type::type_integer) && !locHold->tipe().is_type(lille_type::type_real))
                {
                    // if the variable passed to read isnt an integer or real number
                    error->flag(scan->this_token(), 86); // throw int or real expected
                }
            }
            scan->must_be(symbol::identifier);
        }

        if (par) // if theres opening paren must be closing
        {
            scan->must_be(symbol::right_paren_sym);
        }
    }
    else if (scan->have(symbol::null_sym))
    {
        if (debugging)
        {
            cout << "in simple statement: null symbol logic" << endl;
        }
        scan->get_token(); // just get next token and return to statement
    }
    else // ident, exit, return, write, writeln
    {
        if (scan->have(symbol::identifier)) // if theres an identifier
        {
            if (debugging)
            {
                cout << "in simple statement: identifier symbol logic" << endl;
            }
            // lookup the identifier and make sure that it is in the data struct
            locHold = idTable->lookup(scan->this_token());
            // if the entry isn't found throw an appropriate error
            // and add the identifier to the list to avoid seg faults
            if (locHold == NULL)
            {
                error->flag(scan->this_token(), 81);              // identifier not prev declared
                locHold = new id_table_entry(scan->this_token()); // create new one to avoid seg faults
            }
            scan->get_token();
            if (scan->have(symbol::left_paren_sym)) // check for parent
            {
                par = true;        // theres a parenth
                scan->get_token(); // get what follows paren
            }
            else if (scan->have(symbol::semicolon_sym)) // if it is just an ident
            {
                expr = false; // dont parse expression logi
                scan->get_token();
            }
            else // if not parens then must be becomes
            {
                scan->must_be(symbol::becomes_sym);
                sing = true;
            }
        }
        else if (scan->have(symbol::exit_sym)) // if theres  an exit symbol
        {
            if (debugging)
            {
                cout << "in simple statement: exit symbol logic" << endl;
            }
            scan->get_token();
            if (!scan->have(symbol::when_sym)) // if theres no when then no need to calc for expressions
            {
                expr = false;
            }
            else
            {
                exWhen = true;
                sing = true;
                scan->get_token(); // only get another if theres the when symbol
            }
        }
        else if (scan->have(symbol::return_sym))
        { // need to see if there a expres following
            if (debugging)
            {
                cout << "in simple statement: return symbol logic" << endl;
            }
            scan->get_token();
            if (scan->have(symbol::semicolon_sym))
            { // if theres a semicolon we know no expression
                expr = false;
            }
            sing = true;
        }
        else if (scan->have(symbol::write_sym))
        {
            scan->get_token();
            if (scan->have(symbol::left_paren_sym))
            {
                par = true;
                scan->get_token();
            }
        }
        else if (scan->have(symbol::writeln_sym)) // keep track of parents
        {
            scan->get_token();
            if (scan->have(symbol::left_paren_sym))
            {
                scan->get_token();
                if (scan->have(symbol::right_paren_sym))
                { // if it is just write() then dont need to parse for expr
                    expr = false;
                    scan->get_token();
                }
                par = true;
            }
            if (scan->have(symbol::semicolon_sym))
            {
                expr = false;
            }
        }

        if (expr) // if we are doing expresion parsing logic
        {
            if (!IS_EXPRESSION()) // if there is not a simple expression
            {
                error->flag(scan->this_token(), 83);
            }
            // from this point currentHold should have some sort of val we need to compare
            // currentHold is populated
            if (exWhen)
            {
                // if it is an exit when function than it must be followed by a boolean type
                if (!currentHold->tipe().is_type(lille_type::type_boolean))
                {
                    error->flag(currentHold->token_value(), 103);
                }
            }
            if (sing && !exWhen) // if there is an assignment or the becomes symbol
            {
                // if the identifier is a real
                if (locHold->tipe().is_type(lille_type::type_real))
                {
                    // the expression must either result in a real or an int
                    if (!currentHold->tipe().is_type(lille_type::type_real) && !currentHold->tipe().is_type(lille_type::type_integer))
                    {
                        error->flag(currentHold->token_value(), 118);
                    }
                    else // if there is no type issue than just set value of ident to expression
                    {
                        locHold->fix_const(0, currentHold->real_value());
                    }
                }
                // if the identifier is a
                else if (locHold->tipe().is_type(lille_type::type_integer))
                {
                    if (!currentHold->tipe().is_type(lille_type::type_integer))
                    {
                        error->flag(currentHold->token_value(), 119); // throw integer expression expected
                    }
                }
                // if the identifier is a string
                else if (locHold->tipe().is_type(lille_type::type_string))
                {
                    if (!currentHold->tipe().is_type(lille_type::type_string))
                    {
                        error->flag(currentHold->token_value(), 103); // string is expected error
                    }
                }
            }
            while (scan->have(symbol::comma_sym) && !sing) // loop if theres an expression list
            {                                              // also only loop for the ones that are allowed expression lists
                scan->get_token();
                if (!IS_EXPRESSION()) // if there is not a simple expression
                {
                    error->flag(scan->this_token(), 83);
                }
                if (exWhen)
                {
                    // if it is an exit when function than it must be followed by a boolean type
                    if (!currentHold->tipe().is_type(lille_type::type_boolean))
                    {
                        error->flag(currentHold->token_value(), 103);
                    }
                }
            }
            if (par)
            {
                scan->must_be(symbol::right_paren_sym);
            }
        }
    }
    if (debugging)
    {
        cout << "exiting SIMPLE_STATEMENT()" << endl;
    }
}

void parser::COMPOUND_STATEMENT()
{
    // compound statement code
    if (scan->have(symbol::if_sym))
    {
        IF_STATEMENT();
    }
    else if (scan->have(symbol::loop_sym))
    {
        LOOP_STATEMENT();
    }
    else if (scan->have(symbol::for_sym))
    {
        FOR_STATEMENT();
    }
    else
    {
        WHILE_STATEMENT();
    }
}

void parser::IF_STATEMENT()
{
    scan->get_token();    // get the next token
    if (!IS_EXPRESSION()) // if symbol must be followed by an expression
    {
        error->flag(scan->this_token(), 103);
    }
    scan->must_be(symbol::then_sym); // must be a then symbol
    STATEMENT_LIST();
    while (scan->have(symbol::elsif_sym)) // can be multiple elsif statemets
    {
        scan->get_token();
        if (!IS_EXPRESSION()) // must have expression
        {
            error->flag(scan->this_token(), 103);
        }
        scan->must_be(symbol::then_sym);
        STATEMENT_LIST();
    }
    if (scan->have(symbol::else_sym)) // if there's an else
    {
        scan->get_token();
        STATEMENT_LIST();
    }
    scan->must_be(symbol::end_sym); // finish with end if
    scan->must_be(symbol::if_sym);
}

void parser::LOOP_STATEMENT() // eval loop statement
{
    scan->must_be(symbol::loop_sym);
    // scan->get_token();
    STATEMENT_LIST();
    scan->must_be(symbol::end_sym);
    scan->must_be(symbol::loop_sym);
}

void parser::FOR_STATEMENT()
{
    currentHold = NULL;
    bool rev = false; // track if there is a reverse
    if (debugging)
    {
        cout << "entering for logic" << endl;
    }
    scan->get_token();
    if (scan->have(symbol::identifier))
    {
        idTable->enter_id(scan->this_token());
    }
    scan->must_be(symbol::identifier);
    scan->must_be(symbol::in_sym);
    if (scan->have(symbol::reverse_sym))
    {
        rev = true;
        scan->get_token();
    }
    // now range <simple_expr> .. <simple_expr>
    simple = true;        // signify to IS_EXPRESSION() to do simple expression only
    if (!IS_EXPRESSION()) // must have a simple expression
    {
        error->flag(scan->this_token(), 83);
    }
    // check and make sure the first expression evaluates to an integer
    if (!currentHold->tipe().is_type(lille_type::type_integer))
    {
        error->flag(currentHold->token_value(), 104);
    }
    scan->must_be(symbol::range_sym);

    if (!IS_EXPRESSION()) // must have simple expression
    {
        error->flag(scan->this_token(), 83);
    }
    // scan->get_token();
    // must be an integer .. integer
    if (!currentHold->tipe().is_type(lille_type::type_integer))
    {
        error->flag(currentHold->token_value(), 104);
    }
    simple = false; // reset simple expression flag
    LOOP_STATEMENT();
}

void parser::WHILE_STATEMENT()
{
    scan->get_token();
    if (!IS_EXPRESSION()) // must have boolean? expression
    {
        error->flag(scan->this_token(), 103);
    }
    LOOP_STATEMENT();
}

bool parser::IS_EXPRESSION()
{
    currentHold = NULL;
    // function to check for expression
    if (debugging)
    {
        cout << "entering expression logic" << endl;
        if (simple)
        {
            cout << "in expression: simple expression logic only" << endl;
        }
    }
    if (simple) // if it is a simple expression dont do extended logic
    {
        if (!IS_SIMPEX())
        {
            error->flag(scan->this_token(), 83);
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if (!IS_SIMPEX()) // must start with simple expression
        {
            return false;
        }
        // check for relational operators
        if (scan->have(symbol::greater_than_sym) || scan->have(symbol::less_than_sym) || scan->have(symbol::equals_sym) || scan->have(symbol::not_equals_sym) || scan->have(symbol::less_or_equal_sym) || scan->have(symbol::greater_or_equal_sym))
        {
            scan->get_token(); // get off relop
            if (!IS_SIMPEX())  // must be followed by other simple expression
            {
                return false;
            }
        }
        else if (scan->have(symbol::in_sym)) // if theres an in <range>
        {
            scan->get_token(); // get off in
            if (!IS_SIMPEX())  // must be followed by a simple expression
            {
                return false;
            }
            scan->must_be(symbol::range_sym); // must be range symbol
            if (!IS_SIMPEX())
            {
                return false;
            }
        }

        if (debugging)
        {
            cout << "exiting expression" << endl;
        }

        return true;
    }

    simple = false; // reset value at end of each time
}

// very important not to advance any tokens
bool parser::IS_STATEMENT() // we want the statement logic to pick up with checked token
{
    if (debugging)
    {
        cout << "entering IS_STATEMENT()" << endl;
    }
    if (scan->have(symbol::identifier) || scan->have(symbol::exit_sym) || scan->have(symbol::return_sym) || scan->have(symbol::read_sym) || scan->have(symbol::write_sym) || scan->have(symbol::writeln_sym) || scan->have(symbol::null_sym) || scan->have(symbol::if_sym) || scan->have(symbol::loop_sym) || scan->have(symbol::for_sym) || scan->have(symbol::while_sym))
    {
        return true;
    }
    else
    {
        return false;
    }
    if (debugging)
    {
        cout << "exiting IS_STATEMENT()" << endl;
    }
}

// return and check factor logic
bool parser::IS_FACTOR()
{
    // some entry storage
    id_table_entry *holdParent;
    symbol *tempSym;
    token *parTok;
    token *cOneTok;
    token *cTwoTok;
    id_table_entry *childOne;
    id_table_entry *childTwo;
    id_table_entry *temp;
    bool addr, subr, frst, boolFlag = false;
    float rH = 0.0;
    int iH = 0;
    lille_type compType;
    if (debugging)
    {
        cout << "entering factor logic check" << endl;
    }
    // if there is not an entry currently being held
    if (currentHold == NULL)
    {
        if (debugging)
        {
            cout << "CURRENT HOLD - NULL" << endl;
        }
        frst = true;
    }
    else // if there is a preceding factor  store that info
    {
        holdParent = currentHold;
        // check if it is a boolean and flip bool flag
        if (holdParent->tipe().is_type(lille_type::type_boolean))
        {
            boolFlag = true;
        }
    }

    // as soon as the function is called get the entry
    // **** FACTOR SECTION ****
    if (scan->have(symbol::plus_sym) || scan->have(symbol::minus_sym)) // if theres a preceding addop
    {
        if (boolFlag) // if theres a boolean shouldnt be followed by a plus/minus
        {
            error->flag(scan->this_token(), 103); // throw boolean expression expected flag
            return false;
        }
        // if theres an adding sign we want to add
        if (scan->have(symbol::plus_sym))
        {
            addr = true;
        }
        else
        {
            subr = true;
        }

        scan->get_token(); // get next token
    }
    if (!IS_PRIMARY()) // theres must be a primary
    {
        return false;
    }
    // after is primary there SHOULD be some sort of current_kind set
    // so store at local level
    childOne = currentHold;
    cOneTok = childOne->token_value();
    if (debugging)
    {
        cout << "IN FACTOR - currentHold type: " << currentHold->tipe().to_string() << endl;
    }

    if (scan->have(symbol::power_sym)) // check for <primary> ** <primary>
    {
        scan->get_token();
        if (!IS_PRIMARY()) // if it isnt followed by a primary then exit
        {
            return false;
        }
        // get the returned entry and type check then preform operation
        childTwo = currentHold;
        cTwoTok = childTwo->token_value();

        if (childOne->tipe().is_type(lille_type::type_real) && childTwo->tipe().is_type(lille_type::type_integer)) // if the first primary is a real than the expression evaluates to a real
        {
            // do the actual operation
            rH = pow(childOne->real_value(), childTwo->integer_value());
            compType = lille_type::type_real;   // if it starts with a real then the whole expression evals to real
            tempSym->set_sym(symbol::real_num); // set the symbol currectly
        }
        else if (childOne->tipe().is_type(lille_type::type_integer) && childTwo->tipe().is_type(lille_type::type_integer))
        {
            iH = pow(childOne->integer_value(), childTwo->integer_value());
            compType = lille_type::type_integer; // get the type set
            tempSym->set_sym(symbol::integer);   // set the symbol correctly
        }
        else
        {
            // if theres a type incompatability throw appropriate error at correct spot
            if (!childOne->tipe().is_type(lille_type::type_integer) && !childOne->tipe().is_type(lille_type::type_integer))
            {
                error->flag(cOneTok, 86); // throw int or real expected
            }
            else if (!childTwo->tipe().is_type(lille_type::type_integer))
            {
                error->flag(cTwoTok, 3); // must be integer on right side
            }
            return false;
        }
        // create new id table entry with all the computer infor an set currentHold to it
        parTok = new token(tempSym, scan->this_token()->get_line_number(), scan->this_token()->get_pos_on_line());
        // need to make sure that we actually set appropriate values based on type
        if (iH != 0)
        {
            parTok->set_integer_value(iH);
        }
        else
        {
            parTok->set_real_value(rH);
        }
        temp = new id_table_entry(parTok, compType); // create new entry
        temp->fix_const(iH, rH);                     // theoretically will only be a int or real
        // set the exiting holder before returning
        currentHold = temp;

        return true;
    }

    else if (!frst && (addr || subr) && !boolFlag) // if there is not some sort of preceding ordered or identifier
    {
        // need to type check everything to make sure its compatable
        if (holdParent->tipe().is_type(lille_type::type_integer))
        {
            // if it is a real than the resulting type is real
            if (childOne->tipe().is_type(lille_type::type_real))
            {
                // calc the actual new value to be stored
                if (addr) // if we need to add
                {
                    rH = holdParent->integer_value() + childOne->real_value();
                }
                else // subtraction
                {
                    rH = holdParent->integer_value() - childOne->real_value();
                }
                // create a new token and create entry for id table
                compType = lille_type::type_real;
                tempSym->set_sym(symbol::real_num);
                parTok = new token(tempSym, scan->this_token()->get_line_number(), scan->this_token()->get_pos_on_line());
                // set the calculated value and add to token
                parTok->set_real_value(rH);
            }
            else if (childOne->tipe().is_type(lille_type::type_integer)) // if its integer +/- integer
            {
                if (addr)
                {
                    iH = holdParent->integer_value() + childOne->integer_value();
                }
                else
                {
                    iH = holdParent->integer_value() - childOne->integer_value();
                }
                // create a new token and create entry for
                compType = lille_type::type_integer;
                tempSym->set_sym(symbol::integer);
                parTok = new token(tempSym, scan->this_token()->get_line_number(), scan->this_token()->get_pos_on_line());
                // set the calculated value and add to token
                parTok->set_integer_value(iH);
            }
            else // if it isnt followed by an int or real
            {
                error->flag(cOneTok, 86); // throw integer or real expecter error
                return false;
            }
            // create the new id table entry to replace the old current
            temp = new id_table_entry(parTok, compType);
            temp->fix_const(iH, rH);
            currentHold = temp;
        }
        else if (holdParent->tipe().is_type(lille_type::type_real)) // if there is a preceding real type
        {
            if (childOne->tipe().is_type(lille_type::type_integer))
            {
                if (addr) // if theres an adding operatior
                {
                    rH = holdParent->real_value() + childOne->integer_value();
                }
                else // if there is a subtractor
                {
                    rH = holdParent->real_value() - childOne->integer_value();
                }
            }
            else if (childOne->tipe().is_type(lille_type::type_real))
            {
                if (addr) // if theres an adding operatior
                {
                    rH = holdParent->real_value() + childOne->real_value();
                }
                else // if there is a subtractor
                {
                    rH = holdParent->real_value() - childOne->real_value();
                }
            }
            else // if it isnt followed by an int or real
            {
                error->flag(cOneTok, 86); // throw integer or real expecter error
                return false;
            }
            // create a new token and create entry for id table
            compType = lille_type::type_real;
            tempSym->set_sym(symbol::real_num);
            parTok = new token(tempSym, scan->this_token()->get_line_number(), scan->this_token()->get_pos_on_line());
            // set the calculated value and add to token
            parTok->set_real_value(rH);
            // create the new id table entry to replace the old current
            temp = new id_table_entry(parTok, lille_type::type_real);
            temp->fix_const(iH, rH);
            currentHold = temp;
        }
        return true;
    }
    if (debugging)
    {
        cout << "exiting factor logic" << endl;
        cout << "currentHolde type: " << currentHold->tipe().to_string() << endl;
    }

    return true;
}

// retrun and check term logic
bool parser::IS_TERM()
{
    // some entry storage
    id_table_entry *holdOne;
    id_table_entry *holdTwo;

    if (debugging)
    {
        cout << "entering term logic" << endl;
    }
    // must start with a factor
    if (!IS_FACTOR())
    {
        return false;
    }

    // if multiple factors togther
    while (scan->have(symbol::asterisk_sym) || scan->have(symbol::slash_sym) || scan->have(symbol::and_sym))
    {
        scan->get_token();
        if (!IS_FACTOR()) // must be followed by a factor
        {
            return false;
        }
    }
    if (debugging)
    {
        cout << "exiting term logic" << endl;
    }

    return true; // return true if logic checks out
}

// expr 2 checking logic
bool parser::IS_EXPR2()
{
    id_table_entry *holdOne;
    id_table_entry *holdTwo;

    if (debugging)
    {
        cout << "entering expr2 logic" << endl;
    }
    if (!IS_TERM()) // must start with a terms
    {
        return false;
    }

    // if theres multiple
    while (scan->have(symbol::plus_sym) || scan->have(symbol::minus_sym) || scan->have(symbol::or_sym))
    {
        // scan->get_token();
        if (!IS_TERM()) // must start with a terms
        {
            return false;
        }
        // scan->get_token();
    }
    if (debugging)
    {
        cout << "exiting expr2 logic" << endl;
    }
    return true; // if it gets through then return true
}

// simple expression logic
bool parser::IS_SIMPEX()
{
    if (debugging)
    {
        cout << "entering simpex logic" << endl;
    }
    if (!IS_EXPR2()) // must start with an expression
    {
        return false;
    }

    // if theres a list of expr2 & expr2
    while (scan->have(symbol::ampersand_sym))
    {
        scan->get_token(); // get off & symbol
        if (!IS_EXPR2())   // must be followed by expression
        {
            return false;
        }
    }
    if (debugging)
    {
        cout << "exiting simpex logic" << endl;
    }
    return true;
}

bool parser::IS_PRIMARY()
{
    id_table_entry *tempHold;
    lille_type *idType;
    int parCount = 1;
    token *tempTok;

    if (debugging)
    {
        cout << "checking primary logic" << endl;
        scan->this_token()->print_token();
        cout << endl;
    }
    // if its a number/string/bool store into ID table and pass entrty to current hold which is accessible by caller for type checking
    if (scan->have(symbol::real_num) || scan->have(symbol::integer) || scan->have(symbol::true_sym) || scan->have(symbol::false_sym) || scan->have(symbol::strng))
    {
        if (debugging)
        {
            cout << "ordered found" << endl;
        }
        if (scan->have(symbol::real_num)) // based off the type that it is enter it into the table and store in currentHold
        {
            currentHold = idTable->enter_id(scan->this_token(), lille_type::type_real, lille_kind::constant);
            currentHold->fix_const(0, scan->this_token()->get_real_value());
        }
        else if (scan->have(symbol::integer))
        {
            currentHold = idTable->enter_id(scan->this_token(), lille_type::type_integer, lille_kind::constant);
            currentHold->fix_const(scan->this_token()->get_integer_value());
        }
        else if (scan->have(symbol::strng))
        {
            currentHold = idTable->enter_id(scan->this_token(), lille_type::type_string, lille_kind::constant);
            currentHold->fix_const(0, 0, scan->this_token()->get_string_value());
        }
        else
        {
            currentHold = idTable->enter_id(scan->this_token(), lille_type::type_boolean, lille_kind::constant);
            if (scan->have(symbol::true_sym))
            {
                currentHold->fix_const(0, 0, "", true);
            }
            else
            {
                currentHold->fix_const(0, 0, "", false);
            }
        }
        scan->get_token();
        if (debugging)
        {
            cout << "exiting primary" << endl;
            cout << "currentHold type: " << currentHold->tipe().to_string() << endl;
        }
        return true;
    }
    else if (scan->have(symbol::identifier)) // if theres an identifier
    {
        // we want to look for it and store it so calling function can access information
        // about it
        currentHold = idTable->lookup(scan->this_token()); // look in the id table
        if (currentHold == NULL)                           // if the search doesn't resolve
        {
            error->flag(scan->this_token(), 81);
            currentHold = idTable->enter_id(scan->this_token()); // add to the id table to avoid segmentation faults
        }

        scan->get_token();                      // get off the identifier token
        if (scan->have(symbol::left_paren_sym)) // if thers an expression list
        {
            // NOTE THAT YOU CAN CALL A FUNCTION AS A PARAMETER TO ANOTHER
            // FUNC/PROC that is needing to take a paremeter
            // need to check and make sure that they are a procedure or type
            if (!currentHold->tipe().is_type(lille_type::type_proc) && !currentHold->tipe().is_type(lille_type::type_func))
            {
                // need to implement some sort of check to see if it needs to be a function
                // call based on if it is part of an expression
                error->flag(currentHold->token_value(), 121); // function call expected
            }
            // temp hold should be holding the proc/fun id table entry
            tempHold = currentHold; // when is expression gets called it will modify current hold

            // get the next expression
            scan->get_token();
            if (scan->have(symbol::right_paren_sym)) // we need to check for func() right?
            {
                if (tempHold->number_of_params() == 0) // if there are no parameters expected
                {
                    return true;
                }
                else // we are expecting parameters
                {
                    error->flag(scan->this_token(), 95); // number of parameters does not match error
                    return false;
                }
            }

            // if there are parameters than lets keep evaluating the validity of each
            if (!IS_EXPRESSION()) // needs to be followed by an expression or list of expressions
            {
                error->flag(scan->this_token(), 116); // throw expression exprected error
                return false;
            }
            // after currentHold is retured from this expression we need to type check the
            // returned type to that of what we are expecting with the function/proc (should be the first param)
            else if (!currentHold->tipe().is_type(tempHold->nth_parameter(parCount)->tipe())) // if there isn't a match
            {
                error->flag(currentHold->token_value(), 98); // throw parameter types dont match error
                return false;
            }
            else
            {
                parCount++;
            }
            while (scan->have(symbol::comma_sym)) // if theres multiple expressions
            {
                scan->get_token();
                if (!IS_EXPRESSION()) // needs to be followed by an expression or list of expressions
                {
                    error->flag(scan->this_token(), 116); // throw expression exprected error
                    return false;
                }
                else if (!currentHold->tipe().is_type(tempHold->nth_parameter(parCount)->tipe())) // if there isn't a match
                {
                    error->flag(currentHold->token_value(), 98); // throw parameter types dont match error
                    return false;
                }
                else
                {
                    parCount++;
                }
            }
            if (scan->have(symbol::right_paren_sym)) // must close parenths
            {
                // check to make sure that the correct number of parameters were supplied
                if (parCount < tempHold->number_of_params())
                {
                    error->flag(tempHold->token_value(), 97); // number of parameters dont match error
                    return false;
                }
                // construct id table entry to be sent back to parent with correct type
                tempTok = new token();                                              // create a null new token
                currentHold = new id_table_entry(tempTok, tempHold->return_tipe()); // make current hold type the return type of the successful function
                scan->get_token();
                return true;
            }
            else
            {
                error->flag(scan->this_token(), 21);
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    else if (scan->have(symbol::left_paren_sym)) // if theres a left parenth
    {
        simple = true; // only evaluating simple statement logic
        scan->get_token();
        if (!IS_EXPRESSION()) // needs to be followed by an expression or list of expressions
        {
            error->flag(scan->this_token(), 116); // throw expression exprected error
            return false;
        }
        if (scan->have(symbol::right_paren_sym))
        {
            scan->get_token();
            return true;
        }
        else // need closing paren so throw error
        {
            error->flag(scan->this_token(), 21);
            return false;
        }
    }
    else if (scan->have(symbol::not_sym) || scan->have(symbol::odd_sym))
    {
        scan->get_token();      // get next token
        return IS_EXPRESSION(); // must be followed by a single expression
    }

    if (debugging)
    {
        cout << "exiting primary check" << endl;
    }

    return false;
}