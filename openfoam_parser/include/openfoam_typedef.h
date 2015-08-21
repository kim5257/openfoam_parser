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

// Neighbour 구조체 및 Vector 자료구조 정의
// * Neighbour 구조체는 Owner 구조체와 동일하므로 그대로 사용하도록 함
typedef	Owner					Neighbour;
typedef	owner_vector_t		neighbour_vector_t;

// Face 구조체 및 Vector 자료구조 정의
struct	Face
{
	std::vector<uint32_t>	mPoints;
};

typedef	std::vector<Face>		face_vector_t;

struct	Boundary
{
	std::string	mName;
	std::string	mType;
	uint32_t		mFaces;
	uint32_t		mStartFace;
};

typedef	std::vector<Boundary>	boundary_vector_t;

}

#endif	/* INCLUDE__OPENFOAM_TYPEDEF_H_ */
