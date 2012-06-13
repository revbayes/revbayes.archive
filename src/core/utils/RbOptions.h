/**
 * @file
 * This file contains global RevBayes options.
 *
 * @brief Global options
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RbOptions_H
#define RbOptions_H

/* The debug switches */
//#define DEBUG_ALL
//#define DEBUG_PARSER
#define ASSERTIONS_ALL
//#define ASSERTIONS_DISTRIBUTIONS


/* Test whether we need to debug everything. */
#if defined (DEBUG_ALL)

// switch all assertions on
#ifndef ASSERTIONS_ALL
#define ASSERTIONS_ALL
#endif

// switch debuggng parser on
#ifndef DEBUG_PARSER
#define DEBUG_PARSER
#endif

#endif


/* Debug the parser */
#if defined (DEBUG_PARSER)
#   define PRINTF printf
#else
#   define PRINTF(...)
#endif


/* Test whether we need to debug everything. */
#if defined (ASSERTIONS_ALL)

// switch all assertions on
#ifndef ASSERTIONS_DISTRIBUTIONS
#define ASSERTIONS_DISTRIBUTIONS
#endif

#endif




#endif
