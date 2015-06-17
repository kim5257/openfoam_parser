// openfoam_example01.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <openfoam.h>

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		// owner 파일 열기
		openfoam::Parser_labelList	parser("owner");
		openfoam::owner_list_t		owners;

		// 헤더 정보 읽기
		parser.readHdr();

		// 헤더 정보 출력
		printf("version: %s\n",parser.getHdr().getVersion());
		printf("format: %s\n",parser.getHdr().getFormat());
		printf("class: %s\n",parser.getHdr().getClass());
		printf("location: %s\n",parser.getHdr().getLocation());
		printf("object: %s\n",parser.getHdr().getObject());

		// 데이터 갯수 읽기
		parser.readSize();

		// 데이터 갯수 출력
		printf("Data Len: %lu\n", parser.getSize());

		// 데이터 갯수 만큼 반복
		for(size_t cnt=0;cnt<parser.getSize();++cnt)
		{
			// owner 값 저장 할 구조체
			openfoam::Owner	owner;

			// 한 개 owner 값 읽기
			if( parser.readData() )
			{
				break;
			}

			// 읽은 owner 값 가져오기
			std::queue<double>	data	=	parser.getData();

			// owner 값을 구조체에 넣기
			owner.mCell	=	(uint32_t)data.front();

			// owner 값을 owners 자료구조에 적재
			owners.push_back(owner);
			data.pop();
		}

		// 적재한 owners 데이터 출력
		printf("(\n");
		for(size_t cnt=0;cnt<owners.size();++cnt)
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
