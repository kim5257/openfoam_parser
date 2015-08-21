/*
 * openfoam_example06.cpp
 *
 *  Created on: 2015. 7. 22.
 *      Author: asran
 */

#include <openfoam.h>

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		openfoam::Boundaries		boundaries("boundary", openfoam::Parser::FILE_READ);
		openfoam::Boundaries		boundaries2;

		boundaries2.open("copy_boundary", openfoam::Parser::FILE_WRITE);

		// owner 헤더 및 데이터 읽기
		boundaries.readFile();
		boundaries2.copy(boundaries);
		boundaries2.writeFile();

		// 헤더 정보 출력
		printf("version: %s\n",boundaries.getHdr().getVersion());
		printf("format: %s\n",boundaries.getHdr().getFormat());
		printf("class: %s\n",boundaries.getHdr().getClass());
		printf("location: %s\n",boundaries.getHdr().getLocation());
		printf("object: %s\n",boundaries.getHdr().getObject());

		// 적재한 owners 데이터 출력
		printf("(\n");
		for(size_t cnt=0;cnt<boundaries.getSize();++cnt)
		{
			openfoam::Boundary	boundary	=	boundaries[cnt];

			printf	(	"%s\n{\n"
						"    %-15s %s;\n"
						"    %-15s %d;\n"
						"    %-15s %d;\n"
						"}\n",
						boundary.mName.c_str(),
						"type",
						boundary.mType.c_str(),
						"nFaces",
						boundary.mFaces,
						"startFace",
						boundary.mStartFace
					);
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
