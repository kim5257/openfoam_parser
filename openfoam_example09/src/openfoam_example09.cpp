/*
 * openfoam_example08.cpp
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
		openfoam::Scalarfields	scalarField("p", openfoam::Parser::FILE_READ);
		openfoam::Scalarfields	scalarField2;

		scalarField2.open("copy_p", openfoam::Parser::FILE_WRITE);

		// owner 헤더 및 데이터 읽기
		scalarField.readFile();
		scalarField2.copy(scalarField);
		scalarField2.writeFile();

		// 헤더 정보 출력
		printf("version: %s\n",scalarField.getHdr().getVersion());
		printf("format: %s\n",scalarField.getHdr().getFormat());
		printf("class: %s\n",scalarField.getHdr().getClass());
		printf("location: %s\n",scalarField.getHdr().getLocation());
		printf("object: %s\n",scalarField.getHdr().getObject());

		// 적재한 owners 데이터 출력
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
