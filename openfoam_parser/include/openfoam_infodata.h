/*
 * openfoam_infodata.h
 *
 *  Created on: 2015. 7. 22.
 *      Author: asran
 */

#ifndef OPENFOAM_INFODATA_H_
#define OPENFOAM_INFODATA_H_

#include <map>
#include <string>

namespace openfoam
{

class	InfoData
{
public:
	typedef	std::map<std::string, std::vector<std::string> >	info_data_elem;
	typedef	info_data_elem::iterator		info_data_elem_itor;
private:
	std::string		mHdr;
	info_data_elem	mData;
public:
				InfoData(){}
				inline InfoData	(	std::string	hdr		);
	virtual	~InfoData	(	void	){}
public:
	inline void	setSingleData	(	std::string	name,
										std::string	val
									);
	inline void	setData	(	std::string					name,
									const std::vector<std::string>&		val
								);
	inline std::string	getSingleData	(	std::string	name	);
	inline std::vector<std::string>	getData	(	std::string	name	);
	inline std::string	getHdr			(	void	) const;
	inline info_data_elem&	getAllData	(	void	);
};

InfoData::InfoData	(	std::string	hdr		)
:mHdr(hdr)
{
}

void	InfoData::setSingleData	(	std::string	name,
										std::string	val
									)
{
	if( mData[name].size() == 0 )
	{
		mData[name].push_back(val);
	}
	else
	{
		mData[name][0] = val;
	}
}

void	InfoData::setData		(	std::string					name,
									const std::vector<std::string>&	val
								)
{
	mData[name] = val;
}

std::string	InfoData::getSingleData	(	std::string	name	)
{
	return	mData[name][0];
}

std::vector<std::string>	InfoData::getData		(	std::string	name	)
{
	return	mData[name];
}

std::string	InfoData::getHdr		(	void	) const
{
	return	mHdr;
}

InfoData::info_data_elem&
				InfoData::getAllData	(	void	)
{
	return	mData;
}

}

#endif /* OPENFOAM_INFODATA_H_ */
