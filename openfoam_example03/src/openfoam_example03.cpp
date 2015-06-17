/*
 * openfoam_example03.cpp
 *
 *  Created on: 2015. 2. 23.
 *      Author: asran
 */

#include "openfoam_parser_labellist.h"
#include "openfoam_error.h"
#include "faces.h"
#include "neighbors.h"
#include "owners.h"
#include "points.h"
#include "cells.h"
#include "boundaries.h"
#include "volumes.h"
#include "sparse_matrix.h"
#include "matrix_error.h"

#include <math.h>

// Input files
#define	FACES_FILE_NAME			"polyMesh/faces"
#define	NEIGHBOUR_FILE_NAME		"polyMesh/neighbour"
#define	OWNER_FILE_NAME			"polyMesh/owner"
#define	POINTS_FILE_NAME			"polyMesh/points"

// Output files
#define	A_MATRIX_DATA				"1/A matrix.dat"
#define	CELL_VOLUME_DATA			"1/cell volume.dat"
#define	POINT_DATA					"1/point data.dat"
#define	TEMPERATURE				"1/temperature.dat"

#define	CELLS_NUM					(640)
#define	VOLUME_GRADIENT_CNT		(41)

#define	B_C_TEMPERATURE			(1.0)
#define	DT							(0.001)	///< DT
#define	DENSITY					(1.0)		///< DEN
#define	CONSTANT					(1.0)		///< C
#define	GAMMA						(1.0)		///< GAMMA
#define	ADJUST						(1.0)		///< Aj

void	readFaces		(	openfoam::Parser&		facesFile,
							openfoam::Faces&		faces
						);

void	readNeighbors	(	openfoam::Parser_labelList&	neighbourFile,
							openfoam::Neighbors&			neighbours
						);

void	readOwners		(	openfoam::Parser_labelList&	ownerFile,
							openfoam::Owners&				owners
						);

void	readPoints		(	openfoam::Parser&		pointsFile,
							openfoam::Points&		points
						);

void	initCells		(	openfoam::Cells&			cells,
							openfoam::Faces&			faces,
							openfoam::Neighbors&		neighbours,
							openfoam::Owners&			owners
						);

void	initBoundaries	(	openfoam::Boundaries&	boundaries,
								openfoam::Faces&			faces,
								openfoam::Neighbors&		neighbours
							);

void	initSurfaces	(	openfoam::Points&			surfaces,
							openfoam::Faces&			faces,
							openfoam::Points&			points
						);

void	calcVolumes	(	openfoam::Volumes&		volumes,
							openfoam::Points&			points,
							openfoam::Cells&			cells,
							openfoam::Faces&			faces
						);

void	calcCenterVolumes		(	openfoam::Points&		centerVolumes,
									openfoam::Cells&		cells,
									openfoam::Faces&		faces,
									openfoam::Points&		points
								);

void	calcVolumeGradient	(	openfoam::Points&			volumeGradients,
									openfoam::Owners&			owners,
									openfoam::Cells&			cells,
									openfoam::Neighbors&		neighbors,
									openfoam::Boundaries&	boundaries,
									openfoam::Points&			surfaces,
									openfoam::Volumes&		volumes,
									openfoam::Points&			centerVolumes,
									matrix::SparseMatrix&	matrixA,
									matrix::SparseMatrix&	matrixX,
									matrix::SparseMatrix&	matrixB,
									FILE*						temperatureData
								);

void	storePointData		(	openfoam::Points&			points,
									FILE*						file
								);

void	storeTemperatureData	(	openfoam::Points&			centerVolumes,
									matrix::SparseMatrix&	matrixX,
									FILE*						file
								);

void	storeMatrixAData		(	matrix::SparseMatrix&	matrixA,
									FILE*						file
								);

void	storeCellVolData		(	openfoam::Owners&			owners,
									openfoam::Neighbors&		neighbors,
									openfoam::Cells&			cells,
									openfoam::Boundaries&	boundaries,
									openfoam::Points&			surfaces,
									openfoam::Points&			volumeGradients,
									FILE*						file
								);

void	ptrFaces		(	openfoam::Faces&			faces		);
void	ptrNeighbors	(	openfoam::Neighbors&		neighbours	);
void	ptrOwners		(	openfoam::Owners&			owners		);
void	ptrPoints		(	openfoam::Points&			points		);
void	ptrCells		(	openfoam::Cells&			cells		);
void	ptrBoundaries	(	openfoam::Boundaries&	boundaries	);
void	ptrSurfaces	(	openfoam::Points&			surfaces	);
void	ptrVolumes		(	openfoam::Volumes&		volumes	);
void	ptrCenterVolumes	(	openfoam::Points&			centerVolumes	);

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		openfoam::Parser				facesFile(FACES_FILE_NAME);
		openfoam::Parser_labelList	neighbourFile(NEIGHBOUR_FILE_NAME);
		openfoam::Parser_labelList	ownerFile(OWNER_FILE_NAME);
		openfoam::Parser				pointsFile(POINTS_FILE_NAME);

		openfoam::Faces				faces;
		openfoam::Neighbors			neighbours;
		openfoam::Owners				owners;
		openfoam::Points				points;
		openfoam::Cells				cells;
		openfoam::Boundaries			boundaries;
		openfoam::Points				surfaces;
		openfoam::Volumes				volumes;
		openfoam::Points				centerVolumes;
		openfoam::Points				volumeGradients;

		FILE*	pointData			=	NULL;
		FILE*	cellVolData		=	NULL;
		FILE*	temperatureData	=	NULL;
		FILE*	matrixAData		=	NULL;

		pointData			=	fopen(POINT_DATA, "w");
		cellVolData		=	fopen(CELL_VOLUME_DATA, "w");
		temperatureData	=	fopen(TEMPERATURE, "w");
		matrixAData		=	fopen(A_MATRIX_DATA, "w");

		if( ( pointData == NULL ) ||
			( cellVolData == NULL ) ||
			( temperatureData == NULL ) ||
			( matrixAData == NULL ) )
		{
			if( pointData != NULL )			{	fclose(pointData);		}
			if( cellVolData != NULL )		{	fclose(cellVolData);		}
			if( temperatureData != NULL )	{	fclose(temperatureData);	}
			if( matrixAData != NULL )		{	fclose(matrixAData);		}

			throw openfoam::ErrMsg::createErrMsg("데이터 파일 열기 실패.");
		}

		// 각 파일 헤더 및 데이터 크기 읽기
		facesFile.readHdr();
		neighbourFile.readHdr();
		ownerFile.readHdr();
		pointsFile.readHdr();

		facesFile.readSize();
		neighbourFile.readSize();
		ownerFile.readSize();
		pointsFile.readSize();

		readFaces(facesFile, faces);
		readNeighbors(neighbourFile, neighbours);
		readOwners(ownerFile, owners);
		readPoints(pointsFile, points);

		//ptrFaces(faces);
		//ptrNeighbors(neighbours);
		//ptrOwners(owners);
		//ptrPoints(points);

		initCells(cells, faces, neighbours, owners);
		//ptrCells(cells);

		initBoundaries(boundaries, faces, neighbours);
		//ptrBoundaries(boundaries);

		initSurfaces(surfaces, faces, points);
		//ptrSurfaces(surfaces);

		calcVolumes(volumes, points, cells, faces);
		//ptrVolumes(volumes);

		calcCenterVolumes(centerVolumes, cells, faces, points);
		//ptrCenterVolumes(centerVolumes);

		matrix::SparseMatrix		matrixA(cells.getSize(), cells.getSize());
		matrix::SparseMatrix		matrixX(cells.getSize(), 1);
		matrix::SparseMatrix		matrixB(cells.getSize(), 1);

		calcVolumeGradient	(	volumeGradients,
									owners,
									cells,
									neighbours,
									boundaries,
									surfaces,
									volumes,
									centerVolumes,
									matrixA,
									matrixX,
									matrixB,
									temperatureData
								);

		storePointData(points, pointData);
		storeMatrixAData(matrixA, matrixAData);

		storeCellVolData		(	owners,
									neighbours,
									cells,
									boundaries,
									surfaces,
									volumeGradients,
									cellVolData
								);
	}
	catch( openfoam::ErrMsg*	exception	)
	{
		printf	(	"%s\n",
					exception->getErrString()
				);
		openfoam::ErrMsg::destroyErrMsg(exception);
	}
	catch( matrix::ErrMsg*	exception	)
	{
		printf	(	"%s\n",
					exception->getErrString()
				);
		matrix::ErrMsg::destroyErrMsg(exception);
	}

	return 0;
}

void	readFaces		(	openfoam::Parser&		facesFile,
							openfoam::Faces&		faces
						)
{
	for(size_t cnt=0;cnt<facesFile.getSize();++cnt)
	{
		if( facesFile.readData() )
		{
			break;
		}

		openfoam::Face		face;
		std::queue<double>	data	=	facesFile.getData();
		size_t					index	=	0;

		while( !data.empty() )
		{
			face.mPoint[index++]	=	(uint32_t)data.front();
			data.pop();
		}

		faces.add(face);
	}
}

void	readNeighbors	(	openfoam::Parser_labelList&	neighbourFile,
							openfoam::Neighbors&			neighbours
						)
{
	for(size_t cnt=0;cnt<neighbourFile.getSize();++cnt)
	{
		if( neighbourFile.readData() )
		{
			break;
		}

		openfoam::Neighbor	neighbor;
		std::queue<double>	data	=	neighbourFile.getData();

		neighbor.mCell	=	(uint32_t)data.front();
		data.pop();

		neighbours.add(neighbor);
	}
}

void	readOwners		(	openfoam::Parser_labelList&	ownerFile,
							openfoam::Owners&				owners
						)
{
	for(size_t cnt=0;cnt<ownerFile.getSize();++cnt)
	{
		if( ownerFile.readData() )
		{
			break;
		}

		openfoam::Owner		owner;
		std::queue<double>	data	=	ownerFile.getData();

		owner.mCell	=	(uint32_t)data.front();
		data.pop();

		owners.add(owner);
	}
}

void	readPoints		(	openfoam::Parser&		pointsFile,
							openfoam::Points&		points
						)
{
	for(size_t cnt=0;cnt<pointsFile.getSize();++cnt)
	{
		if( pointsFile.readData() )
		{
			break;
		}

		openfoam::Point		point;
		std::queue<double>	data	=	pointsFile.getData();

		point.mX	=	data.front();
		data.pop();
		point.mY	=	data.front();
		data.pop();
		point.mZ	=	data.front();
		data.pop();

		points.add(point);
	}
}

void	initCells		(	openfoam::Cells&			cells,
							openfoam::Faces&			faces,
							openfoam::Neighbors&		neighbours,
							openfoam::Owners&			owners
						)
{
	for(size_t cnt=0;cnt<CELLS_NUM;++cnt)
	{
		openfoam::Cell	cell;

		for(size_t cnt2=0;cnt2<FACES_PER_CELL;++cnt2)
		{
			cell.mFace[cnt2]	=	(uint32_t)-1;
		}

		cells.add(cell);
	}

	for(size_t cnt=0;cnt<neighbours.getSize();++cnt)
	{
		openfoam::Neighbor&	neighbor	=	neighbours.get(cnt);
		openfoam::Cell&		cell		=	cells.get(neighbor.mCell);

		for(size_t cnt2=0;cnt2<FACES_PER_CELL;++cnt2)
		{
			if( cell.mFace[cnt2] == (uint32_t)-1 )
			{
				cell.mFace[cnt2]	=	cnt;
				break;
			}
		}
	}

	for(size_t cnt=0;cnt<owners.getSize();++cnt)
	{
		openfoam::Owner&		owner	=	owners.get(cnt);
		openfoam::Cell&		cell	=	cells.get(owner.mCell);

		for(size_t cnt2=0;cnt2<FACES_PER_CELL;++cnt2)
		{
			if( cell.mFace[cnt2] == (uint32_t)-1 )
			{
				cell.mFace[cnt2]	=	cnt;
				break;
			}
		}
	}
}

void	initBoundaries	(	openfoam::Boundaries&	boundaries,
								openfoam::Faces&			faces,
								openfoam::Neighbors&		neighbours
							)
{
	for(size_t cnt=0;cnt<neighbours.getSize();++cnt)
	{
		openfoam::Boundary	boundary;

		boundary.mType	=	openfoam::Boundary::INTERNAL;

		boundaries.add(boundary);
	}

	for(size_t cnt=neighbours.getSize();cnt<faces.getSize();++cnt)
	{
		openfoam::Boundary	boundary;

		boundary.mType	=	openfoam::Boundary::TEMPERATURA;

		boundaries.add(boundary);
	}
}

void	initSurfaces	(	openfoam::Points&			surfaces,
							openfoam::Faces&			faces,
							openfoam::Points&			points
						)
{
	for(size_t cnt=0;cnt<faces.getSize();++cnt)
	{
		openfoam::Point	surface;

		openfoam::Face&	face	=	faces.get(cnt);
		openfoam::Point&	point1	=	points.get(face.mPoint[2]);
		openfoam::Point&	point2	=	points.get(face.mPoint[0]);
		openfoam::Point&	point3	=	points.get(face.mPoint[3]);
		openfoam::Point&	point4	=	points.get(face.mPoint[1]);

		double	x1	=	point1.mX - point2.mX;
		double	y1	=	point1.mY - point2.mY;
		double	z1	=	point1.mZ - point2.mZ;

		double	x2	=	point3.mX - point4.mX;
		double	y2	=	point3.mY - point4.mY;
		double	z2	=	point3.mZ - point4.mZ;

		surface.mX	=	0.5 * ((y1 * z2) - (y2 * z1));
		surface.mY	=	0.5 * ((z1 * x2) - (z2 * x1));
		surface.mZ	=	0.5 * ((x1 * y2) - (x2 * y1));

		surfaces.add(surface);
	}
}

void	calcVolumes	(	openfoam::Volumes&		volumes,
							openfoam::Points&			points,
							openfoam::Cells&			cells,
							openfoam::Faces&			faces
						)
{
	for(size_t cnt=0;cnt<cells.getSize();++cnt)
	{
		openfoam::Volume		volume;
		uint32_t	faceNum1	=	cells.get(cnt).mFace[0];
		uint32_t	id4			=	faces.get(faceNum1).mPoint[0];

		volume.mValue		=	0.0;

		for(size_t cnt2=1;cnt2<FACES_PER_CELL;++cnt2)
		{
			uint32_t	faceNum2	=	cells.get(cnt).mFace[cnt2];
			bool		owned		=	false;

			for(size_t cnt3=0;cnt3<POINTS_PER_FACE;++cnt3)
			{
				if( id4 == faces.get(faceNum2).mPoint[cnt3] )
				{
					owned	=	true;
					break;
				}
			}

			if( owned == false )
			{
				{
					double	id1		=	faces.get(faceNum2).mPoint[0];
					double	id2		=	faces.get(faceNum2).mPoint[1];
					double	id3		=	faces.get(faceNum2).mPoint[2];

					double	x1		=	points.get(id1).mX - points.get(id4).mX;
					double	y1		=	points.get(id1).mY - points.get(id4).mY;
					double	z1		=	points.get(id1).mZ - points.get(id4).mZ;
					double	x2		=	points.get(id2).mX - points.get(id4).mX;
					double	y2		=	points.get(id2).mY - points.get(id4).mY;
					double	z2		=	points.get(id2).mZ - points.get(id4).mZ;
					double	x3		=	points.get(id3).mX - points.get(id4).mX;
					double	y3		=	points.get(id3).mY - points.get(id4).mY;
					double	z3		=	points.get(id3).mZ - points.get(id4).mZ;

					volume.mValue		+=	( (	(x1 * ((y2*z3) - (y3*z2))) +
												(y1 * ((z2*x3) - (z3*x2))) +
												(z1 * ((x2*y3) - (x3*y2))) ) / 6.0 );
				}

				{
					double	id1		=	faces.get(faceNum2).mPoint[2];
					double	id2		=	faces.get(faceNum2).mPoint[3];
					double	id3		=	faces.get(faceNum2).mPoint[0];

					double	x1		=	points.get(id1).mX - points.get(id4).mX;
					double	y1		=	points.get(id1).mY - points.get(id4).mY;
					double	z1		=	points.get(id1).mZ - points.get(id4).mZ;
					double	x2		=	points.get(id2).mX - points.get(id4).mX;
					double	y2		=	points.get(id2).mY - points.get(id4).mY;
					double	z2		=	points.get(id2).mZ - points.get(id4).mZ;
					double	x3		=	points.get(id3).mX - points.get(id4).mX;
					double	y3		=	points.get(id3).mY - points.get(id4).mY;
					double	z3		=	points.get(id3).mZ - points.get(id4).mZ;

					volume.mValue		+=	( (	(x1 * ((y2*z3) - (y3*z2))) +
												(y1 * ((z2*x3) - (z3*x2))) +
												(z1 * ((x2*y3) - (x3*y2))) ) / 6.0 );
				}
			}
		}

		volumes.add(volume);
	}
}

void	calcCenterVolumes		(	openfoam::Points&			centerVolumes,
									openfoam::Cells&			cells,
									openfoam::Faces&			faces,
									openfoam::Points&			points
								)
{
	for(size_t cnt=0;cnt<cells.getSize();++cnt)
	{
		openfoam::Point	centerVolume	=	{0.0, };

		for(size_t cnt2=0;cnt2<FACES_PER_CELL;++cnt2)
		{
			size_t	faceNum	=	cells.get(cnt).mFace[cnt2];

			for(size_t cnt3=0;cnt3<POINTS_PER_FACE;++cnt3)
			{
				size_t	pointNum	=	faces.get(faceNum).mPoint[cnt3];

				centerVolume.mX	+=		points.get(pointNum).mX;
				centerVolume.mY	+=		points.get(pointNum).mY;
				centerVolume.mZ	+=		points.get(pointNum).mZ;
			}
		}

		centerVolume.mX	/=	24.0;
		centerVolume.mY	/=	24.0;
		centerVolume.mZ	/=	24.0;

		centerVolumes.add(centerVolume);
	}
}

void	calcVolumeGradient	(	openfoam::Points&			volumeGradients,
									openfoam::Owners&			owners,
									openfoam::Cells&			cells,
									openfoam::Neighbors&		neighbors,
									openfoam::Boundaries&	boundaries,
									openfoam::Points&			surfaces,
									openfoam::Volumes&		volumes,
									openfoam::Points&			centerVolumes,
									matrix::SparseMatrix&	matrixA,
									matrix::SparseMatrix&	matrixX,
									matrix::SparseMatrix&	matrixB,
									FILE*						temperatureData
								)
{
	for(size_t cnt=0;cnt<VOLUME_GRADIENT_CNT;++cnt)
	{
		volumeGradients.clear();
		for(size_t cnt2=0;cnt2<cells.getSize();++cnt2)
		{
			openfoam::Point	volGrad	=	{0.0,};

			for(size_t cnt3=0;cnt3<FACES_PER_CELL;++cnt3)
			{
				uint32_t	faceNum	=	cells.get(cnt2).mFace[cnt3];
				double		x		=	0.0;
				double		y		=	0.0;
				double		z		=	0.0;

				if( cnt2 == owners.get(faceNum).mCell )
				{
					x	=	surfaces.get(faceNum).mX;
					y	=	surfaces.get(faceNum).mY;
					z	=	surfaces.get(faceNum).mZ;
				}
				else
				{
					x	=	-surfaces.get(faceNum).mX;
					y	=	-surfaces.get(faceNum).mY;
					z	=	-surfaces.get(faceNum).mZ;
				}

				if( faceNum < neighbors.getSize() )
				{
					uint32_t	cellNum		=	owners.get(faceNum).mCell;
					uint32_t	neighborNum	=	neighbors.get(faceNum).mCell;

					double		temperature	=	0.5 * (matrixX.getElem(cellNum,0) + matrixX.getElem(neighborNum,0));

					volGrad.mX		+=	temperature * x;
					volGrad.mY		+=	temperature * y;
					volGrad.mZ		+=	temperature * z;
				}
				else
				{
					switch( boundaries.get(faceNum).mType )
					{
					case openfoam::Boundary::INTERNAL:
						break;
					case openfoam::Boundary::FLUX:
						break;
					case openfoam::Boundary::TEMPERATURA:
						{
							double		temperature	=	B_C_TEMPERATURE;

							volGrad.mX		+=	temperature * x;
							volGrad.mY		+=	temperature * y;
							volGrad.mZ		+=	temperature * z;
						}
						break;
					}
				}
			}

			volGrad.mX		/=	volumes.get(cnt2).mValue;
			volGrad.mY		/=	volumes.get(cnt2).mValue;
			volGrad.mZ		/=	volumes.get(cnt2).mValue;

			volumeGradients.add(volGrad);
		}

		matrixA.clear();
		for(size_t cnt2=0;cnt2<cells.getSize();++cnt2)
		{
			double	valueA		=	matrixA.getElem(cnt2, cnt2);
			double	valueB		=	0.0;

			double	area		=	0.0;
			openfoam::Point	p;

			valueA	=	DENSITY * volumes.get(cnt2).mValue / DT;
			valueB	=	matrixX.getElem(cnt2, 0) * DENSITY * volumes.get(cnt2).mValue / DT;

			matrixA.setElem(cnt2, cnt2, valueA);
			matrixB.setElem(cnt2, 0, valueB);

			for(size_t cnt3=0;cnt3<FACES_PER_CELL;++cnt3)
			{
				size_t	faceNum	=	cells.get(cnt2).mFace[cnt3];

				if( cnt2 == owners.get(faceNum).mCell )
				{
					p.mX	=	surfaces.get(faceNum).mX;
					p.mY	=	surfaces.get(faceNum).mY;
					p.mZ	=	surfaces.get(faceNum).mZ;
				}
				else
				{
					p.mX	=	-surfaces.get(faceNum).mX;
					p.mY	=	-surfaces.get(faceNum).mY;
					p.mZ	=	-surfaces.get(faceNum).mZ;
				}

				area	=	sqrt(p.mX*p.mX + p.mY*p.mY + p.mZ*p.mZ);

				if( faceNum < neighbors.getSize() )
				{
					uint32_t	j		=	0.0;
					double		aJ		=	0.0;
					double		value	=	0.0;

					openfoam::Point	p1;
					openfoam::Point	p2;
					openfoam::Point	p3;

					if( cnt2 == owners.get(faceNum).mCell )
					{
						j	=	neighbors.get(faceNum).mCell;
					}
					else
					{
						j	=	owners.get(faceNum).mCell;
					}

					p1.mX	=	centerVolumes.get(j).mX - centerVolumes.get(cnt2).mX;
					p1.mY	=	centerVolumes.get(j).mY - centerVolumes.get(cnt2).mY;
					p1.mZ	=	centerVolumes.get(j).mZ - centerVolumes.get(cnt2).mZ;

					p2.mX	=	p.mX / area;
					p2.mY	=	p.mY / area;
					p2.mZ	=	p.mZ / area;

					p3		=	volumeGradients.get(cnt2);

					aJ		=	area / (p1.mX*p2.mX + p1.mY*p2.mY + p1.mZ*p2.mZ);

					value	=	matrixA.getElem(cnt2, cnt2);
					value	+=	GAMMA * aJ;
					matrixA.setElem(cnt2, cnt2,  value);

					value	=	matrixA.getElem(cnt2, j);
					value	-=	GAMMA * aJ;
					matrixA.setElem(cnt2, j,  value);

					value	=	matrixB.getElem(cnt2, 0);
					value	+=	GAMMA * ((p3.mX*p.mX + p3.mY*p.mY + p3.mZ*p.mZ) - (p3.mX*p1.mX + p3.mY*p1.mY + p3.mZ*p1.mZ) * aJ );
					matrixB.setElem(cnt2, 0,  value);
				}
				else
				{
					switch( boundaries.get(faceNum).mType )
					{
					case openfoam::Boundary::INTERNAL:
						break;
					case openfoam::Boundary::FLUX:
						break;
					case openfoam::Boundary::TEMPERATURA:
						{
							double		aJ		=	0.0;
							double		value	=	0.0;

							openfoam::Point	p1;
							openfoam::Point	p2;
							openfoam::Point	p3;

							p2.mX	=	p.mX / area;
							p2.mY	=	p.mY / area;
							p2.mZ	=	p.mZ / area;

							p1.mX	=	0.025 * p2.mX / 2;
							p1.mY	=	0.025 * p2.mY / 2;
							p1.mZ	=	0.025 * p2.mZ / 2;

							p3		=	volumeGradients.get(cnt2);

							aJ		=	area / (p1.mX*p2.mX + p1.mY*p2.mY + p1.mZ*p2.mZ);

							value	=	matrixA.getElem(cnt2, cnt2);
							value	+=	GAMMA * aJ;
							matrixA.setElem(cnt2, cnt2,  value);

							value	=	matrixB.getElem(cnt2, 0);
							value	+=	GAMMA * aJ * B_C_TEMPERATURE;
							value	+=	GAMMA * ((p3.mX*p.mX + p3.mY*p.mY + p3.mZ*p.mZ) - (p3.mX*p1.mX + p3.mY*p1.mY + p3.mZ*p1.mZ) * aJ );
							matrixB.setElem(cnt2, 0,  value);
						}
						break;
					}
				}
			}
		}

		matrixX	=	matrixA.sol_cg(matrixB);

		if( (cnt%10) == 0 )
		{
			storeTemperatureData(centerVolumes, matrixX, temperatureData);
		}
	}
}

void	storePointData		(	openfoam::Points&			points,
									FILE*						file
								)
{
	for(size_t cnt=0;cnt<points.getSize();++cnt)
	{
		fprintf	(	file,
						"%lf %lf %lf\n",
						points.get(cnt).mX,
						points.get(cnt).mY,
						points.get(cnt).mZ
					);
	}
}

void	storeTemperatureData	(	openfoam::Points&			centerVolumes,
									matrix::SparseMatrix&	matrixX,
									FILE*						file
								)
{
	fprintf(file, "zone\n");

	for(size_t cnt=0;cnt<centerVolumes.getSize();++cnt)
	{
		openfoam::Point&	cell	=	centerVolumes.get(cnt);
		fprintf	(	file,
						"%lf   %lf   %lf   %lf\n",
						cell.mX,
						cell.mY,
						cell.mZ,
						matrixX.getElem(cnt, 0)
					);
	}
}

void	storeMatrixAData		(	matrix::SparseMatrix&	matrixA,
									FILE*						file
								)
{
	for(size_t row=0;row<matrixA.getRow();++row)
	{
		for(size_t col=0;col<matrixA.getCol();++col)
		{
			fprintf(file, "%lf   ", matrixA.getElem(row, col));
		}
		fprintf(file, "\n");
	}
}

void	storeCellVolData		(	openfoam::Owners&			owners,
									openfoam::Neighbors&		neighbors,
									openfoam::Cells&			cells,
									openfoam::Boundaries&	boundaries,
									openfoam::Points&			surfaces,
									openfoam::Points&			volumeGradients,
									FILE*						file
								)
{
	for(size_t cnt=0;cnt<cells.getSize();++cnt)
	{
		uint32_t	I	=	0;

		for(size_t cnt2=0;cnt2<FACES_PER_CELL;++cnt2)
		{
			uint32_t	faceNum	=	cells.get(cnt).mFace[cnt2];
			I	+=	boundaries.get(faceNum).mType;
		}

		fprintf	(	file,
						"%lu   %d   %lf   %lf   %lf\n",
						cnt,
						I,
						volumeGradients.get(cnt).mX,
						volumeGradients.get(cnt).mY,
						volumeGradients.get(cnt).mZ
					);

		for(size_t cnt2=0;cnt2<FACES_PER_CELL;++cnt2)
		{
			uint32_t			faceNum	=	cells.get(cnt).mFace[cnt2];
			openfoam::Point	point;

			if( cnt == owners.get(faceNum).mCell )
			{
				point	=	surfaces.get(faceNum);
			}
			else
			{
				point.mX	=	-surfaces.get(faceNum).mX;
				point.mY	=	-surfaces.get(faceNum).mY;
				point.mZ	=	-surfaces.get(faceNum).mZ;
			}

			fprintf	(	file,
							"%lf   %lf   %lf   %d\n",
							point.mX,
							point.mY,
							point.mZ,
							boundaries.get(faceNum).mType
						);
		}
	}
}

void	ptrFaces		(	openfoam::Faces&			faces		)
{
	for(size_t cnt=0;cnt<faces.getSize();++cnt)
	{
		for(size_t cnt2=0;cnt2<POINTS_PER_FACE;++cnt2)
		{
			printf("%d ", faces.get(cnt).mPoint[cnt2]);
		}
		printf("\n");
	}
}

void	ptrNeighbors	(	openfoam::Neighbors&		neighbours	)
{
	for(size_t cnt=0;cnt<neighbours.getSize();++cnt)
	{
		printf("%d\n", neighbours.get(cnt).mCell);
	}
}

void	ptrOwners		(	openfoam::Owners&			owners		)
{
	for(size_t cnt=0;cnt<owners.getSize();++cnt)
	{
		printf("%d\n", owners.get(cnt).mCell);
	}
}

void	ptrPoints		(	openfoam::Points&			points		)
{
	for(size_t cnt=0;cnt<points.getSize();++cnt)
	{
		printf("%lf %lf %lf\n", points.get(cnt).mX, points.get(cnt).mY, points.get(cnt).mZ);
	}
}

void	ptrCells		(	openfoam::Cells&			cells		)
{
	for(size_t cnt=0;cnt<cells.getSize();++cnt)
	{
		for(size_t cnt2=0;cnt2<FACES_PER_CELL;++cnt2)
		{
			printf("%d ", cells.get(cnt).mFace[cnt2]);
		}
		printf("\n");
	}
}

void	ptrBoundaries	(	openfoam::Boundaries&	boundaries	)
{
	for(size_t cnt=0;cnt<boundaries.getSize();++cnt)
	{
		printf("%d\n", boundaries.get(cnt).mType);
	}
}

void	ptrSurfaces	(	openfoam::Points&			surfaces	)
{
	ptrPoints(surfaces);
}

void	ptrVolumes		(	openfoam::Volumes&		volumes	)
{
	for(size_t cnt=0;cnt<volumes.getSize();++cnt)
	{
		printf("%lf\n", volumes.get(cnt).mValue);
	}
}

void	ptrCenterVolumes	(	openfoam::Points&			centerVolumes	)
{
	ptrPoints(centerVolumes);
}
