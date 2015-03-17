/*
 * points.cpp
 *
 *  Created on: 2015. 2. 25.
 *      Author: asran
 */

#include "points.h"

namespace	openfoam
{

Points::Points		(	void	)
{

}

Points::~Points		(	void	)
{

}

void		Points::add		(	Point&	face	)
{
	mData.push_back(face);
}

Point&		Points::get		(	size_t	index	)
{
	return	mData[index];
}

void		Points::clear		(	void	)
{
	mData.clear();
}

}
