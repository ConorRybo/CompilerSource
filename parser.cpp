/*
    parser.cpp
    Conor Rybacki
*/

#include <iostream>
#include <fstream>
#include <string>

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
}

parser::parser(scanner *s, id_table *idt, error_handler *e)
{ // parameterized constructor
    // set corresponding private variables to passed vals
    scan = s;
    error = e;
    idTable = idt;
    simple = false;
    ident_name = "";
    PROG();
}

void parser::PROG()
{
    // setup the predefined tokens before running the first scan
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
        id_table_entry *run = idTable->enter_id(scan->this_token());
    }
    scan->must_be(symbol::identifier); // if it isn't an identifier throw err
    idTable->enter_new_scope();        // now we are entering the program so we change scope
    idTable->dump_id_table(true);
    scan->must_be(symbol::is_sym); // looking for the is symbol
    BLOCK(ident_name);             // call block and pass the name of the proc
    scan->must_be(symbol::semicolon_sym);
    idTable->exit_scope();
    idTable->dump_id_table(true);
    scan->must_be(symbol::end_of_program); // catches any trash after program
}

void parser::BLOCK(string bName)
{
    if (debugging)
    {
        cout << "entering block " << bName << endl;
    }

    while (scan->have(symbol::identifier) || scan->have(symbol::procedure_sym) || scan->have(symbol::function_sym)) // if the current symbol is an identifier
    {
        DECLARATION();
    } // call declaration to check the semantics
    // can have multiple lines of identifiers and assignment statements

    scan->must_be(symbol::begin_sym); // if there isn't any identifiers then must be begin
    STATEMENT_LIST();                 // call the statement list function
    scan->must_be(symbol::end_sym);
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

    if (debugging)
        cout << "entering declaration" << endl;
    // if it is an identifier list
    if (scan->have(symbol::identifier))
    {
        if (debugging)
        {
            cout << "starting declaration ident logic" << endl;
        }
        // enters this function ON the identifier token
        scan->get_token();                    // get the next token
        while (scan->have(symbol::comma_sym)) // if theres a comma (identifier list)
        {
            scan->get_token();                 // advance on to next token
            scan->must_be(symbol::identifier); // clarify that it's an identifier
        }
        // after loop exit we are looking for a comma
        scan->must_be(symbol::colon_sym); // ident or ident list must be followed by colon
        if (scan->have(symbol::constant_sym))
        { // if theres a constant symbol
            constant = true;
            scan->get_token();
        }
        // needs to be a type declaration next
        if (scan->have(symbol::integer_sym) || scan->have(symbol::real_sym) || scan->have(symbol::string_sym) || scan->have(symbol::boolean_sym))
        {
            scan->get_token();
        }
        else
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
                error->flag(scan->get_token(), 84); // expecting number, string, or boolean
            }
            scan->get_token();
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
            bname = scan->this_token()->get_identifier_value();
        }
        scan->must_be(symbol::identifier);
        if (scan->have(symbol::left_paren_sym)) // if theres a parameter list
        {
            while (prm)
            {
                scan->get_token();                 // get the next token
                scan->must_be(symbol::identifier); // check and throw out first ident
                while (scan->have(symbol::comma_sym))
                {                                      // if there is a list of identifiers
                    scan->get_token();                 // get rid of comma
                    scan->must_be(symbol::identifier); // must be list of identifiers
                }
                scan->must_be(symbol::colon_sym);                                   // there must be a colon following ident/ident list
                if (!scan->have(symbol::value_sym) && !scan->have(symbol::ref_sym)) // looking for parameter kind
                {
                    // error we expect parameter kind
                    error->flag(scan->this_token(), 94);
                }
                else
                {
                    scan->get_token(); // correct so get next token
                }
                // next thing must be a type declaration
                if (scan->have(symbol::integer_sym) || scan->have(symbol::real_sym) || scan->have(symbol::string_sym) || scan->have(symbol::boolean_sym))
                {
                    scan->get_token(); // get a new token
                }
                else
                {
                    error->flag(scan->get_token(), 96); // throw type name expected error
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
            scan->must_be(symbol::return_sym);
            if (scan->have(symbol::integer_sym) || scan->have(symbol::real_sym) || scan->have(symbol::string_sym) || scan->have(symbol::boolean_sym))
            {
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
        scan->must_be(symbol::identifier);
        while (scan->have(symbol::comma_sym)) // if there are multiple identifiers
        {                                     // loop until no more idents
            scan->get_token();
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
            while (scan->have(symbol::comma_sym) && !sing) // loop if theres an expression list
            {                                              // also only loop for the ones that are allowed expression lists
                scan->get_token();
                if (!IS_EXPRESSION()) // if there is not a simple expression
                {
                    error->flag(scan->this_token(), 83);
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
    if (debugging)
    {
        cout << "entering for logic" << endl;
    }
    scan->get_token();
    scan->must_be(symbol::identifier);
    scan->must_be(symbol::in_sym);
    if (scan->have(symbol::reverse_sym))
    {
        scan->get_token();
    }
    // now range <simple_expr> .. <simple_expr>
    simple = true;        // signify to IS_EXPRESSION() to do simple expression only
    if (!IS_EXPRESSION()) // must have a simple expression
    {
        error->flag(scan->this_token(), 83);
    }

    scan->must_be(symbol::range_sym);

    if (!IS_EXPRESSION()) // must have simple expression
    {
        error->flag(scan->this_token(), 83);
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
    if (debugging)
    {
        cout << "entering factor logic check" << endl;
    }
    // **** FACTOR SECTION ****
    if (scan->have(symbol::plus_sym) || scan->have(symbol::minus_sym)) // if theres a preceding addop
    {
        scan->get_token(); // get next token
    }
    if (!IS_PRIMARY()) // theres must be a primary
    {
        return false;
    }
    if (scan->have(symbol::power_sym)) // check for <primary> ** <primary>
    {
        scan->get_token();
        if (!IS_PRIMARY()) // if it isnt followed by a primary then exit
        {
            return false;
        }
    }
    if (debugging)
    {
        cout << "exiting factor logic" << endl;
    }
    return true;
}

// retrun and check term logic
bool parser::IS_TERM()
{
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
        scan->get_token();
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
    if (debugging)
    {
        cout << "checking primary logic" << endl;
    }
    if (scan->have(symbol::real_num) || scan->have(symbol::integer) || scan->have(symbol::true_sym) || scan->have(symbol::false_sym) || scan->have(symbol::strng))
    {
        scan->get_token();
        return true;
    }
    else if (scan->have(symbol::identifier)) // if theres an identifier
    {
        scan->get_token();
        if (scan->have(symbol::left_paren_sym)) // if thers an expression list
        {
            scan->get_token();
            if (!IS_EXPRESSION()) // needs to be followed by an expression or list of expressions
            {
                error->flag(scan->this_token(), 116); // throw expression exprected error
                return false;
            }
            while (scan->have(symbol::comma_sym)) // if theres multiple expressions
            {
                scan->get_token();
                if (!IS_EXPRESSION()) // needs to be followed by an expression or list of expressions
                {
                    error->flag(scan->this_token(), 116); // throw expression exprected error
                    return false;
                }
            }
            if (scan->have(symbol::right_paren_sym)) // must close parenths
            {
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