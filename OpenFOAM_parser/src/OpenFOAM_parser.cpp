/*
 * OpenFOAM_parser.cpp
 *
 *  Created on: 2015. 02. 11.
 *      Author: asran
 */

#include "OpenFOAM_parser.h"
#include "openfoam_error.h"
#include <stdlib.h>

namespace openfoam
{

OpenFOAM_Parser::OpenFOAM_Parser	(	const char		fileName[]
										)
:mFoamFile(NULL),
mBufSize(BUF_SIZE),
mDataSize(0),
mDataOffset(0),
mCurState(NOTING),
mReadHdr(false),
mReadDataStart(false),
mReadData(false),
mReadDataEnd(false)
{
	open(fileName);

	memset(mBuf, 0, sizeof(mBuf[0]) * mBufSize);

	mTmpString.clear();
}

OpenFOAM_Parser::~OpenFOAM_Parser		(	void	)
{
}

void	OpenFOAM_Parser::open	(	const char		fileName[]
								)
{
	if( mFoamFile != NULL )
	{
		throw	ErrMsg::createErrMsg("파일을 이미 열기했습니다.");
	}

	mFoamFile	=	fopen	(	fileName,
								"r"
							);

	if( mFoamFile == NULL )
	{
		throw	ErrMsg::createErrMsg("파일 열기 실패");
	}
}

void	OpenFOAM_Parser::close	(	void	)
{
	fclose(mFoamFile);
	mFoamFile	=	NULL;
}

bool		OpenFOAM_Parser::doSequence		(	void	)
{
	bool	finish	=	false;

	if( mDataSize == mDataOffset )
	{
		mDataSize	=	readToBuf();
		mDataOffset	=	0;
	}

	if( mDataSize != 0 )
	{
		doParsing(mBuf[mDataOffset]);
		mDataOffset++;
	}
	else
	{
		finish	=	true;
	}

	return	finish;
}

void		OpenFOAM_Parser::doParsing		(	char	val		)
{
	switch( mCurState )
	{
	case NOTING:
		{
			if( val == '/' )
			{
				mToken	+=	val;
				if( mToken == "//" )
				{
					mStateStack.push(mCurState);
					mCurState	=	CPP_COMMENT;
					mToken.clear();
				}
			}
			else if( val == '*' )
			{
				if( mToken == "/" )
				{
					mStateStack.push(mCurState);
					mCurState	=	COMMENT;
					mToken.clear();
				}
			}
			else if( val == '{' )
			{
				if( mTmpString == "FoamFile" )
				{
					mStateStack.push(mCurState);
					mCurState	=	HEADER;
					mTmpString.clear();
				}
				mToken.clear();
			}
			else if( val == '(' )
			{
				if( isDigit(mTmpString.c_str()) )
				{
					mStateStack.push(mCurState);
					mCurState	=	DATA;
					mDataLen	=	atol(mTmpString.c_str());
					mReadDataStart	=	true;
					mTmpString.clear();
				}
				mToken.clear();
			}
			else if( val == ' ' )
			{
				mToken.clear();
			}
			else if( val == '\n' )
			{
				mToken.clear();
			}
			else
			{
				mToken.clear();
				mTmpString	+=	val;
			}
		}
		break;
	case COMMENT:
		{
			if( val == '*' )
			{
				if( mToken.size() != 0 )
				{
					mToken.clear();
					
				}
				
				mToken	+=	val;
			}
			else if( val == '/' )
			{
				mToken	+=	val;
				if( mToken == "*/" )
				{
					mCurState	=	mStateStack.top();
					mStateStack.pop();
					mToken.clear();
				}
			}
			else
			{
				mToken.clear();
			}
		}
		break;
	case CPP_COMMENT:
		{
			if( val == '\n' )
			{
				mCurState	=	mStateStack.top();
				mStateStack.pop();
			}
		}
		break;
	case HEADER:
		{
			switch( val )
			{
			case '}':
				{
					mCurState	=	mStateStack.top();
					mStateStack.pop();

					mReadHdr	=	true;
				}
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				mTmpString.clear();
				break;
			default:
				{
					mTmpString	+=	val;
					if( mTmpString == "version" )
					{
						mStateStack.push(mCurState);
						mCurState	=	HEADER_VERSION;
						mTmpString.clear();
					}
					else if( mTmpString == "format" )
					{
						mStateStack.push(mCurState);
						mCurState	=	HEADER_FORMAT;
						mTmpString.clear();
					}
					else if( mTmpString == "class" )
					{
						mStateStack.push(mCurState);
						mCurState	=	HEADER_CLASS;
						mTmpString.clear();
					}
					else if( mTmpString == "location" )
					{
						mStateStack.push(mCurState);
						mCurState	=	HEADER_LOCATION;
						mTmpString.clear();
					}
					else if( mTmpString == "object" )
					{
						mStateStack.push(mCurState);
						mCurState	=	HEADER_OBJECT;
						mTmpString.clear();
					}
				}
				break;
			}
		}
		break;
	case HEADER_VERSION:
		{
			std::string		dest;

			if( readHdrData(val, dest) )
			{
				mHdr.setVersion(dest.c_str());
			}
		}
		break;
	case HEADER_FORMAT:
		{
			std::string		dest;

			if( readHdrData(val, dest) )
			{
				mHdr.setFormat(dest.c_str());
			}
		}
		break;
	case HEADER_CLASS:
		{
			std::string		dest;

			if( readHdrData(val, dest) )
			{
				mHdr.setClass(dest.c_str());
			}
		}
		break;
	case HEADER_LOCATION:
		{
			std::string		dest;

			if( readHdrData(val, dest) )
			{
				mHdr.setLocation(dest.c_str());
			}
		}
		break;
	case HEADER_OBJECT:
		{
			std::string		dest;

			if( readHdrData(val, dest) )
			{
				mHdr.setObject(dest.c_str());
			}
		}
		break;
	case DATA:
		{
			switch( val )
			{
			case ')':
				{
					mCurState	=	mStateStack.top();
					mStateStack.pop();

					mReadDataEnd	=	true;
				}
				break;
			case '(':
				{
					mStateStack.push(mCurState);
					mCurState	=	DATA_READ;

					mElemLen	=	atol(mTmpString.c_str());

					while(!mDataQueue.empty())
					{
						mDataQueue.pop();
					}
					mTmpString.clear();
				}
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				mTmpString.clear();
				break;
			default:
				mToken.clear();
				mTmpString	+=	val;
				break;
			}
		}
		break;
	case DATA_READ:
		{
			switch( val )
			{
			case ')':
				{
					double		value	=	atol(mTmpString.c_str());
					mDataQueue.push(value);
					mTmpString.clear();

					mCurState	=	mStateStack.top();
					mStateStack.pop();

					mReadData	=	true;
				}
				break;
			case '(':
				{
				}
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				{
					double		value	=	atol(mTmpString.c_str());
					mDataQueue.push(value);
					mTmpString.clear();
				}
				break;
			default:
				mToken.clear();
				mTmpString	+=	val;
				break;
			}
		}
		break;
	}
}

size_t		OpenFOAM_Parser::readToBuf		(	void	)
{
	size_t	readSize	=	fread	(	mBuf,
										sizeof(mBuf[0]),
										mBufSize,
										mFoamFile
									);

	return	readSize;
}

void		OpenFOAM_Parser::readHdr			(	void	)
{
	while( !doSequence() )
	{
		if( mReadHdr )
		{
			break;
		}
	}
}

void		OpenFOAM_Parser::readDataStart	(	void	)
{
	while( !doSequence() )
	{
		if( mReadDataStart )
		{
			break;
		}
	}
}

bool		OpenFOAM_Parser::readData		(	void	)
{
	while( !doSequence() )
	{
		if( mReadData )
		{
			break;
		}
	}

	return	mReadDataEnd;
}

bool		OpenFOAM_Parser::readHdrData	(	char			val,
													std::string&	dest
												)
{
	bool	getFlag	=	false;
	switch( val )
	{
	case ';':
		{
			dest	=	mTmpString.c_str();
			mCurState	=	mStateStack.top();
			mStateStack.pop();
			getFlag		=	true;
		}
		break;
	case ' ':
	case '\t':
	case '\n':
	case '\r':
		mTmpString.clear();
		break;
	default:
		{
			mTmpString	+=	val;
		}
		break;
	}

	return	getFlag;
}

bool		OpenFOAM_Parser::isDigit		(	const char		val[]	)
{
	bool		digit	=	true;
	size_t		offset	=	0;

	while( val[offset] != '\0' )
	{
		if( (val[offset] < '0') ||
			('9' < val[offset]) )
		{
			digit	=	false;
			break;
		}
		offset++;
	}

	return	digit;
}

}
