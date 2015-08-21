/*
 * openfoam_example05.cpp
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
		openfoam::Faces	faces("faces", openfoam::Parser::FILE_READ);
		openfoam::Faces	faces2;

		faces2.open("copy_faces", openfoam::Parser::FILE_WRITE);

		// owner 헤더 및 데이터 읽기
		faces.readFile();
		faces2.copy(faces);
		faces2.writeFile();

		// 헤더 정보 출력
		printf("version: %s\n",faces.getHdr().getVersion());
		printf("format: %s\n",faces.getHdr().getFormat());
		printf("class: %s\n",faces.getHdr().getClass());
		printf("location: %s\n",faces.getHdr().getLocation());
		printf("object: %s\n",faces.getHdr().getObject());

		// 적재한 owners 데이터 출력
		printf("(\n");
		for(size_t cnt=0;cnt<faces.getSize();++cnt)
		{
			printf("%d(", POINTS_PER_FACE);
			for(size_t num=0;num<POINTS_PER_FACE;++num)
			{
				printf("%d ", faces[cnt].mPoint[num]);
			}
			printf(")\n");
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
