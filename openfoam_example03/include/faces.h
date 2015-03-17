/*
 * faces.h
 *
 *  Created on: 2015. 2. 25.
 *      Author: asran
 */

#ifndef INCLUDE_FACES_H_
#define INCLUDE_FACES_H_

#include <vector>
#include <stdint.h>
#include <stddef.h>

#define	POINTS_PER_FACE	(4)

namespace	openfoam
{

struct	Face
{
	uint32_t	mPoint[POINTS_PER_FACE];
};

typedef	std::vector<Face>		face_list_t;

class	Faces
{
private:
	face_list_t	mData;
public:
				Faces		(	void	);
	virtual	~Faces		(	void	);
public:
	void		add			(	Face&	face	);
	Face&		get			(	size_t	index	);
public:
	inline size_t		getSize	(	void	);
};

size_t		Faces::getSize	(	void	)
{
	return	mData.size();
}

}

#endif /* INCLUDE_FACES_H_ */
