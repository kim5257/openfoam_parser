/*
 * openfoam_parser_labellist.h
 *
 *  Created on: 2015. 2. 14.
 *      Author: asran
 */

#ifndef INCLUDE_OPENFOAM_PARSER_LABELLIST_H_
#define INCLUDE_OPENFOAM_PARSER_LABELLIST_H_

#include "openfoam_parser.h"

namespace openfoam
{

class	Parser_labelList:
		public Parser
{
public:
				Parser_labelList		(	const char		fileName[]		);
	virtual	~Parser_labelList		(	void	);
protected:
	virtual void	doData	(	char	val		);
};

}

#endif /* INCLUDE_OPENFOAM_PARSER_LABELLIST_H_ */
