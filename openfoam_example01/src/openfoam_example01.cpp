// openfoam_example01.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include <openfoam.h>

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		// owner ���� ����
		openfoam::Parser_labelList	parser("owner");
		openfoam::owner_list_t		owners;

		// ��� ���� �б�
		parser.readHdr();

		// ��� ���� ���
		printf("version: %s\n",parser.getHdr().getVersion());
		printf("format: %s\n",parser.getHdr().getFormat());
		printf("class: %s\n",parser.getHdr().getClass());
		printf("location: %s\n",parser.getHdr().getLocation());
		printf("object: %s\n",parser.getHdr().getObject());

		// ������ ���� �б�
		parser.readSize();

		// ������ ���� ���
		printf("Data Len: %lu\n", parser.getSize());

		// ������ ���� ��ŭ �ݺ�
		for(size_t cnt=0;cnt<parser.getSize();++cnt)
		{
			// owner �� ���� �� ����ü
			openfoam::Owner	owner;

			// �� �� owner �� �б�
			if( parser.readData() )
			{
				break;
			}

			// ���� owner �� ��������
			std::queue<double>	data	=	parser.getData();

			// owner ���� ����ü�� �ֱ�
			owner.mCell	=	(uint32_t)data.front();

			// owner ���� owners �ڷᱸ���� ����
			owners.push_back(owner);
			data.pop();
		}

		// ������ owners ������ ���
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
