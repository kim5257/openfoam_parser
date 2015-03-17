/*
 * openfoam_debug.h
 *
 *  Created on: 2015. 3. 3.
 *      Author: asran
 */

#ifndef INCLUDE_OPENFOAM_DEBUG_H_
#define INCLUDE_OPENFOAM_DEBUG_H_

#include <stdio.h>

#define	DEBUG_PRINT_ON	(0)

#define	DEBUG_PRINT(args ...)	if(DEBUG_PRINT_ON)fprintf(stderr,##args)

#endif /* INCLUDE_OPENFOAM_DEBUG_H_ */
