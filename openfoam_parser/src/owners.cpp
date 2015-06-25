/*
 * owners.cpp
 *
 *  Created on: 2015. 6. 17.
 *      Author: asran
 */

#include "owners.h"

namespace	openfoam
{

Owners::Owners	(	const char		fileName[]		)
:mParser(fileName)
{

}

Owners::~Owners	(	void	)
{

}

void		Owners::prepare	(	void	)
{
	// 헤더 정보 읽기
	mParser.readHdr();

	// 데이터 갯수 읽기
	mParser.readSize();

	// 데이터 갯수 만큼 반복
	for(size_t cnt=0;cnt<mParser.getSize();++cnt)
	{
		// owner 값 저장 할 구조체
		Owner	owner;

		// 한 개 owner 값 읽기
		if( mParser.readData() )
		{
			break;
		}

		// 읽은 owner 값 가져오기
		std::queue<double>	data	=	mParser.getData();

		// owner 값을 구조체에 넣기
		owner.mCell	=	(uint32_t)data.front();

		// owner 값을 owners 자료구조에 적재
		mData.push_back(owner);
		data.pop();
	}
}

Owner&		Owners::getData	(	size_t		index	)
{
	return	mData[index];
}

};
