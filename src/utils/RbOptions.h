/**
 * @file
 * This file contains global RevBayes options.
 *
 * @brief Global options
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RbOptions_H
#define RbOptions_H

/* Flag for generating debugging info */
#define DEBUG_PARSER

/* The magic PRINTF defines */
#if defined (DEBUG_PARSER)
#define PRINTF printf
#else
#define PRINTF(...)
#endif

#endif
