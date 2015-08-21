/*
 * openfoam_example04.cpp
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
		openfoam::Neighbours	neighbours("neighbour", openfoam::Parser::FILE_READ);
		openfoam::Neighbours	neighbours2;

		neighbours2.open("copy_neighbour", openfoam::Parser::FILE_WRITE);

		// owner 헤더 및 데이터 읽기
		neighbours.readFile();
		neighbours2.copy(neighbours);
		neighbours2.writeFile();

		// 헤더 정보 출력
		printf("version: %s\n",neighbours.getHdr().getVersion());
		printf("format: %s\n",neighbours.getHdr().getFormat());
		printf("class: %s\n",neighbours.getHdr().getClass());
		printf("location: %s\n",neighbours.getHdr().getLocation());
		printf("object: %s\n",neighbours.getHdr().getObject());

		// 적재한 owners 데이터 출력
		printf("(\n");
		for(size_t cnt=0;cnt<neighbours.getSize();++cnt)
		{
			printf("%d\n", neighbours[cnt].mCell);
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
