/*
 * openfoam_debug.h
 *
 *  Created on: 2015. 3. 3.
 *      Author: asran
 */

#ifndef INCLUDE__OPENFOAM_DEBUG_H_
#define INCLUDE__OPENFOAM_DEBUG_H_

#include <stdio.h>

#define	DEBUG_PRINT_ON	(0)

#define	DEBUG_PRINT(args ...)	if(DEBUG_PRINT_ON)fprintf(stderr,##args)

#endif /* INCLUDE__OPENFOAM_DEBUG_H_ */
