/*
 * openfoam_boundary.h
 *
 *  Created on: 2015. 7. 22.
 *      Author: asran
 */

#ifndef OPENFOAM_PARSER_DICTIONARY_H_
#define OPENFOAM_PARSER_DICTIONARY_H_

#include "openfoam_parser.h"
#include "openfoam_infodata.h"

namespace openfoam
{

class	Parser_dictionary:
		public Parser
{
private:
	InfoData	mInfoData;
public:
				Parser_dictionary		(	void	);
				Parser_dictionary		(	const char		fileName[],
											OpenType		type
										);
	virtual	~Parser_dictionary	(	void	);
public:
	void	writeData	(	InfoData	info	);
protected:
	virtual void	doData	(	char	val		);
public:
	inline InfoData	getData	(	void	);
};

InfoData	Parser_dictionary::getData	(	void	)
{
	mReadData	=	false;
	return	mInfoData;
}

}

#endif /* OPENFOAM_PARSER_DICTIONARY_H_ */
