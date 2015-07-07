/*
 * openfoam_example03.cpp
 *
 *  Created on: 2015. 6. 25.
 *      Author: asran
 */

#include <openfoam.h>

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		openfoam::Points	points("points", openfoam::Parser::FILE_READ);
		openfoam::Points	points2("copy_points", openfoam::Parser::FILE_WRITE);

		// owner 헤더 및 데이터 읽기
		points.readFile();
		points2.copy(points);
		points2.writeFile();

		// 헤더 정보 출력
		printf("version: %s\n",points.getHdr().getVersion());
		printf("format: %s\n",points.getHdr().getFormat());
		printf("class: %s\n",points.getHdr().getClass());
		printf("location: %s\n",points.getHdr().getLocation());
		printf("object: %s\n",points.getHdr().getObject());

		// 적재한 owners 데이터 출력
		printf("(\n");
		for(size_t cnt=0;cnt<points.getSize();++cnt)
		{
			printf("(%.2lf %.2lf %.2lf)\n", points[cnt].mX, points[cnt].mY, points[cnt].mZ);
		}
		printf(")\n");
	}
	catch( openfoam::ErrMsg*	exception	)
	{
		printf	(	"%s\n",
					exception->getErrString()
				);
		openfoam::ErrMsg::destroyErrMsg(exception);
	}

	return 0;
}
