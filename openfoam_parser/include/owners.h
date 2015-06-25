/*
 * owners.h
 *
 *  Created on: 2015. 6. 17.
 *      Author: asran
 */

#ifndef INCLUDE__OWNERS_H_
#define INCLUDE__OWNERS_H_

#include "openfoam.h"

namespace	openfoam
{

class	Owners
{
private:
	Parser_labelList	mParser;
	owner_vector_t	mData;
public:
				Owners		(	const char		fileName[]		);
	virtual	~Owners	(	void	);
public:
	void		prepare	(	void	);
	Owner&		getData	(	size_t		index	);
public:
	inline Hdr&		getHdr			(	void	);
	inline size_t		getSize		(	void	);
	inline Owner&		operator[]		(	size_t		index	);
};

Hdr&		Owners::getHdr		(	void	)
{
	return	mParser.getHdr();
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
