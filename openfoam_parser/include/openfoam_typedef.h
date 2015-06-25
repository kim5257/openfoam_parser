/*
 * openfoam_typedef.h
 *
 *  Created on: 2015. 2. 26.
 *      Author: asran
 */

#ifndef INCLUDE__OPENFOAM_TYPEDEF_H_
#define INCLUDE__OPENFOAM_TYPEDEF_H_

#include <stdint.h>

namespace	openfoam
{

// Owner 구조체 및 Vector 자료구조 정의
struct	Owner
{
	uint32_t	mCell;
};

typedef	std::vector<Owner>	owner_vector_t;

// Point 구조체 및 Vector 자료구조 정의
struct	Point
{
	double		mX;
	double		mY;
	double		mZ;
};

typedef	std::vector<Point>	point_vector_t;

}

#endif	/* INCLUDE__OPENFOAM_TYPEDEF_H_ */
