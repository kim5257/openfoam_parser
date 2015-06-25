/*
 * openfoam_buf.h
 *
 *  Created on: 2015. 2. 12.
 *      Author: asran
 */

#ifndef INCLUDE__OPENFOAM_BUF_H_
#define INCLUDE__OPENFOAM_BUF_H_

#include <stdio.h>

namespace openfoam
{

class	Buffer
{
private:
	char*		mBuf;			///< 버퍼
	size_t		mBufSize;		///< 버퍼 크기
public:
				Buffer		(	size_t		size	);
	virtual	~Buffer	(	void	);
private:
	void		createBuf		(	size_t		size	);
	void		destroyBuf		(	void	);
public:
	inline size_t		getSize		(	void	);
public:
	inline operator char*	(	void	);
};

size_t		Buffer::getSize		(	void	)
{
	return	mBufSize;
}

Buffer::operator char*	(	void	)
{
	return	mBuf;
}

}

#endif /* INCLUDE__OPENFOAM_BUF_H_ */
