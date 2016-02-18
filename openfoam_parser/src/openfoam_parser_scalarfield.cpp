/*
 * openfoam_parser_scalarfield.cpp
 *
 *  Created on: 2016. 2. 17.
 *      Author: arun
 */

#include "openfoam_parser_scalarfield.h"
#include "openfoam_templet.h"
#include "openfoam_error.h"
#include <stdio.h>

namespace openfoam
{

Parser_scalarfield::Parser_scalarfield		(	void	)
:Parser(),
 mInfoData("")
{

}

Parser_scalarfield::Parser_scalarfield		(	const char		fileName[],
													OpenType		type
												)
:Parser(fileName, type),
 mInfoData("")
{

}

Parser_scalarfield::~Parser_scalarfield	(	void	)
{

}

void	Parser_scalarfield::writeDataStart	(	void	)
{
	if( !isOpen() )
	{
		// 파일 열기 하기 않음
		throw	ErrMsg::createErrMsg("파일을 열기 하지 않음");
	}

	fprintf(getFP(),	"boundaryField\n"
						"{\n");
}

void	Parser_scalarfield::writeDataEnd	(	void	)
{
	if( !isOpen() )
	{
		// 파일 열기 하기 않음
		throw	ErrMsg::createErrMsg("파일을 열기 하지 않음");
	}

	fprintf(getFP(), "}\n");

	fprintf(getFP(), "\n\n");
	fprintf(getFP(), OPENFOAM_LAST_SEPARATE"\n");
}

void	Parser_scalarfield::writeData	(	InfoData	info	)
{
	std::string	hdr		=	info.getHdr();
	std::string	tmpStr;

	if( !isOpen() )
	{
		// 파일 열기 하기 않음
		throw	ErrMsg::createErrMsg("파일을 열기 하지 않음");
	}

	fprintf(getFP(), "    %s\n    {\n", hdr.c_str());

	tmpStr	=	info.getSingleData("type");
	fprintf(getFP(), "        %-15s %s;\n", "type", tmpStr.c_str());

	if( info.isExistData("value") )
	{
		tmpStr	=	info.getSingleData("value");
		fprintf(getFP(), "        %-15s %s %s;\n", "value", "uniform", tmpStr.c_str());
	}

	fprintf(getFP(), "    }\n", hdr.c_str());
}

void	Parser_scalarfield::doNothing	(	char	val		)
{
	do
	{
		// 공백류 문자는 무시
		if( isBlank(val) )
		{
			mBlanked	=	true;
			//break;
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
		// 및 데이터 관련 키워드 처리
		case '{':
			{
				std::string str = mStrQueue.front();
				if( str == "FoamFile" )
				{
					changeState(HEADER);

					mToken.clear();
					mString.clear();

					mReadHdr	=	false;
					DEBUG_PRINT("헤더 진입\n");
				}
				else if( str == "boundaryField" )
				{
					changeState(DATA);

					mToken.clear();
					mString.clear();

					DEBUG_PRINT("데이터 진입 - %lu\n", mDataDepth);
				}

				mStrQueue.pop();
			}
			break;
		default:
			if( mBlanked )
			{
				if( mString == "dimensions" )
				{
					changeState(DIMENSIONS);
					mString.clear();
				}
				else if( mString == "internalField" )
				{
					changeState(INTERNALFIELD);
					mString.clear();
				}
				else if( mString != "" )
				{
					mStrQueue.push(mString);
					mString.clear();
				}

				//mString	+=	val;
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

void	Parser_scalarfield::doData		(	char	val		)
{
	do
	{
		// 공백류 문자는 무시
		if( isBlank(val) )
		{
			mBlanked	=	true;
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

				if( 0 < mDataDepth )
				{
					InfoData	info(mStrQueue.front());
					mStrQueue.pop();

					mInfoData = info;
				}
			}
			break;
		case '}':
			{
				if( 0 < mDataDepth )
				{
					mReadData	=	true;
				}

				--mDataDepth;
				returnState();
			}
			break;
		case ';':
			{
				std::string	name;

				mStrQueue.push(mString);
				mString.clear();


				name	=	mStrQueue.front();
				mStrQueue.pop();

				if( name == "type" )
				{
					std::string	value;
					value		=	mStrQueue.front();
					mStrQueue.pop();

					mInfoData.setSingleData(name, value);
				}
				else if( name == "value" )
				{
					std::string	type;

					type		=	mStrQueue.front();
					mStrQueue.pop();

					if( type == "uniform" )
					{
						std::string	value;

						value		=	mStrQueue.front();

						mStrQueue.pop();

						mInfoData.setSingleData(name, value);
					}
				}
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



