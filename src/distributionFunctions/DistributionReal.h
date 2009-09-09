/**
 * @file
 * This file contains the declaration of DistributionReal, which
 * is used to hold distribution functions on real-valued para-
 * meters in the symbol table. It is also used for the distri-
 * bution attribute of stochastic DAG nodes of real-valued type.
 *
 * @brief Declaration of DistributionReal
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

#ifndef DistributionReal_H
#define DistributionReal_H

#include "ArgumentRule.h"
#include "Distribution.h"

using namespace std;

/** This is the base class used to hold distribution functions on
 *  real-valued parameters in the symbol table. These functions should
 *  come in four different flavors: the d*(), p*(), q*(), and
 *  r*() forms. The first one calculates densities, the second
 *  calculates cumulative probabilities, the third calculates quan-
 *  tiles, and the last generates random variables.
 *
 *  When a DistributionReal is added to the symbol table, four
 *  different function objects are inserted, one for each form of
 *  the function. The functionType member variable allows the execute
 *  method to find the right function to call for a particular function
 *  type. Similarly, the getArgRules function behaves appropriately
 *  by checking the functionType member variable.
 *
 *  A special variant of the d*() form is used in tilde assignments,
 *  when creating stochastic DAG nodes. It differs from the regular
 *  form mainly in that it does not expect an initial argument con-
 *  taining one or more observations.
 *
 *  @interface DistributionReal
 *  @package distributions
 */
class DistributionReal :  public Distribution {

    public:
            DistributionReal() : Distribution() {}  //!< Constructor sets type to "d" in base class Distribution
	        virtual ~DistributionReal() {}    //!< Destructor does nothing

#pragma mark Parser info
        virtual ArgumentRule*   getTemplateArgRules() = 0;  //!< Get template argument rules

#pragma mark Setup functions
        RbDataType*             execute();                  //!< Call appropriate function based on functionType
        RbDataType              getDataType() { return RbVector(); }  //!< Get data type for type checking
        void                    setResultContainers();      //!< Set result containers
        void                    setType(functionT type) { functionType=type; }  //!< Set function type

#pragma mark Probability functions
        virtual RbVector*       cdf() = 0;                  //!< Cumulative density   
        virtual double          lnProbRatioObs() = 0;       //!< Ln probability ratio of observation(s)
        virtual double          lnProbRatioParams() = 0;    //!< Ln probability ratio of observation(s)
        virtual RbVector*       pdf() = 0;                  //!< Probability density
        virtual RbVector*       quantile() = 0;             //!< Calculate quantile(s)
        virtual RbDataType*     rv() = 0;                   //!< Draw random variables

    protected:
        RbVector*        vec;             //!< Convenient pointer to result
};

#endif
