/*
 * openfoam_error.h
 *
 *  Created on: 2015. 02. 11.
 *      Author: asran
 */

#ifndef OPENFOAM_ERROR_H_
#define OPENFOAM_ERROR_H_

#include <stdio.h>
#include <string.h>
#include <new>

namespace	openfoam
{

#define	MAX_ERR_STRING_LEN	(255)		///< 에러 메시지 최대 길이


/**
 * 에러 메시지 클래스
 */
class	ErrMsg
{
private:
	char*		mErrString;		///< 에러 메시지
	size_t		mMsgSize;
private:
	inline			ErrMsg		(	void	);
	inline virtual	~ErrMsg		(	void	);
private:
	inline bool		setErrString		(	const char		string[]	);
	inline void		delErrString		(	void	);
public:
	inline const char*	getErrString		(	void	);
	inline size_t			getErrMsgSize		(	void	);
public:
	inline static ErrMsg*	createErrMsg		(	const char		string[]	);
	inline static void		destroyErrMsg		(	ErrMsg*	errMsg			);
};

/**
 * 생성자
 */
ErrMsg::ErrMsg	(	void	)
:mErrString(NULL),
mMsgSize(0)
{

}

/**
 * 소멸자
 */
ErrMsg::~ErrMsg	(	void	)
{
	delErrString();
}

/**
 * 에러 메시지 설정
 * @return		에러 메시지 설정에 성공하면 true, 문자열 할당 실패 시 false
 */
bool		ErrMsg::setErrString		(	const char		string[]	///< 에러 메시지
										)
{
	bool	ret	=	false;

	do
	{
		size_t	length		=	::strnlen(string, MAX_ERR_STRING_LEN);
		if( length == 0 )
		{
			break;
		}

		try
		{
			mErrString		=	new char[length+1];
			mMsgSize		=	length;
		}
		catch( std::bad_alloc&	exception	)
		{
			exception.what();
			break;
		}

		memcpy(mErrString, string, sizeof(char)*(length+1));

		ret		=	true;
	}while(0);

	return	ret;
}

/**
 * 에러 메시지 제거
 */
void		ErrMsg::delErrString		(	void	)
{
	delete[]	mErrString;
}

/**
 * 에러 메시지 참조
 * @return		에러 메시지 리턴
 */
const char*	ErrMsg::getErrString		(	void	)
{
	return	mErrString;
}

/**
 * 에러 메시지 길이 참조
 * @return		에러 메시지 길이 리턴
 */
size_t			ErrMsg::getErrMsgSize		(	void	)
{
	return	mMsgSize;
}

/**
 * 에러 메시지 객체 생성
 * @return		성공 시 객체 리턴, 실패 시 NULL 리턴
 */
ErrMsg*	ErrMsg::createErrMsg		(	const char			string[]	///< 에러 메시지
										)
{
	ErrMsg*	errMsg	=	NULL;

	errMsg	=	new ErrMsg();
	if( errMsg->setErrString(string) == false )
	{
		delete	errMsg;
		errMsg	=	NULL;
	}

	return	errMsg;
}

/**
 * 에러 메시지 객체 제거
 */
void		ErrMsg::destroyErrMsg	(	ErrMsg*	errMsg			///< 제거 할 에러 메시지 객체
										)
{
	delete	errMsg;
}

};

#endif /* OPENFOAM_ERROR_H_ */
