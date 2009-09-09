/**
 * @file
 * This file contains the declaration of DistGamma, the
 * REvBayes gamma distribution function.
 *
 * @brief Declaration of DistGamma
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

#ifndef DistGamma_H
#define DistGamma_H

#include "ArgumentRule.h"
#include "DistributionReal.h"

using namespace std;

/** This is the class used for the gamma distribution.
 *
 *  @implements DistributionReal
 *  @package    distributions
 */
class DistGamma :  public DistributionReal {

    public:
            DistGamma() : DistributionReal() {} //!< Constructor sets type to "d" in base class Distribution
	        virtual ~DistGamma() {}             //!< Destructor does nothing

#pragma mark Parser info
        static ArgumentRule argRules[];     //!< The template argument rules
        ArgumentRule*       getTemplateArgRules() { return argRules; }  //!< Get template argument rules

#pragma mark Probability functions
        RbVector*       cdf();                      //!< Cumulative density   
        double          lnProbRatioObs();           //!< Ln probability ratio of observation(s)
        double          lnProbRatioParams();        //!< Ln probability ratio of parameter(s)
        RbVector*       pdf();                      //!< Probability density
        RbVector*       quantile();                 //!< Calculate quantile(s)
        RbDataType*     rv();                       //!< Draw random variables
};

#endif
