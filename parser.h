/*
    parser.h
    Conor Rybacki
*/
#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <fstream>
#include <string>

#include "symbol.h"
#include "token.h"
#include "error_handler.h"
#include "id_table.h"
#include "symbol.h"
#include "scanner.h"

using namespace std;

class parser
{
private:
    bool debugging{false};
    bool simple;
    // bool gdug;
    scanner *scan;
    error_handler *error;
    id_table *idTable;
    string ident_name;
    id_table_entry *currentHold;
    vector<id_table_entry *> ent_hold;
    vector<lille_type> typeHolder;
    lille_type current_type;
    lille_kind current_kind;
    lille_type type_hold;

    void PROG();
    void BLOCK(string bName);
    void DECLARATION();
    void STATEMENT_LIST();
    void STATEMENT();
    void SIMPLE_STATEMENT();
    void COMPOUND_STATEMENT();
    void IF_STATEMENT();
    void LOOP_STATEMENT();
    void FOR_STATEMENT();
    void WHILE_STATEMENT();

    bool IS_EXPRESSION();
    bool IS_STATEMENT();
    bool IS_PRIMARY();
    bool IS_FACTOR();
    bool IS_TERM();
    bool IS_EXPR2();
    bool IS_SIMPEX();

public:
    parser(); // default constructor
    parser(scanner *s, id_table *idt, error_handler *e);
    // parameterized constructor to set appropriate values
};

#endif // PARSER_H_