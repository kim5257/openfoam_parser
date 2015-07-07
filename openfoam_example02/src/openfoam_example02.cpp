// openfoam_example01.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <openfoam.h>

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		openfoam::Owners	owners("owner", openfoam::Parser::FILE_READ);
		openfoam::Owners	owners2("copy_owner", openfoam::Parser::FILE_WRITE);

		// owner 헤더 및 데이터 읽기
		owners.readFile();

		owners2.copy(owners);

		owners2.writeFile();

		// 헤더 정보 출력
		printf("version: %s\n",owners.getHdr().getVersion());
		printf("format: %s\n",owners.getHdr().getFormat());
		printf("class: %s\n",owners.getHdr().getClass());
		printf("location: %s\n",owners.getHdr().getLocation());
		printf("object: %s\n",owners.getHdr().getObject());

		// 적재한 owners 데이터 출력
		printf("(\n");
		for(size_t cnt=0;cnt<owners.getSize();++cnt)
		{
			printf("%d\n", owners[cnt].mCell);
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

