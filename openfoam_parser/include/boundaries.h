/*
 * boundaries.h
 *
 *  Created on: 2015. 7. 23.
 *      Author: asran
 */

#ifndef BOUNDARIES_H_
#define BOUNDARIES_H_

#include <openfoam_error.h>
#include <openfoam_parser.h>
#include <openfoam_parser_boundary.h>
#include <openfoam_typedef.h>

namespace openfoam
{

class	Boundaries
{
private:
	Parser_boundary		mParser;
	boundary_vector_t		mData;
public:
				Boundaries		(	const char			fileName[],
									Parser::OpenType	type
								);
	virtual	~Boundaries	(	void	);
public:
	void		readFile	(	void	);
	void		writeFile	(	void	);
	Boundary&	getData	(	size_t				index	);
	void		copy		(	const Boundaries&	boundaries	);
public:
	inline Hdr&			getHdr			(	void	);
	inline size_t			getSize		(	void	);
	inline Boundary&		operator[]		(	size_t		index	);
};

Hdr&		Boundaries::getHdr		(	void	)
{
	return	mParser.refHdr();
}

size_t		Boundaries::getSize		(	void	)
{
	return	mData.size();
}

Boundary&	Boundaries::operator[]	(	size_t		index	)
{
	return	getData(index);
}

}

#endif /* BOUNDARIES_H_ */
