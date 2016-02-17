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
public:
	enum	OpenType
	{
		FILE_NULL,
		FILE_READ,
		FILE_WRITE
	};
private:
	OpenType		mOpenType;
	std::string	mFileName;
	FILE*			mFile;
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
		DIMENSIONS,
		INTERNALFIELD,
	};
	State					mCurState;
	std::stack<State>		mStateStack;
public:
				Parser		(	void	);
				Parser		(	const char		fileName[],
								OpenType		type
							);
	virtual	~Parser	(	void	);
public:
	void	open	(	const char		fileName[],
						OpenType		type
					);
	void	close	(	void	);
	void	readHdr	(	void	);
	void	readSize	(	void	);
	bool	readData	(	void	);
public:
	void	writeComment		(	void	);
	void	writeHdr			(	void	);
	void	writeSize			(	size_t		size	);
	void	writeDataStart	(	void	);
	void	writeDataEnd		(	void	);
	void	writeData			(	std::queue<int>		data,
									bool					showFlag
								);
	void	writeData			(	std::queue<double>	data,
									bool					showFlag
								);
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
	virtual void		doDimensions	(	char	val		);
	virtual void		doInternalField	(	char	val		);
protected:
	bool	isDigit	(	const char		str[]	);
protected:
	inline bool	isBlank	(	char	val		);
	inline	bool	isDigit	(	char	val		);
protected:
	inline FILE*	getFP		(	void	);
public:
	inline	OpenType		getOpenType	(	void	) const;
	inline std::string	getFileName	(	void	) const;
	inline	Hdr		getHdr		(	void	) const;
	inline Hdr&	refHdr		(	void	);
	inline void	setHdr		(	const Hdr&		hdr		);
	inline size_t	getSize	(	void	) const;
	inline std::queue<double>	getData	(	void	);
	inline bool	isOpen	(	void	);
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

FILE*	Parser::getFP		(	void	)
{
	return	mFile;
}

Parser::OpenType		Parser::getOpenType	(	void	) const
{
	return	mOpenType;
}

std::string	Parser::getFileName	(	void	) const
{
	return	mFileName;
}

Hdr		Parser::getHdr	(	void	) const
{
	return	mHdr;
}

Hdr&	Parser::refHdr	(	void	)
{
	return	mHdr;
}

void	Parser::setHdr	(	const Hdr&		hdr		)
{
	mHdr	=	hdr;
}

size_t	Parser::getSize	(	void	) const
{
	return	mElemSize;
}

std::queue<double>	Parser::getData	(	void	)
{
	mReadData	=	false;
	return	mDataQueue;
}

bool	Parser::isOpen	(	void	)
{
	return	(mFile!=NULL)?(true):(false);
}

}

#endif /* INCLUDE__OPENFOAM_PARSER_H_ */
