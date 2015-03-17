/*
 * openfoam_buf.cpp
 *
 *  Created on: 2015. 2. 12.
 *      Author: asran
 */

#include "openfoam_buf.h"
#include "openfoam_error.h"
#include <stdio.h>
#include <new>

namespace openfoam
{

Buffer::Buffer	(	size_t		size
					)
:mBuf(NULL),
mBufSize(0)
{
	createBuf(size);
}

Buffer::~Buffer	(	void	)
{
	destroyBuf();
}

void		Buffer::createBuf		(	size_t		size
									)
{
	try
	{
		mBuf		=	new char[size];
		mBufSize	=	size;
	}
	catch( std::bad_alloc& )
	{
		throw	ErrMsg::createErrMsg("버퍼 할당 실패");
	}
}

void		Buffer::destroyBuf	(	void	)
{
	delete[]	mBuf;
}

}
