/*
 * faces.cpp
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#include "faces.h"

namespace	openfoam
{

Faces::Faces	(	void	)
:mParser()
{

}

Faces::Faces	(	const char			fileName[],
					Parser::OpenType	type
				)
:mParser(fileName, type)
{

}

Faces::~Faces	(	void	)
{

}

void		Faces::open		(	const char			fileName[],
									Parser::OpenType	type
								)
{
	mParser.open(fileName, type);
}

void		Faces::close		(	void	)
{
	mParser.close();
}

void		Faces::readFile	(	void	)
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
			face.mPoints.push_back(data.front());
			data.pop();
			offset++;
		}

		// point 값을 points 자료구조에 적재
		mData.push_back(face);
		data.pop();
	}
}

void		Faces::writeFile	(	void	)
{
	mParser.writeComment();
	mParser.writeHdr();

	mParser.writeSize(mData.size());
	mParser.writeDataStart();

	for(size_t cnt=0;cnt<mData.size();++cnt)
	{
		Face	face	=	getData(cnt);
		std::queue<int>	data;

		for(size_t cnt2=0;cnt2<face.mPoints.size();++cnt2)
		{
			data.push(face.mPoints[cnt2]);
		}
		mParser.writeData(data, true);
	}

	mParser.writeDataEnd();
}


Face&		Faces::getData	(	size_t		index	)
{
	return	mData[index];
}

void		Faces::copy		(	const Faces&	faces	)
{
	if( mParser.getOpenType() != Parser::FILE_WRITE )
	{
		throw	ErrMsg::createErrMsg("파서가 파일 쓰기 형식으로 열리지 않았습니다.");
	}

	if( faces.mParser.getOpenType() != Parser::FILE_READ )
	{
		throw	ErrMsg::createErrMsg("복사 할 파서가 파일 읽기 형식으로 열리지 않았습니다.");
	}

	mData	=	faces.mData;
	mParser.setHdr(faces.mParser.getHdr());
}


}
