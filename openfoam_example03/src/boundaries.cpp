/*
 * boundary.cpp
 *
 *  Created on: 2015. 2. 26.
 *      Author: asran
 */

#include "boundaries.h"

namespace	openfoam
{

Boundaries::Boundaries		(	void	)
{

}

Boundaries::~Boundaries	(	void	)
{

}

void			Boundaries::add		(	Boundary&	face	)
{
	mData.push_back(face);
}

Boundary&		Boundaries::get		(	size_t		index	)
{
	return	mData[index];
}

}
