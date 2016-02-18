/*
 * scalarfields.cpp
 *
 *  Created on: 2016. 2. 17.
 *      Author: arun
 */

#include "scalarfields.h"
#include <iostream>

namespace	openfoam
{

Scalarfields::Scalarfields	(	void	)
:mParser()
{

}

Scalarfields::Scalarfields	(	const char			fileName[],
									Parser::OpenType	type
								)
:mParser(fileName, type)
{

}

Scalarfields::~Scalarfields	(	void	)
{

}

void		Scalarfields::open	(	const char			fileName[],
										Parser::OpenType	type
									)
{
	mParser.open(fileName, type);
}

void		Scalarfields::close		(	void	)
{
	mParser.close();
}

void	Scalarfields::readFile	(	void	)
{
	// 헤더 정보 읽기
	mParser.readHdr();

	// 데이터 갯수 읽기
	//mParser.readSize();

	// 데이터 갯수 만큼 반복
	while(1)
	{
		ScalarField	scalarField;

		// 한 개 point 값 읽기
		if( mParser.readData() )
		{
			break;
		}

		// 읽은 point 값 가져오기
		InfoData		data		=	mParser.getData();

		std::string	tmpStr;

		scalarField.mName	=	data.getHdr();
		scalarField.mType	=	data.getSingleData("type");

		if( data.isExistData("value") )
		{
			std::vector<std::string>	 rawData = data.getData("value");

			for(size_t cnt=0;cnt<rawData.size();++cnt)
			{
				scalarField.mValue.push_back(atof(rawData[cnt].c_str()));
			}
		}

		// Boundary 값을 Boundaries 자료구조에 적재
		mData.push_back(scalarField);
	}
}

void	Scalarfields::writeFile	(	void	)
{
	mParser.writeComment();
	mParser.writeHdr();

	mParser.writeDimensions();
	mParser.writeIntalField();

	mParser.writeDataStart();

	for(size_t cnt=0;cnt<mData.size();++cnt)
	{
		ScalarField	scalarField	=	getData(cnt);
		InfoData		infoData		=	InfoData(scalarField.mName);
		char			tmpStr[5]		=	{0,};

		infoData.setSingleData("type", scalarField.mType);

		if( scalarField.mValue.size() != 0 )
		{
			sprintf(tmpStr, "%.0lf", scalarField.mValue[0]);
			infoData.setSingleData("value", tmpStr);
		}

		mParser.writeData(infoData);
	}

	mParser.writeDataEnd();
}


ScalarField&	Scalarfields::getData	(	size_t		index	)
{
	return	mData[index];
}

void	Scalarfields::copy		(	const Scalarfields&		scalarfields	)
{
	if( mParser.getOpenType() != Parser::FILE_WRITE )
	{
		throw	ErrMsg::createErrMsg("파서가 파일 쓰기 형식으로 열리지 않았습니다.");
	}

	if( scalarfields.mParser.getOpenType() != Parser::FILE_READ )
	{
		throw	ErrMsg::createErrMsg("복사 할 파서가 파일 읽기 형식으로 열리지 않았습니다.");
	}

	mData	=	scalarfields.mData;
	mParser.setDimensions(scalarfields.mParser.getDimensions());
	mParser.setInternalField(scalarfields.mParser.getInternalField());
	mParser.setHdr(scalarfields.mParser.getHdr());
}

}
