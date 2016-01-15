/*
 * boundaries.cpp
 *
 *  Created on: 2015. 7. 23.
 *      Author: asran
 */

#include "boundaries.h"
#include <iostream>

namespace	openfoam
{

Boundaries::Boundaries	(	void	)
:mParser()
{

}

Boundaries::Boundaries	(	const char			fileName[],
								Parser::OpenType	type
							)
:mParser(fileName, type)
{

}

Boundaries::~Boundaries	(	void	)
{

}

void		Boundaries::open		(	const char			fileName[],
										Parser::OpenType	type
									)
{
	mParser.open(fileName, type);
}

void		Boundaries::close		(	void	)
{
	mParser.close();
}

void	Boundaries::readFile	(	void	)
{
	// 헤더 정보 읽기
	mParser.readHdr();

	// 데이터 갯수 읽기
	mParser.readSize();

	// 데이터 갯수 만큼 반복
	for(size_t cnt=0;cnt<mParser.getSize();++cnt)
	{
		Boundary		boundary;

		// 한 개 point 값 읽기
		if( mParser.readData() )
		{
			break;
		}

		// 읽은 point 값 가져오기
		InfoData		data		=	mParser.getData();

		std::string	tmpStr;

		boundary.mName	=	data.getHdr();
		boundary.mType	=	data.getSingleData("type");

		tmpStr		=	data.getSingleData("nFaces");
		boundary.mFaces	=	atol(tmpStr.c_str());
		tmpStr		=	data.getSingleData("startFace");
		boundary.mStartFace	=	atol(tmpStr.c_str());

		// Boundary 값을 Boundaries 자료구조에 적재
		mData.push_back(boundary);
	}
}

void	Boundaries::writeFile	(	void	)
{
	mParser.writeComment();
	mParser.writeHdr();

	mParser.writeSize(mData.size());
	mParser.writeDataStart();

	for(size_t cnt=0;cnt<mData.size();++cnt)
	{
		Boundary	boundary	=	getData(cnt);
		InfoData	infoData	=	InfoData(boundary.mName);
		char		tmpStr[5]	=	{0,};

		infoData.setSingleData("type", boundary.mType);

		sprintf(tmpStr, "%d", boundary.mFaces);
		infoData.setSingleData("nFaces", tmpStr);

		sprintf(tmpStr, "%d", boundary.mStartFace);
		infoData.setSingleData("startFace", tmpStr);

		mParser.writeData(infoData);
	}

	mParser.writeDataEnd();
}


Boundary&	Boundaries::getData	(	size_t		index	)
{
	return	mData[index];
}

void	Boundaries::copy		(	const Boundaries&		boundaries	)
{
	if( mParser.getOpenType() != Parser::FILE_WRITE )
	{
		throw	ErrMsg::createErrMsg("파서가 파일 쓰기 형식으로 열리지 않았습니다.");
	}

	if( boundaries.mParser.getOpenType() != Parser::FILE_READ )
	{
		throw	ErrMsg::createErrMsg("복사 할 파서가 파일 읽기 형식으로 열리지 않았습니다.");
	}

	mData	=	boundaries.mData;
	mParser.setHdr(boundaries.mParser.getHdr());
}


}
