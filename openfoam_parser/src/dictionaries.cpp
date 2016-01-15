/*
 * boundaries.cpp
 *
 *  Created on: 2015. 7. 23.
 *      Author: asran
 */

#include "dictionaries.h"
#include <iostream>

namespace	openfoam
{

Dictionaries::Dictionaries	(	void	)
:mParser()
{

}

Dictionaries::Dictionaries	(	const char			fileName[],
									Parser::OpenType	type
								)
:mParser(fileName, type)
{

}

Dictionaries::~Dictionaries	(	void	)
{

}

void		Dictionaries::open		(	const char			fileName[],
											Parser::OpenType	type
										)
{
	mParser.open(fileName, type);
}

void		Dictionaries::close		(	void	)
{
	mParser.close();
}

void	Dictionaries::readFile	(	void	)
{
	// 헤더 정보 읽기
	mParser.readHdr();

	// 데이터 갯수 읽기
	mParser.readSize();

	// 데이터 갯수 만큼 반복
	for(size_t cnt=0;cnt<mParser.getSize();++cnt)
	{
		Dictionary		dictionary;

		// 한 개 point 값 읽기
		if( mParser.readData() )
		{
			break;
		}

		// 읽은 point 값 가져오기
		InfoData		data		=	mParser.getData();

		std::string	tmpStr;

		dictionary.mName	=	data.getHdr();
		dictionary.mType	=	data.getSingleData("type");

		std::vector<std::string> values = data.getData("cellLabels");
		for(size_t cnt2=0;cnt2<values.size();++cnt2)
		{
			dictionary.mCellLabels.push_back(atoi(values[cnt2].c_str()));
		}

		// Boundary 값을 Boundaries 자료구조에 적재
		mData.push_back(dictionary);
	}
}

void	Dictionaries::writeFile	(	void	)
{
	mParser.writeComment();
	mParser.writeHdr();

	mParser.writeSize(mData.size());
	mParser.writeDataStart();

	for(size_t cnt=0;cnt<mData.size();++cnt)
	{
		Dictionary		boundary	=	getData(cnt);
		InfoData	infoData	=	InfoData(boundary.mName);
		char		tmpStr[10]	=	{0,};
		std::vector<std::string> dataList;

		infoData.setSingleData("type", boundary.mType);

		for(size_t cnt2=0;cnt2<boundary.mCellLabels.size();++cnt2)
		{
			sprintf(tmpStr, "%d", boundary.mCellLabels[cnt2]);
			dataList.push_back(tmpStr);
		}
		infoData.setData("cellLabels", dataList);

		mParser.writeData(infoData);
	}

	mParser.writeDataEnd();
}


Dictionary&	Dictionaries::getData	(	size_t		index	)
{
	return	mData[index];
}

void	Dictionaries::copy		(	const Dictionaries&		dictionaries	)
{
	if( mParser.getOpenType() != Parser::FILE_WRITE )
	{
		throw	ErrMsg::createErrMsg("파서가 파일 쓰기 형식으로 열리지 않았습니다.");
	}

	if( dictionaries.mParser.getOpenType() != Parser::FILE_READ )
	{
		throw	ErrMsg::createErrMsg("복사 할 파서가 파일 읽기 형식으로 열리지 않았습니다.");
	}

	mData	=	dictionaries.mData;
	mParser.setHdr(dictionaries.mParser.getHdr());
}


}
