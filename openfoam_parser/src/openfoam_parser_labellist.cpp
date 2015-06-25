/*
 * openfoam_parser_labellist.cpp
 *
 *  Created on: 2015. 2. 14.
 *      Author: asran
 */

#include "openfoam_parser_labellist.h"

namespace openfoam
{

Parser_labelList::Parser_labelList		(	const char		fileName[]		)
:Parser(fileName)
{

}

Parser_labelList::~Parser_labelList		(	void	)
{

}

void	Parser_labelList::doData		(	char	val		)
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
		default:
			if( mBlanked )
			{
				while( !mDataQueue.empty() )
				{
					mDataQueue.pop();
				}

				if( mString != "" )
				{
					//mStrQueue.push(mString);
					const char* str	=	mString.c_str();
					mDataQueue.push(atof(str));
					mString.clear();
				}

				if( !mDataQueue.empty() )
				{
					mReadData	=	true;
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

}
