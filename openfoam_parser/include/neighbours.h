/*
 * neighbours.h
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#ifndef NEIGHBOURS_H_
#define NEIGHBOURS_H_

#include <openfoam_error.h>
#include <openfoam_parser.h>
#include <openfoam_parser_labellist.h>
#include <openfoam_typedef.h>

namespace	openfoam
{

class	Neighbours
{
private:
	Parser_labelList		mParser;
	neighbour_vector_t	mData;
public:
				Neighbours		(	void	);
				Neighbours		(	const char			fileName[],
									Parser::OpenType	type
								);
	virtual	~Neighbours	(	void	);
public:
	void			open			(	const char			fileName[],
										Parser::OpenType	type
									);
	void			close			(	void	);
	void			readFile		(	void	);
	void			writeFile		(	void	);
	Neighbour&		getData		(	size_t				index		);
	void			copy			(	const Neighbours&	neighbours	);
public:
	inline Hdr&			getHdr			(	void	);
	inline size_t			getSize		(	void	);
	inline Neighbour&		operator[]		(	size_t		index	);
};

Hdr&		Neighbours::getHdr		(	void	)
{
	return	mParser.refHdr();
}

size_t		Neighbours::getSize		(	void	)
{
	return	mData.size();
}

Neighbour&		Neighbours::operator[]	(	size_t		index	)
{
	return	getData(index);
}

};

#endif /* NEIGHBOURS_H_ */
