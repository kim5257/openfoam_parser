/*
 * cells.h
 *
 *  Created on: 2015. 2. 26.
 *      Author: asran
 */

#ifndef INCLUDE__CELLS_H_
#define INCLUDE__CELLS_H_

#include <vector>
#include <stdint.h>
#include <stddef.h>

#define	FACES_PER_CELL	(6)

namespace	openfoam
{

struct	Cell
{
	uint32_t	mFace[FACES_PER_CELL];
};

typedef	std::vector<Cell>		cell_list_t;

class	Cells
{
private:
	cell_list_t	mData;
public:
				Cells		(	void	);
	virtual	~Cells		(	void	);
public:
	void		add			(	Cell&	face	);
	Cell&		get			(	size_t	index	);
public:
	inline size_t		getSize	(	void	);
};

size_t		Cells::getSize	(	void	)
{
	return	mData.size();
}

}

#endif /* INCLUDE__CELLS_H_ */
