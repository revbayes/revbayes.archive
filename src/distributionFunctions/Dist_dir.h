/**
 * @file
 * This file contains the declaration of DistDir, the
 * REvBayes Dirichlet distribution function.
 *
 * @brief Declaration of DistDir
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef DistDir_H
#define DistDir_H

#include "ArgumentRule.h"
#include "Distribution.h"

using namespace std;

/** This is the class used for the gamma distribution.
 *
 *  @implements Distribution
 *  @package    distributions
 */
class DistDir :  public Distribution {

    public:
            DistDir() : Distribution() {}   //!< Constructor sets type to "d" in base class Distribution
	        virtual ~DistDir() {}           //!< Destructor does nothing

#pragma mark Parser info
        static ArgumentRule argRules[];     //!< The template argument rules
        ArgumentRule*       getTemplateArgRules() { return argRules; }  //!< Get template argument rules

#pragma mark Probability functions
        double          lnProbRatioObs();           //!< Ln probability ratio of observation(s)
        double          lnProbRatioParams();        //!< Ln probability ratio of parameter(s)
        RbVector*       pdf();                      //!< Probability density
        RbDataType*     rv();                       //!< Draw random variables
};

#endif
