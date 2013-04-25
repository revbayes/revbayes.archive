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
//#define ASSERTIONS_ALL
//#define ASSERTIONS_TREE
//#define ASSERTIONS_DISTRIBUTIONS
//#define DEBUG_ALL
//#define DEBUG_PARSER
//#define DEBUG_WORKSPACE
//#define DEBUG_BISON_FLEX
//#define DEBUG_MCMC
//#define DEBUG_HELP_SYSTEM
//#define TESTING


/* Test whether we need to debug everything. */
#if defined (DEBUG_ALL)

// switch all assertions on
#ifndef ASSERTIONS_ALL
#define ASSERTIONS_ALL
#endif

// switch debugging parser on
//#ifndef DEBUG_BISON_FLEX
//#define DEBUG_BISON_FLEX
//#endif

// switch debugging parser on
//#ifndef DEBUG_PARSER
//#define DEBUG_PARSER
//#endif

// switch debugging parser on
#ifndef DEBUG_WORKSPACE
#define DEBUG_WORKSPACE
#endif

// switch debugging MCMC on
#ifndef DEBUG_MCMC
#define DEBUG_MCMC
#endif

// switch debugging help on
#ifndef DEBUG_HELP_SYSTEM
#define DEBUG_HELP_SYSTEM
#endif

#endif




/* Test whether we need to debug everything. */
#if defined (ASSERTIONS_ALL)

// switch all assertions on
#ifndef ASSERTIONS_DISTRIBUTIONS
#define ASSERTIONS_DISTRIBUTIONS
#endif

#ifndef ASSERTIONS_TREE
#define ASSERTIONS_TREE
#endif

#endif




#endif
