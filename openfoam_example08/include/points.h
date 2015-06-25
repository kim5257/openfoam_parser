/*
 * points.h
 *
 *  Created on: 2015. 2. 25.
 *      Author: asran
 */

#ifndef INCLUDE_POINTS_H_
#define INCLUDE_POINTS_H_

#include <vector>
#include <stdint.h>
#include <stddef.h>

namespace	openfoam
{

struct	Point
{
	double		mX;
	double		mY;
	double		mZ;
};

typedef	std::vector<Point>		point_list_t;

class	Points
{
private:
	point_list_t	mData;
public:
				Points		(	void	);
	virtual	~Points	(	void	);
public:
	void		add			(	Point&	face	);
	Point&		get			(	size_t	index	);
	void		clear		(	void	);
public:
	inline size_t		getSize	(	void	);
};

size_t		Points::getSize	(	void	)
{
	return	mData.size();
}

}

#endif /* INCLUDE_POINTS_H_ */
