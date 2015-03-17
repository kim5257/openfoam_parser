// openfoam_example01.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "OpenFOAM_parser.h"
#include "openfoam_error.h"

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		openfoam::OpenFOAM_Parser	parser("faces");

		//parser.readHdr();
		parser.readDataStart();

		printf("version: %s\n",parser.getHdr().getVersion());
		printf("format: %s\n",parser.getHdr().getFormat());
		printf("class: %s\n",parser.getHdr().getClass());
		printf("location: %s\n",parser.getHdr().getLocation());
		printf("object: %s\n",parser.getHdr().getObject());

		printf("Data Len: %lu\n", parser.getDataLen());

		size_t	cnt	=	0;

		while(!parser.readData())
		{
			std::queue<double>&	data	=	parser.getData();
			printf("%lu(", ++cnt);
			while( !data.empty() )
			{
				printf("%lf ", data.front());
				data.pop();
			}
			printf(")\n");
		}

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

