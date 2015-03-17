/*
 * OpenFOAM_parser.h
 *
 *  Created on: 2015. 02. 11.
 *      Author: asran
 */

#ifndef OPENFOAM_PARSER_H_
#define OPENFOAM_PARSER_H_

#include <string>
#include <stack>
#include <queue>
#include "openfoam_hdr.h"

namespace openfoam
{

#define		BUF_SIZE		(1024)

class OpenFOAM_Parser
{
private:
	enum SyntaxState
	{
		NOTING,
		COMMENT,
		CPP_COMMENT,
		HEADER,
		HEADER_VERSION,
		HEADER_FORMAT,
		HEADER_CLASS,
		HEADER_LOCATION,
		HEADER_OBJECT,
		DATA,
		DATA_READ,
	};
private:
	FILE*						mFoamFile;
	char						mBuf[BUF_SIZE];
	size_t						mBufSize;
	size_t						mDataSize;
	size_t						mDataOffset;
	std::stack<SyntaxState>	mStateStack;
	std::queue<double>		mDataQueue;
	SyntaxState				mCurState;
	std::string				mTmpString;
	std::string				mToken;
private:
	bool		mReadHdr;
	bool		mReadDataStart;
	bool		mReadData;
	bool		mReadDataEnd;
	Hdr			mHdr;
	size_t		mDataLen;
	size_t		mElemLen;
public:
				OpenFOAM_Parser		(	const char		fileName[]	);
	virtual	~OpenFOAM_Parser		(	void	);
private:
	void		open			(	const char		fileName[]	);
	void		close			(	void	);
public:
	bool		doSequence		(	void	);
	void		readHdr		(	void	);
	void		readDataStart	(	void	);
	bool		readData		(	void	);
private:
	void		doParsing		(	char	val		);
	size_t		readToBuf		(	void	);

private:
	bool		readHdrData	(	char			val,
									std::string&	dest
								);
private:
	bool		isDigit		(	const char		val[]	);
public:
	inline		Hdr				getHdr			(	void	);
	inline		size_t			getDataLen		(	void	);
	inline		std::queue<double>&	getData		(	void	);
};

Hdr			OpenFOAM_Parser::getHdr		(	void	)
{
	return	mHdr;
}

size_t		OpenFOAM_Parser::getDataLen	(	void	)
{
	return	mDataLen;
}

std::queue<double>&	OpenFOAM_Parser::getData		(	void	)
{
	mReadData	=	false;
	return	mDataQueue;
}

}
#endif /* OPENFOAM_PARSER_H_ */
