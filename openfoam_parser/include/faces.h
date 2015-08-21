/*
 * faces.h
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#ifndef FACES_H_
#define FACES_H_

#include <openfoam_error.h>
#include <openfoam_parser.h>
#include <openfoam_parser_labellist.h>
#include <openfoam_typedef.h>

namespace openfoam
{

class	Faces
{
private:
	Parser				mParser;
	face_vector_t		mData;
public:
				Faces		(	void	);
				Faces		(	const char			fileName[],
								Parser::OpenType	type
							);
	virtual	~Faces		(	void	);
public:
	void		open		(	const char			fileName[],
								Parser::OpenType	type
							);
	void		close		(	void	);
	void		readFile	(	void	);
	void		writeFile	(	void	);
	Face&		getData	(	size_t			index	);
	void		copy		(	const Faces&	faces	);
public:
	inline Hdr&		getHdr			(	void	);
	inline size_t		getSize		(	void	);
	inline Face&		operator[]		(	size_t		index	);
};

Hdr&		Faces::getHdr		(	void	)
{
	return	mParser.refHdr();
}

size_t		Faces::getSize		(	void	)
{
	return	mData.size();
}

Face&		Faces::operator[]	(	size_t		index	)
{
	return	getData(index);
}

}

#endif /* FACES_H_ */
