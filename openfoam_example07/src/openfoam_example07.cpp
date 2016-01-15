/*
 * openfoam_example07.cpp
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
		openfoam::Dictionaries	dictionaries("cellZones", openfoam::Parser::FILE_READ);
		openfoam::Dictionaries	dictionaries2;

		dictionaries2.open("copy_cellZones", openfoam::Parser::FILE_WRITE);

		// owner 헤더 및 데이터 읽기
		dictionaries.readFile();
		dictionaries2.copy(dictionaries);
		dictionaries2.writeFile();

		// 헤더 정보 출력
		printf("version: %s\n",dictionaries.getHdr().getVersion());
		printf("format: %s\n",dictionaries.getHdr().getFormat());
		printf("class: %s\n",dictionaries.getHdr().getClass());
		printf("location: %s\n",dictionaries.getHdr().getLocation());
		printf("object: %s\n",dictionaries.getHdr().getObject());

		// 적재한 owners 데이터 출력
		printf("(\n");
		for(size_t cnt=0;cnt<dictionaries.getSize();++cnt)
		{
			openfoam::Dictionary	dictionary	=	dictionaries[cnt];

			printf	(	"%s\n{\n"
						"    %-15s %s;\n"
						"}\n",
						dictionary.mName.c_str(),
						"type",
						dictionary.mType.c_str()
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
