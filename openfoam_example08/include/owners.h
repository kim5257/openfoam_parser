/*
 * owners.h
 *
 *  Created on: 2015. 2. 25.
 *      Author: asran
 */

#ifndef INCLUDE_OWNERS_H_
#define INCLUDE_OWNERS_H_

#include <vector>
#include <stdint.h>
#include <stddef.h>

namespace	openfoam
{

struct	Owner
{
	uint32_t	mCell;
};

typedef	std::vector<Owner>	owner_list_t;

class	Owners
{
private:
	owner_list_t	mData;
public:
				Owners		(	void	);
	virtual	~Owners	(	void	);
public:
	void		add			(	Owner&	face	);
	Owner&		get			(	size_t	index	);
public:
	inline size_t		getSize	(	void	);
};

size_t		Owners::getSize	(	void	)
{
	return	mData.size();
}

}

#endif /* INCLUDE_OWNERS_H_ */
