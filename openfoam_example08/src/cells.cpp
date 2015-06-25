/*
 * cells.cpp
 *
 *  Created on: 2015. 2. 26.
 *      Author: asran
 */

#include "cells.h"

namespace	openfoam
{

Cells::Cells		(	void	)
{

}

Cells::~Cells		(	void	)
{

}

void		Cells::add			(	Cell&	face	)
{
	mData.push_back(face);
}

Cell&		Cells::get			(	size_t	index	)
{
	return	mData[index];
}

}
