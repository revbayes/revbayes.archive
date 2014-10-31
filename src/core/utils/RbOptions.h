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

/* Debug switches */
/* It is useful to list the switches here but it is preferable to switch
   the defines on in the IDE rather than by uncommenting them here, so
   that accidental commits do not disturb other developers. Beware! */
//#define ASSERTIONS_ALL
//#define ASSERTIONS_TREE
//#define ASSERTIONS_DISTRIBUTIONS
//#define DEBUG_ALL
//#define DEBUG_PARSER
//#define DEBUG_WORKSPACE
//#define DEBUG_BISON_FLEX
//#define DEBUG_MCMC      // Define this to debug mcmc computation shortcuts (and perhaps other mcmc code). NB! Slow!
//#define DEBUG_MCMC_DETAILS   // Define this to see more detailed mcmc debug info
//#define DEBUG_HELP_SYSTEM
//#define DEBUG_RANDOM    // Define this to cause deterministic execution, bypassing time-generated seed for random number generators
#define DEBUG_MEMORY    // Defins this to debug memory leaks in the language layer

//#define TESTING

/* Feature enabling switches */
#define SSE_ENABLED
//#define AVX_ENABLED


/* Test whether we should use linenoise */
#if !defined (NO_LINENOISE)
#define USE_LIB_LINENOISE
#endif

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

    // switch debuggin g MCMC on
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


//#endif


// AdmixtureGraph depends on armadillo for linear algebra
// Uncomment the first line to enable the armadillo library
//#define USE_LIB_ARMADILLO
#ifdef USE_LIB_ARMADILLO
#include <armadillo>
#endif

// ParallelMcmcmc depends on the open mp library for multiprocessing
// Uncomment the first line to enable the open mp library (will include omp.h)
//#define USE_LIB_OPENMP
//#ifdef USE_LIB_OPENMP
//#endif

#endif
