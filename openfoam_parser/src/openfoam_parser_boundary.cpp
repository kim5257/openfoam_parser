/*
 * openfoam_parser_boundary.cpp
 *
 *  Created on: 2015. 7. 22.
 *      Author: asran
 */

#include "openfoam_parser_boundary.h"
#include "openfoam_error.h"

namespace openfoam
{

Parser_boundary::Parser_boundary		(	void	)
:Parser(),
 mInfoData("")
{

}

Parser_boundary::Parser_boundary		(	const char		fileName[],
												OpenType		type
											)
:Parser(fileName, type),
 mInfoData("")
{

}

Parser_boundary::~Parser_boundary		(	void	)
{

}

void	Parser_boundary::writeData	(	InfoData	info	)
{
	std::string	hdr		=	info.getHdr();
	std::string	tmpStr;

	if( !isOpen() )
	{
		// 파일 열기 하기 않음
		throw	ErrMsg::createErrMsg("파일을 열기 하지 않음");
	}

	fprintf(getFP(), "    %s\n    {\n", hdr.c_str());

	tmpStr	=	info.getData("type");
	fprintf(getFP(), "        %-15s %s;\n", "type", tmpStr.c_str());
	tmpStr	=	info.getData("nFaces");
	fprintf(getFP(), "        %-15s %s;\n", "nFaces", tmpStr.c_str());
	tmpStr	=	info.getData("startFace");
	fprintf(getFP(), "        %-15s %s;\n", "startFace", tmpStr.c_str());

	fprintf(getFP(), "    }\n");
}

void	Parser_boundary::doData		(	char	val		)
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
		// Info 데이터 관련 처리
		case '{':
			{
				mInfoData	=	InfoData(mString);
				mString.clear();

				changeState(DATA);

				++mDataDepth;
			}
			break;
		case '}':
			{
				mReadData	=	true;

				--mDataDepth;
				returnState();
			}
			break;
		case ';':
			{
				std::string	name;
				std::string	value;

				mStrQueue.push(mString);
				mString.clear();

				name	=	mStrQueue.front();
				mStrQueue.pop();
				value		=	mStrQueue.front();
				mStrQueue.pop();

				mInfoData.setData(name, value);
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

}



