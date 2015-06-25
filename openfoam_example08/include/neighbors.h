/*
 * neighbors.h
 *
 *  Created on: 2015. 2. 25.
 *      Author: asran
 */

#ifndef INCLUDE__NEIGHBORS_H_
#define INCLUDE__NEIGHBORS_H_

#include <vector>
#include <stdint.h>
#include <stddef.h>

namespace	openfoam
{

struct	Neighbor
{
	uint32_t	mCell;
};

typedef	std::vector<Neighbor>	neighbor_list_t;

class	Neighbors
{
private:
	neighbor_list_t	mData;
public:
				Neighbors		(	void	);
	virtual	~Neighbors		(	void	);
public:
	void			add			(	Neighbor&	face	);
	Neighbor&		get			(	size_t		index	);
public:
	inline size_t		getSize	(	void	);
};

size_t		Neighbors::getSize	(	void	)
{
	return	mData.size();
}

}

#endif /* INCLUDE__NEIGHBORS_H_ */
