/*
 * boundaries.h
 *
 *  Created on: 2015. 7. 23.
 *      Author: asran
 */

#ifndef DICTIONARIES_H_
#define DICTIONARIES_H_

#include <openfoam_error.h>
#include <openfoam_parser.h>
#include <openfoam_parser_dictionary.h>
#include <openfoam_typedef.h>

namespace openfoam
{

class	Dictionaries
{
private:
	Parser_dictionary		mParser;
	dic_vector_t			mData;
public:
				Dictionaries		(	void	);
				Dictionaries		(	const char			fileName[],
										Parser::OpenType	type
									);
	virtual	~Dictionaries	(	void	);
public:
	void		open		(	const char			fileName[],
								Parser::OpenType	type
							);
	void		close		(	void	);
	void		readFile	(	void	);
	void		writeFile	(	void	);
	Dictionary&	getData	(	size_t				index	);
	void		copy		(	const Dictionaries&	dictionaries	);
public:
	inline Hdr&			getHdr			(	void	);
	inline size_t			getSize		(	void	);
	inline Dictionary&		operator[]		(	size_t		index	);
};

Hdr&		Dictionaries::getHdr		(	void	)
{
	return	mParser.refHdr();
}

size_t		Dictionaries::getSize		(	void	)
{
	return	mData.size();
}

Dictionary&	Dictionaries::operator[]	(	size_t		index	)
{
	return	getData(index);
}

}

#endif /* DICTIONARIES_H_ */
