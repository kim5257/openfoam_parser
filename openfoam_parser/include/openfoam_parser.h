/*
 * openfoam_parser.h
 *
 *  Created on: 2015. 2. 12.
 *      Author: asran
 */

#ifndef INCLUDE__OPENFOAM_PARSER_H_
#define INCLUDE__OPENFOAM_PARSER_H_

#include "openfoam_buf.h"
#include "openfoam_debug.h"
#include "openfoam_hdr.h"
#include <stack>
#include <queue>
#include <string>

namespace openfoam
{

class	Parser
{
private:
	FILE*		mFile;
private:
	Buffer		mBuf;
	size_t		mDataSize;
	size_t		mDataOffset;
private:
	Hdr			mHdr;
protected:
	bool		mBlanked;
	bool		mReadHdr;
	bool		mReadSize;
	bool		mReadData;
	size_t		mDataDepth;
	size_t		mElemSize;
protected:
	std::string				mToken;
	std::string				mString;
	std::queue<std::string>	mStrQueue;
	std::queue<double>		mDataQueue;
protected:
	enum	State
	{
		NOTHING,
		COMMENT,
		CPP_COMMENT,
		HEADER,
		DATA,
		STRING,
	};
	State					mCurState;
	std::stack<State>		mStateStack;
public:
				Parser		(	const char		fileName[]		);
	virtual	~Parser	(	void	);
private:
	void	open	(	const char		fileName[]		);
	void	close	(	void	);
public:
	void	readHdr	(	void	);
	void	readSize	(	void	);
	bool	readData	(	void	);
public:
	bool	doHandle	(	void	);
	void	readBuf	(	void	);
protected:
	void	changeState	(	State		state	);
	void	returnState	(	void	);
protected:
	virtual void		doNothing		(	char	val		);
	virtual void		doComment		(	char	val		);
	virtual void		doCppComment	(	char	val		);
	virtual void		doHeader		(	char	val		);
	virtual void		doData			(	char	val		);
	virtual void		doString		(	char	val		);
protected:
	bool	isDigit	(	const char		str[]	);
protected:
	inline bool	isBlank	(	char	val		);
	inline	bool	isDigit	(	char	val		);
public:
	inline Hdr&	getHdr		(	void	);
	inline size_t	getSize	(	void	);
	inline std::queue<double>	getData	(	void	);
};

bool	Parser::isBlank	(	char	val		)
{
	bool	blank	=	false;

	switch( val )
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
		blank	=	true;
		break;
	}

	return	blank;
}

bool	Parser::isDigit	(	char	val		)
{
	bool	digit	=	false;

	if( ('0' <= val) &&
		(val <= '9') )
	{
		digit	=	true;
	}

	return	digit;
}

Hdr&	Parser::getHdr	(	void	)
{
	return	mHdr;
}

size_t	Parser::getSize	(	void	)
{
	return	mElemSize;
}

std::queue<double>	Parser::getData	(	void	)
{
	mReadData	=	false;
	return	mDataQueue;
}

}

#endif /* INCLUDE__OPENFOAM_PARSER_H_ */
