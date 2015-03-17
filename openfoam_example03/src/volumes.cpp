/*
 * volumes.cpp
 *
 *  Created on: 2015. 3. 2.
 *      Author: asran
 */

#include "volumes.h"

namespace	openfoam
{

Volumes::Volumes		(	void	)
{

}

Volumes::~Volumes		(	void	)
{

}

void		Volumes::add		(	Volume&	face	)
{
	mData.push_back(face);
}

Volume&		Volumes::get		(	size_t	index	)
{
	return	mData[index];
}

}
