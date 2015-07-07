/*
 * points.cpp
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#include "points.h"

namespace	openfoam
{

Points::Points	(	const char			fileName[],
						Parser::OpenType	type
					)
:mParser(fileName, type)
{

}

Points::~Points	(	void	)
{

}

void		Points::readFile	(	void	)
{
	// 헤더 정보 읽기
	mParser.readHdr();

	// 데이터 갯수 읽기
	mParser.readSize();

	// 데이터 갯수 만큼 반복
	for(size_t cnt=0;cnt<mParser.getSize();++cnt)
	{
		// point 값 저장 할 구조체
		Point	point;

		// 한 개 point 값 읽기
		if( mParser.readData() )
		{
			break;
		}

		// 읽은 point 값 가져오기
		std::queue<double>	data	=	mParser.getData();

		// point 값을 구조체에 넣기
		point.mX	=	data.front();
		data.pop();
		point.mY	=	data.front();
		data.pop();
		point.mZ	=	data.front();
		data.pop();

		// point 값을 points 자료구조에 적재
		mData.push_back(point);
		data.pop();
	}
}

void		Points::writeFile	(	void	)
{
	mParser.writeComment();
	mParser.writeHdr();

	mParser.writeSize(mData.size());
	mParser.writeDataStart();

	for(size_t cnt=0;cnt<mData.size();++cnt)
	{
		Point	point	=	getData(cnt);
		std::queue<double>	data;

		data.push(point.mX);
		data.push(point.mY);
		data.push(point.mZ);
		mParser.writeData(data, false);
	}

	mParser.writeDataEnd();
}

void		Points::copy		(	const Points&	points	)
{
	if( mParser.getOpenType() != Parser::FILE_WRITE )
	{
		throw	ErrMsg::createErrMsg("파서가 파일 쓰기 형식으로 열리지 않았습니다.");
	}

	if( points.mParser.getOpenType() != Parser::FILE_READ )
	{
		throw	ErrMsg::createErrMsg("복사 할 파서가 파일 읽기 형식으로 열리지 않았습니다.");
	}

	mData	=	points.mData;
	mParser.setHdr(points.mParser.getHdr());
}

Point&		Points::getData	(	size_t		index	)
{
	return	mData[index];
}

}
