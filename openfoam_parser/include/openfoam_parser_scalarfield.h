/*
 * openfoam_parser_scalarfield.h
 *
 *  Created on: 2016. 2. 17.
 *      Author: arun
 */

#ifndef OPENFOAM_PARSER_SCALARFIELD_H_
#define OPENFOAM_PARSER_SCALARFIELD_H_

#include "openfoam_parser.h"
#include "openfoam_infodata.h"

namespace openfoam
{

class	Parser_scalarfield:
		public Parser
{
private:
	InfoData	mInfoData;
public:
				Parser_scalarfield		(	void	);
				Parser_scalarfield		(	const char		fileName[],
												OpenType		type
											);
	virtual	~Parser_scalarfield		(	void	);
public:
	void	writeDataStart	(	void	);
	void	writeDataEnd		(	void	);
	void	writeData			(	InfoData	info	);
protected:
	virtual void	doNothing	(	char	val		);
	virtual void	doData		(	char	val		);
public:
	inline InfoData	getData	(	void	);
};

InfoData	Parser_scalarfield::getData	(	void	)
{
	mReadData	=	false;
	return	mInfoData;
}

}

#endif /* OPENFOAM_PARSER_SCALARFIELD_H_ */
