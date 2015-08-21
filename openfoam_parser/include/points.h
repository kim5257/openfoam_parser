/*
 * points.h
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#ifndef POINTS_H_
#define POINTS_H_

#include <openfoam_error.h>
#include <openfoam_parser.h>
#include <openfoam_parser_labellist.h>
#include <openfoam_typedef.h>

namespace openfoam
{

class	Points
{
private:
	Parser				mParser;
	point_vector_t	mData;
public:
				Points		(	void	);
				Points		(	const char			fileName[],
								Parser::OpenType	type
							);
	virtual	~Points	(	void	);
public:
	void		open		(	const char			fileName[],
								Parser::OpenType	type
							);
	void		close		(	void	);
	void		readFile	(	void	);
	void		writeFile	(	void	);
	Point&		getData	(	size_t			index	);
	void		copy		(	const Points&	points	);
public:
	inline Hdr&		getHdr			(	void	);
	inline size_t		getSize		(	void	);
	inline Point&		operator[]		(	size_t		index	);
};

Hdr&		Points::getHdr		(	void	)
{
	return	mParser.refHdr();
}

size_t		Points::getSize		(	void	)
{
	return	mData.size();
}

Point&		Points::operator[]	(	size_t		index	)
{
	return	getData(index);
}

}

#endif /* POINTS_H_ */
