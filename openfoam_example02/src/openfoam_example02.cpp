// openfoam_example01.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include <owners.h>

int		main	(	int 	argc,
					char*	argv[]
				)
{
	try
	{
		openfoam::Owners	owners("owner");

		// owner ��� �� ������ �б�
		owners.prepare();

		// ��� ���� ���
		printf("version: %s\n",owners.getHdr().getVersion());
		printf("format: %s\n",owners.getHdr().getFormat());
		printf("class: %s\n",owners.getHdr().getClass());
		printf("location: %s\n",owners.getHdr().getLocation());
		printf("object: %s\n",owners.getHdr().getObject());

		// ������ owners ������ ���
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

