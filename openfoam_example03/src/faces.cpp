/*
 * faces.cpp
 *
 *  Created on: 2015. 2. 25.
 *      Author: asran
 */

#include "faces.h"

namespace	openfoam
{

Faces::Faces		(	void	)
{

}

Faces::~Faces		(	void	)
{

}

void		Faces::add			(	Face&	face	)
{
	mData.push_back(face);
}

Face&		Faces::get			(	size_t	index	)
{
	return	mData[index];
}

}
