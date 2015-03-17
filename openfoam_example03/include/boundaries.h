/*
 * boundary.h
 *
 *  Created on: 2015. 2. 26.
 *      Author: asran
 */

#ifndef INCLUDE_BOUNDARIES_H_
#define INCLUDE_BOUNDARIES_H_

#include <vector>
#include <stdint.h>
#include <stddef.h>

namespace	openfoam
{

struct	Boundary
{
	enum	BoundaryType
	{
		INTERNAL		=	0,
		FLUX			=	1,
		TEMPERATURA	=	2,
	};
	BoundaryType	mType;
};

typedef	std::vector<Boundary>	boundary_list_t;

class	Boundaries
{
private:
	boundary_list_t	mData;
public:
				Boundaries		(	void	);
	virtual	~Boundaries		(	void	);
public:
	void			add			(	Boundary&	face	);
	Boundary&		get			(	size_t		index	);
public:
	inline size_t		getSize	(	void	);
};

size_t		Boundaries::getSize	(	void	)
{
	return	mData.size();
}

}

#endif /* INCLUDE_BOUNDARIES_H_ */
