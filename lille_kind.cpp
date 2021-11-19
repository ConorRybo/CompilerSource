/*
 * lillekind.cpp
 *
 *  Created on: Jun 19, 2020
 *      Author: Michael Oudshoron
 */

#include <string>

#include "lille_kind.h"

using namespace std;

lille_kind::lille_kind() {
	kind = unknown;
}


lille_kind::lille_kind(lille_knd k)
{
	kind = k;
}


lille_kind lille_kind::get_kind()
{
	return kind;
}


bool lille_kind::is_kind(lille_kind k)
{
	return (this->kind == k.kind);
}


bool lille_kind::is_kind(lille_knd k)
{
	return (this->kind == k);
}


string lille_kind::to_string()
{
	switch (kind)
	{
	case variable:
		return "VARIABLE";
		break;
	case constant:
		return "CONSTANT";
		break;
	case value_param:
		return "VALUE_PARAM";
		break;
	case ref_param:
		return "REF_PARAM";
		break;
	case for_ident:
		return "FOR_IDENT";
		break;
	case unknown:
		return "UNKNOWN";
		break;
	default:
		return "ERROR";
		break;
	}
}
