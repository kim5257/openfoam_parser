/*
 * volumes.h
 *
 *  Created on: 2015. 3. 2.
 *      Author: asran
 */

#ifndef INCLUDE__VOLUMES_H_
#define INCLUDE__VOLUMES_H_

#include <vector>
#include <stdint.h>
#include <stddef.h>

namespace	openfoam
{

struct	Volume
{
	double		mValue;
};

typedef	std::vector<Volume>	volume_list_t;

class	Volumes
{
private:
	volume_list_t	mData;
public:
				Volumes		(	void	);
	virtual	~Volumes	(	void	);
public:
	void		add			(	Volume&	face	);
	Volume&	get			(	size_t		index	);
public:
	inline size_t		getSize	(	void	);
};

size_t		Volumes::getSize	(	void	)
{
	return	mData.size();
}

}

#endif /* INCLUDE__VOLUMES_H_ */
