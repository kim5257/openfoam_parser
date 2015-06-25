/*
 * faces.cpp
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#include "faces.h"

namespace	openfoam
{

Faces::Faces	(	const char		fileName[]		)
:mParser(fileName)
{

}

Faces::~Faces	(	void	)
{

}

void		Faces::prepare	(	void	)
{
	// 헤더 정보 읽기
	mParser.readHdr();

	// 데이터 갯수 읽기
	mParser.readSize();

	// 데이터 갯수 만큼 반복
	for(size_t cnt=0;cnt<mParser.getSize();++cnt)
	{
		// point 값 저장 할 구조체
		Face	face;

		// 한 개 point 값 읽기
		if( mParser.readData() )
		{
			break;
		}

		// 읽은 point 값 가져오기
		std::queue<double>	data	=	mParser.getData();
		size_t	offset	=	0;

		while( !data.empty() )
		{
			face.mPoint[offset]	=	data.front();
			data.pop();
			offset++;
		}

		// point 값을 points 자료구조에 적재
		mData.push_back(face);
		data.pop();
	}
}

Face&		Faces::getData	(	size_t		index	)
{
	return	mData[index];
}

}
