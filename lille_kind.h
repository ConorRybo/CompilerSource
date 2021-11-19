/*
 * lillekind.h
 *
 *  Created on: Jun 19, 2020
 *      Author: Michael Oudshoorn
 */

#ifndef LILLE_KIND_H_
#define LILLE_KIND_H_

#include <string>

using namespace std;

class lille_kind {
public:
	enum lille_knd
		{
			variable,
			constant,
			value_param,
			ref_param,
			for_ident,
			unknown
		};

	lille_kind();
	lille_kind(lille_knd k);
	lille_kind get_kind();
	bool is_kind(lille_kind k);
	bool is_kind(lille_knd k);
	string to_string();

private:
	lille_knd kind;
};

#endif /* LILLE_KIND_H_ */
