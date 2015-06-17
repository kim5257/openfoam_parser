/*
 * openfoam_typedef.h
 *
 *  Created on: 2015. 2. 26.
 *      Author: asran
 */

#ifndef INCLUDE_OPENFOAM_TYPEDEF_H_
#define INCLUDE_OPENFOAM_TYPEDEF_H_

#include <stdint.h>

namespace	openfoam
{

// Owner 구조체 및 List 자료구조 정의
struct	Owner
{
	uint32_t	mCell;
};

typedef	std::vector<Owner>	owner_list_t;

}

#endif	/* INCLUDE_OPENFOAM_TYPEDEF_H_ */
