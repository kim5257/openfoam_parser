/*
 * owners.cpp
 *
 *  Created on: 2015. 2. 25.
 *      Author: asran
 */

#include "owners.h"

namespace	openfoam
{

Owners::Owners		(	void	)
{

}

Owners::~Owners		(	void	)
{

}

void		Owners::add		(	Owner&	face	)
{
	mData.push_back(face);
}

Owner&		Owners::get		(	size_t	index	)
{
	return	mData[index];
}

}
