/*
 * openfoam_parser.cpp
 *
 *  Created on: 2015. 2. 12.
 *      Author: asran
 */

#include "openfoam_parser.h"
#include "openfoam_error.h"
#include "openfoam_templet.h"
#include <stdio.h>
#include <stdlib.h>

#define	FILE_READ_FLAG	"r"
#define	FILE_WRITE_FLAG	"w"

#define	BUF_SIZE		(1024)

namespace openfoam
{

Parser::Parser	(	const char		fileName[],
						OpenType		type
					)
:mFile(NULL),
mBuf(BUF_SIZE),
mDataSize(0),
mDataOffset(0),
mBlanked(false),
mReadHdr(false),
mReadSize(false),
mReadData(false),
mDataDepth(0),
mCurState(NOTHING)
{
	open(fileName, type);
}

Parser::~Parser	(	void	)
{
	close();
}

void	Parser::open	(	const char		fileName[],
							OpenType		type
						)
{
	const char*	openType	=	NULL;

	if( mFile != NULL )
	{
		// 파일이 이미 열림
		throw	ErrMsg::createErrMsg("파일이 이미 열려있음");
	}

	openType	=	(type == Parser::FILE_READ)?
					(FILE_READ_FLAG):(FILE_WRITE_FLAG);

	mFile	=	fopen	(	fileName,
							openType
						);

	if( mFile == NULL )
	{
		// 파일 열기 에러
		throw	ErrMsg::createErrMsg("파일 열기 실패");
	}

	mOpenType	=	type;
	mFileName	=	fileName;
}

void	Parser::close	(	void	)
{
	fclose(mFile);
	mFile		=	NULL;
	mFileName	=	"";
}

void	Parser::readHdr	(	void	)
{
	while(!doHandle())
	{
		if( mReadHdr )
		{
			break;
		}
	}
}

void	Parser::readSize	(	void	)
{
	while(!doHandle())
	{
		if( mReadSize )
		{
			break;
		}
	}
}

bool	Parser::readData	(	void	)
{
	bool	finish		=	true;

	while(!doHandle())
	{
		if( mReadData )
		{
			finish		=	false;
			break;
		}
	}

	return	finish;
}

void	Parser::writeComment		(	void	)
{
	fprintf(mFile, OPENFOAM_TOP_COMMENT);
}

void	Parser::writeHdr			(	void	)
{
	fprintf(mFile, OPENFOAM_HDR_START);

	if( strcmp(mHdr.getVersion(), "") != 0 )
	{
		fprintf(mFile, HDR_FORMAT, VERSION, mHdr.getVersion());
	}

	if( strcmp(mHdr.getFormat(), "") != 0 )
	{
		fprintf(mFile, HDR_FORMAT, FORMAT, mHdr.getFormat());
	}

	if( strcmp(mHdr.getClass(), "") != 0 )
	{
		fprintf(mFile, HDR_FORMAT, CLASS, mHdr.getClass());
	}

	if( strcmp(mHdr.getNote(), "") != 0 )
	{
		fprintf(mFile, HDR_FORMAT, NOTE, mHdr.getNote());
	}

	if( strcmp(mHdr.getLocation(), "") != 0 )
	{
		fprintf(mFile, HDR_FORMAT, LOCAITON, mHdr.getLocation());
	}

	if( strcmp(mHdr.getObject(), "") != 0 )
	{
		fprintf(mFile, HDR_FORMAT, OBJECT, mHdr.getObject());
	}

	fprintf(mFile, OPENFOAM_HDR_END);
	fprintf(mFile, OPENFOAM_MID_SEPARATE"\n\n\n");
}

void	Parser::writeSize			(	size_t		size	)
{
	fprintf(mFile, "%lu\n", size);
}

void	Parser::writeDataStart	(	void	)
{
	fprintf(mFile, OPENFOAM_DATA_START"\n");
}

void	Parser::writeDataEnd		(	void	)
{
	fprintf(mFile, OPENFOAM_DATA_END"\n");

	fprintf(mFile, "\n\n");
	fprintf(mFile, OPENFOAM_LAST_SEPARATE"\n");
}

void	Parser::writeData			(	std::queue<int>		data,
										bool					showFlag
									)
{
	if( 1 < data.size() )
	{
		if( showFlag )
		{
			fprintf(mFile, "%lu", data.size());
		}
		fprintf(mFile, OPENFOAM_DATA_START);

		fprintf(mFile, "%d", data.front());
		data.pop();
		while(!data.empty())
		{
			fprintf(mFile, " %d", data.front());
			data.pop();
		}

		fprintf(mFile, OPENFOAM_DATA_END"\n");
	}
	else
	{
		fprintf(mFile, "%d\n", data.front());
	}
}

void	Parser::writeData			(	std::queue<double>	data,
										bool					showFlag
									)
{
	if( 1 < data.size() )
	{
		if( showFlag )
		{
			fprintf(mFile, "%lu", data.size());
		}
		fprintf(mFile, OPENFOAM_DATA_START);

		fprintf(mFile, "%g", data.front());
		data.pop();
		while(!data.empty())
		{
			fprintf(mFile, " %g", data.front());
			data.pop();
		}

		fprintf(mFile, OPENFOAM_DATA_END"\n");
	}
	else
	{
		fprintf(mFile, "%4.2lf\n", data.front());
	}
}

bool	Parser::doHandle	(	void	)
{
	bool	finish		=	true;

	if( mDataSize == mDataOffset )
	{
		readBuf();
	}

	if( mDataSize != 0 )
	{
		switch( mCurState )
		{
		case NOTHING:
			doNothing(mBuf[mDataOffset]);
			break;
		case COMMENT:
			doComment(mBuf[mDataOffset]);
			break;
		case CPP_COMMENT:
			doCppComment(mBuf[mDataOffset]);
			break;
		case HEADER:
			doHeader(mBuf[mDataOffset]);
			break;
		case DATA:
			doData(mBuf[mDataOffset]);
			break;
		case STRING:
			doString(mBuf[mDataOffset]);
			break;
		}

		++mDataOffset;
		finish		=	false;
	}

	return	finish;
}

void	Parser::readBuf	(	void	)
{
	mDataSize		=	fread	(	mBuf,
									sizeof(char),
									mBuf.getSize(),
									mFile
								);
	mDataOffset	=	0;
}

void	Parser::changeState	(	State		state
								)
{
	mStateStack.push(mCurState);
	mCurState	=	state;
}

void	Parser::returnState	(	void	)
{
	mCurState	=	mStateStack.top();
	mStateStack.pop();
}

void		Parser::doNothing		(	char	val		)
{
	do
	{
		// 공백류 문자는 무시
		if( isBlank(val) )
		{
			mBlanked	=	true;
			break;
		}

		switch( val )
		{
		// 주석 관련 키워드 처리
		case '/':
			{
				if( mToken == "/" )
				{
					// C++ 주석 상태로 변경
					changeState(CPP_COMMENT);

					mToken.clear();

					DEBUG_PRINT("C++ 주석 진입\n");
				}
				else
				{
					mToken	+=	val;
				}
			}
			break;
		case '*':
			{
				if( mToken == "/" )
				{
					// C 주석 상태로 변경
					changeState(COMMENT);

					mToken.clear();

					DEBUG_PRINT("C 주석 진입\n");
				}
			}
			break;
		// 헤더 관련 키워드 처리
		case '{':
			{
				if( mString == "FoamFile" )
				{

					changeState(HEADER);

					mToken.clear();
					mString.clear();

					mReadHdr	=	false;
					DEBUG_PRINT("헤더 진입\n");
				}
			}
			break;
		// 데이터 관련 키워드 처리
		case '(':
			{
				if( isDigit(mString.c_str()) )
				{
					changeState(DATA);

					mElemSize	=	atol(mString.c_str());
					mReadSize	=	true;

					++mDataDepth;

					while( !mStrQueue.empty() )
					{
						mStrQueue.pop();
					}

					mToken.clear();
					mString.clear();

					DEBUG_PRINT("데이터 진입 - %lu\n", mDataDepth);
				}
			}
			break;
		default:
			if( mBlanked )
			{
				if( mString != "" )
				{
					mStrQueue.push(mString);
					mString.clear();
				}

				mString	+=	val;
			}
			else
			{
				mString	+=	mToken;
				mString	+=	val;

				mToken.clear();
			}
			break;
		}

		mBlanked	=	false;

	}while(0);
}

void		Parser::doComment		(	char	val		)
{
	do
	{
		switch( val )
		{
		// 주석 관련 키워드 처리
		case '/':
			{
				if( mToken == "*" )
				{
					// 이전 상태로 복귀
					returnState();

					mToken.clear();

					DEBUG_PRINT("C 주석 복귀\n");
				}
			}
			break;
		case '*':
			{
				// 별이 두 개 이상 나오면 이전 내용은 버림
				if( mToken == "*" )
				{
					mToken.clear();
				}

				mToken	+=	val;
			}
			break;
		default:
			{
				mToken.clear();
			}
			break;
		}

		mBlanked	=	false;

	}while(0);
}

void		Parser::doCppComment	(	char	val		)
{
	do
	{
		switch( val )
		{
		// 주석 관련 키워드 처리
		case '\r':
		case '\n':
			{
				// 이전 상태로 복귀
				returnState();

				mToken.clear();

				DEBUG_PRINT("C++ 주석 복귀\n");
			}
			break;
		default:
			{
				mToken.clear();
			}
			break;
		}

		mBlanked	=	false;

	}while(0);
}

void		Parser::doHeader		(	char	val		)
{
	do
	{
		// 공백류 문자는 무시
		if( isBlank(val) )
		{
			mBlanked	=	true;
			break;
		}

		switch( val )
		{
		case '}':
			{
				// 이전 상태로 복귀
				returnState();

				mReadHdr	=	true;
				DEBUG_PRINT("헤더 복귀\n");
			}
			break;
		// 문자열 처리
		case '\"':
			{
				if( mBlanked )
				{
					if( mString != "" )
					{
						mStrQueue.push(mString);
						mString.clear();
					}
				}

				DEBUG_PRINT("문자열 시작\n");
				changeState(STRING);
				mString	+=	'\"';
			}
			break;
		case ';':
			{
				mStrQueue.push(mString);
				mString.clear();

				if( mStrQueue.front() == "version" )
				{
					mStrQueue.pop();
					mHdr.setVersion(mStrQueue.front().c_str());
					mStrQueue.pop();
				}
				else if( mStrQueue.front() == "format" )
				{
					mStrQueue.pop();
					mHdr.setFormat(mStrQueue.front().c_str());
					mStrQueue.pop();
				}
				else if( mStrQueue.front() == "class" )
				{
					mStrQueue.pop();
					mHdr.setClass(mStrQueue.front().c_str());
					mStrQueue.pop();
				}
				else if( mStrQueue.front() == "note" )
				{
					mStrQueue.pop();
					mHdr.setNote(mStrQueue.front().c_str());
					mStrQueue.pop();
				}
				else if( mStrQueue.front() == "location" )
				{
					mStrQueue.pop();
					mHdr.setLocation(mStrQueue.front().c_str());
					mStrQueue.pop();
				}
				else if( mStrQueue.front() == "object" )
				{
					mStrQueue.pop();
					mHdr.setObject(mStrQueue.front().c_str());
					mStrQueue.pop();
				}
				else
				{
					mStrQueue.pop();
					mStrQueue.pop();
				}
			}
			break;
		default:
			if( mBlanked )
			{
				if( mString != "" )
				{
					mStrQueue.push(mString);
					mString.clear();
				}
				mString	+=	val;
			}
			else
			{
				mString	+=	mToken;
				mString	+=	val;

				mToken.clear();
			}
			break;
		}

		mBlanked	=	false;

	}while(0);
}

void		Parser::doData		(	char	val		)
{
	do
	{
		// 공백류 문자는 무시
		if( isBlank(val) )
		{
			mBlanked	=	true;
			break;
		}

		switch( val )
		{
		// 주석 관련 키워드 처리
		case '/':
			{
				if( mToken == "/" )
				{
					// C++ 주석 상태로 변경
					changeState(CPP_COMMENT);

					mToken.clear();

					DEBUG_PRINT("C++ 주석 진입\n");
				}
				else
				{
					mToken	+=	val;
				}
			}
			break;
		case '*':
			{
				if( mToken == "/" )
				{
					// C 주석 상태로 변경
					changeState(COMMENT);

					mToken.clear();

					DEBUG_PRINT("C 주석 진입\n");
				}
			}
			break;
		// 헤더 관련 키워드 처리
		case '{':
			break;
		// 데이터 관련 키워드 처리
		case '(':
			{
				//if( isDigit(mString.c_str()) )
				//{
				//	mElemSize	=	atol(mString.c_str());
				//}

				changeState(DATA);

				mToken.clear();
				mString.clear();

				++mDataDepth;
				DEBUG_PRINT("데이터 진입 - %lu\n", mDataDepth);
			}
			break;
		case ')':
			{
				if( mString != "" )
				{
					mStrQueue.push(mString);
					mString.clear();
				}

				// 이전 데이터는 비움
				while( !mDataQueue.empty() )
				{
					mDataQueue.pop();
				}

				// 문자열 저장 큐에서 데이터 큐로 형 변환하여 적재
				while( !mStrQueue.empty() )
				{
					const char*	str		=	mStrQueue.front().c_str();
					double	num		=	atof(str);

					mStrQueue.pop();

					mDataQueue.push(num);
				}

				if( !mDataQueue.empty() )
				{
					mReadData	=	true;
				}

				DEBUG_PRINT("데이터 복귀 - %lu\n", mDataDepth);
				--mDataDepth;

				returnState();
			}
			break;
		default:
			if( mBlanked )
			{
				if( mString != "" )
				{
					mStrQueue.push(mString);
					mString.clear();
				}

				mString	+=	val;
			}
			else
			{
				mString	+=	mToken;
				mString	+=	val;

				mToken.clear();
			}
			break;
		}

		mBlanked	=	false;

	}while(0);
}

void		Parser::doString		(	char	val		)
{
	do
	{
		switch( val )
		{
		case '\"':
			mString	+=	'\"';
			returnState();
			DEBUG_PRINT("문자열 종료\n");
			break;
		default:
			mString	+=	val;
			break;
		}
	}while(0);
}

bool	Parser::isDigit	(	const char		str[]
							)
{
	bool	digit	=	true;
	size_t	offset	=	0;

	while(str[offset] != '\0')
	{
		if(isDigit(str[offset]) == false )
		{
			digit	=	false;
			break;
		}
		++offset;
	}

	return	digit;
}

}
