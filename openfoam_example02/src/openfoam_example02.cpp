// openfoam_example01.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "openfoam_parser_labellist.h"
#include "openfoam_error.h"

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		openfoam::Parser_labelList	parser("neighbour");

		parser.readHdr();

		printf("version: %s\n",parser.getHdr().getVersion());
		printf("format: %s\n",parser.getHdr().getFormat());
		printf("class: %s\n",parser.getHdr().getClass());
		printf("note: %s\n",parser.getHdr().getNote());
		printf("location: %s\n",parser.getHdr().getLocation());
		printf("object: %s\n",parser.getHdr().getObject());

		size_t	cnt		=	0;

		parser.getSize();

		while(!parser.readData())
		{
			std::queue<double>	data	=	parser.getData();

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

