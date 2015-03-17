/*
 * openfoam_hdr.h
 *
 *  Created on: 2015. 02. 11.
 *      Author: asran
 */

#ifndef OPENFOAM_HDR_H_
#define OPENFOAM_HDR_H_

#include <string>

namespace openfoam
{

class Hdr
{
private:
	std::string		mVersion;
	std::string		mFormat;
	std::string		mClass;
	std::string		mNote;
	std::string		mLocation;
	std::string		mObject;
public:
				Hdr		(	void	){}
	virtual	~Hdr	(	void	){}
public:
	inline void	setVersion		(	const char	value[]		);
	inline void	setFormat		(	const char	value[]		);
	inline void	setClass		(	const char	value[]		);
	inline void	setNote		(	const char	value[]		);
	inline void	setLocation	(	const char	value[]		);
	inline void	setObject		(	const char	value[]		);
public:
	inline const char*		getVersion		(	void	);
	inline const char*		getFormat		(	void	);
	inline const char*		getClass		(	void	);
	inline const char*		getNote		(	void	);
	inline const char*		getLocation	(	void	);
	inline const char*		getObject		(	void	);
private:

};

void	Hdr::setVersion		(	const char	value[]		)
{
	mVersion	=	value;
}

void	Hdr::setFormat		(	const char	value[]		)
{
	mFormat		=	value;
}

void	Hdr::setClass		(	const char	value[]		)
{
	mClass		=	value;
}

void	Hdr::setNote		(	const char	value[]		)
{
	mNote		=	value;
}

void	Hdr::setLocation	(	const char	value[]		)
{
	mLocation	=	value;
}

void	Hdr::setObject		(	const char	value[]		)
{
	mObject		=	value;
}

const char*		Hdr::getVersion		(	void	)
{
	return	mVersion.c_str();
}

const char*		Hdr::getFormat		(	void	)
{
	return	mFormat.c_str();
}

const char*		Hdr::getClass		(	void	)
{
	return	mClass.c_str();
}

const char*		Hdr::getNote		(	void	)
{
	return	mNote.c_str();
}

const char*		Hdr::getLocation		(	void	)
{
	return	mLocation.c_str();
}

const char*		Hdr::getObject		(	void	)
{
	return	mObject.c_str();
}

}

#endif /* OPENFOAM_HDR_H_ */
