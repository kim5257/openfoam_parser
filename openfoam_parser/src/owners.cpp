/*
 * owners.cpp
 *
 *  Created on: 2015. 6. 17.
 *      Author: asran
 */

#include "owners.h"

namespace	openfoam
{

Owners::Owners	(	void	)
:mParser()
{

}

Owners::Owners	(	const char			fileName[],
						Parser::OpenType	type
					)
:mParser(fileName, type)
{

}

Owners::~Owners	(	void	)
{

}

void		Owners::open		(	const char			fileName[],
									Parser::OpenType	type
								)
{
	mParser.open(fileName, type);
}

void		Owners::close		(	void	)
{
	mParser.close();
}

void		Owners::readFile	(	void	)
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

void		Owners::writeFile	(	void	)
{
	mParser.writeComment();
	mParser.writeHdr();

	mParser.writeSize(mData.size());
	mParser.writeDataStart();

	for(size_t cnt=0;cnt<mData.size();++cnt)
	{
		Owner	owner	=	getData(cnt);
		std::queue<int>	data;

		data.push(owner.mCell);
		mParser.writeData(data, false);
	}

	mParser.writeDataEnd();
}

Owner&		Owners::getData	(	size_t			index	)
{
	return	mData[index];
}

void		Owners::copy		(	const Owners&	owners	)
{
	if( mParser.getOpenType() != Parser::FILE_WRITE )
	{
		throw	ErrMsg::createErrMsg("파서가 파일 쓰기 형식으로 열리지 않았습니다.");
	}

	if( owners.mParser.getOpenType() != Parser::FILE_READ )
	{
		throw	ErrMsg::createErrMsg("복사 할 파서가 파일 읽기 형식으로 열리지 않았습니다.");
	}

	mData	=	owners.mData;
	mParser.setHdr(owners.mParser.getHdr());
}

};
