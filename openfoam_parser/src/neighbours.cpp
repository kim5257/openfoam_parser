/*
 * neighbours.cpp
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#include "neighbours.h"

namespace	openfoam
{

Neighbours::Neighbours	(	void	)
:mParser()
{

}

Neighbours::Neighbours	(	const char			fileName[],
								Parser::OpenType	type
							)
:mParser(fileName, type)
{

}

Neighbours::~Neighbours	(	void	)
{

}

void			Neighbours::open			(	const char			fileName[],
												Parser::OpenType	type
											)
{
	mParser.open(fileName, type);
}

void			Neighbours::close			(	void	)
{
	mParser.close();
}

void			Neighbours::readFile		(	void	)
{
	// 헤더 정보 읽기
	mParser.readHdr();

	// 데이터 갯수 읽기
	mParser.readSize();

	// 데이터 갯수 만큼 반복
	for(size_t cnt=0;cnt<mParser.getSize();++cnt)
	{
		// owner 값 저장 할 구조체
		Neighbour	neighbour;

		// 한 개 owner 값 읽기
		if( mParser.readData() )
		{
			break;
		}

		// 읽은 owner 값 가져오기
		std::queue<double>	data	=	mParser.getData();

		// owner 값을 구조체에 넣기
		neighbour.mCell	=	(uint32_t)data.front();

		// owner 값을 owners 자료구조에 적재
		mData.push_back(neighbour);
		data.pop();
	}
}

void			Neighbours::writeFile	(	void	)
{
	mParser.writeComment();
	mParser.writeHdr();

	mParser.writeSize(mData.size());
	mParser.writeDataStart();

	for(size_t cnt=0;cnt<mData.size();++cnt)
	{
		Neighbour	neighbour	=	getData(cnt);
		std::queue<int>	data;

		data.push(neighbour.mCell);
		mParser.writeData(data, false);
	}

	mParser.writeDataEnd();
}

Neighbour&		Neighbours::getData		(	size_t		index	)
{
	return	mData[index];
}

void			Neighbours::copy			(	const Neighbours&	neighbours	)
{
	if( mParser.getOpenType() != Parser::FILE_WRITE )
	{
		throw	ErrMsg::createErrMsg("파서가 파일 쓰기 형식으로 열리지 않았습니다.");
	}

	if( neighbours.mParser.getOpenType() != Parser::FILE_READ )
	{
		throw	ErrMsg::createErrMsg("복사 할 파서가 파일 읽기 형식으로 열리지 않았습니다.");
	}

	mData	=	neighbours.mData;
	mParser.setHdr(neighbours.mParser.getHdr());
}

};
