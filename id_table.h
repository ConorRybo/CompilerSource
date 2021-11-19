/*
 * idtable.h
 *
 *  Created on: Jun 18, 2020
 *      Author: Michael Oudshoorn
 */

#ifndef ID_TABLE_H_
#define ID_TABLE_H_

#include <iostream>
#include <string>

#include "token.h"
#include "error_handler.h"

using namespace std;

class id_table {
private:
	error_handler* error;

public:
    id_table(error_handler* err);
    
    void dump_id_table(bool dump_all = true);
};

#endif /* ID_TABLE_H_ */
