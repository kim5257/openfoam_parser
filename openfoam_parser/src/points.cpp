/*
 * points.cpp
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#include "points.h"

namespace	openfoam
{

Points::Points	(	const char		fileName[]		)
:mParser(fileName)
{

}

Points::~Points	(	void	)
{

}

void		Points::prepare	(	void	)
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
		point.mX	=	(uint32_t)data.front();
		data.pop();
		point.mY	=	(uint32_t)data.front();
		data.pop();
		point.mZ	=	(uint32_t)data.front();
		data.pop();

		// point 값을 points 자료구조에 적재
		mData.push_back(point);
		data.pop();
	}
}

Point&		Points::getData	(	size_t		index	)
{
	return	mData[index];
}

}
