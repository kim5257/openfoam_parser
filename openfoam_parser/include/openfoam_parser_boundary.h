/*
 * openfoam_boundary.h
 *
 *  Created on: 2015. 7. 22.
 *      Author: asran
 */

#ifndef OPENFOAM_PARSER_BOUNDARY_H_
#define OPENFOAM_PARSER_BOUNDARY_H_

#include "openfoam_parser.h"
#include "openfoam_infodata.h"

namespace openfoam
{

class	Parser_boundary:
		public Parser
{
private:
	InfoData	mInfoData;
public:
				Parser_boundary		(	void	);
				Parser_boundary		(	const char		fileName[],
											OpenType		type
										);
	virtual	~Parser_boundary		(	void	);
public:
	void	writeData	(	InfoData	info	);
protected:
	virtual void	doData	(	char	val		);
public:
	inline InfoData	getData	(	void	);
};

InfoData	Parser_boundary::getData	(	void	)
{
	mReadData	=	false;
	return	mInfoData;
}

}

#endif /* OPENFOAM_PARSER_BOUNDARY_H_ */
