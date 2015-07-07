/*
 * openfoam_templet.h
 *
 *  Created on: 2015. 6. 30.
 *      Author: asran
 */

#ifndef OPENFOAM_TEMPLET_H_
#define OPENFOAM_TEMPLET_H_

/// 문서 상단 주석
#define	OPENFOAM_TOP_COMMENT	\
"/*--------------------------------*- C++ -*----------------------------------*\\\n"\
"| =========                 |                                                 |\n"\
"| \\\\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |\n"\
"|  \\\\    /   O peration     | Version:  2.1                                   |\n"\
"|   \\\\  /    A nd           | Web:      www.OpenFOAM.org                      |\n"\
"|    \\\\/     M anipulation  |                                                 |\n"\
"\\*---------------------------------------------------------------------------*/\n"\
"/*   Windows 32 and 64 bit porting by blueCAPE: http://www.bluecape.com.pt   *\\\n"\
"|  Based on Windows porting (2.0.x v4) by Symscape: http://www.symscape.com   |\n"\
"\\*---------------------------------------------------------------------------*/\n"

// 헤더 이름
#define	OPENFOAM_HDR_START	\
"FoamFile\n"	\
"{\n"

#define	OPENFOAM_HDR_END		\
"}\n"

#define	HDR_FORMAT		"    %-12s%s;\n"
#define	VERSION		"version"
#define	FORMAT			"format"
#define	CLASS			"class"
#define	NOTE			"note"
#define	LOCAITON		"location"
#define	OBJECT			"object"

#define	OPENFOAM_MID_SEPARATE	\
"// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //"

#define	OPENFOAM_LAST_SEPARATE	\
"// ************************************************************************* //"

#define	OPENFOAM_DATA_START	"("
#define	OPENFOAM_DATA_END		")"

#endif /* OPENFOAM_TEMPLET_H_ */
