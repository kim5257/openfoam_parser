// openfoam_example01.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

		// owner ��� �� ������ �б�
		owners.readFile();

		owners2.copy(owners);

		owners2.writeFile();

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

