/*
 * owners.h
 *
 *  Created on: 2015. 6. 17.
 *      Author: asran
 */

#ifndef INCLUDE__OWNERS_H_
#define INCLUDE__OWNERS_H_

#include <openfoam_error.h>
#include <openfoam_parser.h>
#include <openfoam_parser_labellist.h>
#include <openfoam_typedef.h>

namespace	openfoam
{

class	Owners
{
private:
	Parser_labelList	mParser;
	owner_vector_t	mData;
public:
				Owners		(	void	);
				Owners		(	const char			fileName[],
								Parser::OpenType	type
							);
	virtual	~Owners	(	void	);
public:
	void		open		(	const char			fileName[],
								Parser::OpenType	type
							);
	void		close		(	void	);
	void		readFile	(	void	);
	void		writeFile	(	void	);
	Owner&		getData	(	size_t			index	);
	void		copy		(	const Owners&	owners	);
public:
	inline Hdr&		getHdr			(	void	);
	inline size_t		getSize		(	void	);
	inline Owner&		operator[]		(	size_t		index	);
};

Hdr&		Owners::getHdr		(	void	)
{
	return	mParser.refHdr();
}

size_t		Owners::getSize		(	void	)
{
	return	mData.size();
}

Owner&		Owners::operator[]	(	size_t		index	)
{
	return	getData(index);
}

};

#endif /* INCLUDE__OWNERS_H_ */
