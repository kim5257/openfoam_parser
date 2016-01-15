/*
 * openfoam_parser_boundary.cpp
 *
 *  Created on: 2015. 7. 22.
 *      Author: asran
 */

#include "openfoam_parser_dictionary.h"
#include "openfoam_error.h"

namespace openfoam
{

Parser_dictionary::Parser_dictionary		(	void	)
:Parser(),
 mInfoData("")
{

}

Parser_dictionary::Parser_dictionary		(	const char		fileName[],
												OpenType		type
											)
:Parser(fileName, type),
 mInfoData("")
{

}

Parser_dictionary::~Parser_dictionary		(	void	)
{

}

void	Parser_dictionary::writeData	(	InfoData	info	)
{
	std::string	hdr		=	info.getHdr();
	std::string	tmpStr;
	std::vector<std::string> tmpStrList;

	if( !isOpen() )
	{
		// 파일 열기 하기 않음
		throw	ErrMsg::createErrMsg("파일을 열기 하지 않음");
	}

	fprintf(getFP(), "%s\n{\n", hdr.c_str());

	tmpStr	=	info.getSingleData("type");
	fprintf(getFP(), "    %-15s %s;\n", "type", tmpStr.c_str());

	tmpStrList = info.getData("cellLabels");

	fprintf(getFP(), "    %-15s %d ( ", "cellLabels", tmpStrList.size());
	for(int cnt=0;cnt<tmpStrList.size();++cnt)
	{
		fprintf(getFP(), "%s ", tmpStrList[cnt].c_str());
	}
	fprintf(getFP(), ");\n}\n\n");
}

void	Parser_dictionary::doData		(	char	val		)
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

				mStrQueue.push(mString);
				mString.clear();

				name	=	mStrQueue.front();
				mStrQueue.pop();

				if( name == "cellLabels" )
				{
					size_t size = atoi(mStrQueue.front().c_str());
					mStrQueue.pop();
					std::vector<std::string> values;


					// 소괄호 빼기
					mStrQueue.pop();

					for(size_t cnt=0;cnt<size;++cnt)
					{
						// 데이터 하나씩 넣기
						values.push_back(mStrQueue.front());
						mStrQueue.pop();
					}

					mInfoData.setData(name, values);

					mStrQueue.pop();
				}
				else
				{
					std::string	value;

					value		=	mStrQueue.front();
					mStrQueue.pop();

					mInfoData.setSingleData(name, value);
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



