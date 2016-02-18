/*
 * scalarfields.h
 *
 *  Created on: 2016. 2. 17.
 *      Author: arun
 */

#ifndef SCALARFIELDS_H_
#define SCALARFIELDS_H_

#include <openfoam_error.h>
#include <openfoam_parser.h>
#include <openfoam_parser_scalarfield.h>
#include <openfoam_typedef.h>

namespace openfoam
{

class	Scalarfields
{
private:
	Parser_scalarfield		mParser;
	scalarfield_vector_t		mData;
public:
				Scalarfields	(	void	);
				Scalarfields	(	const char			fileName[],
									Parser::OpenType	type
								);
	virtual	~Scalarfields	(	void	);
public:
	void		open		(	const char			fileName[],
								Parser::OpenType	type
							);
	void			close		(	void	);
	void			readFile	(	void	);
	void			writeFile	(	void	);
	ScalarField&	getData	(	size_t		index	);
	void			copy		(	const Scalarfields&	scalarfields	);
public:
	inline Hdr&			getHdr			(	void	);
	inline size_t			getSize		(	void	);
	inline ScalarField&	operator[]		(	size_t		index	);
};

Hdr&		Scalarfields::getHdr		(	void	)
{
	return	mParser.refHdr();
}

size_t		Scalarfields::getSize		(	void	)
{
	return	mData.size();
}

ScalarField&	Scalarfields::operator[]	(	size_t		index	)
{
	return	getData(index);
}

}

#endif /* SCALARFIELDS_H_ */
