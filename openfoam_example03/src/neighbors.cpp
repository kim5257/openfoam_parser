/*
 * neighbors.cpp
 *
 *  Created on: 2015. 2. 25.
 *      Author: asran
 */

#include "neighbors.h"

namespace	openfoam
{

Neighbors::Neighbors		(	void	)
{

}

Neighbors::~Neighbors	(	void	)
{

}

void			Neighbors::add		(	Neighbor&	face	)
{
	mData.push_back(face);
}

Neighbor&		Neighbors::get		(	size_t		index	)
{
	return	mData[index];
}

}
