/**
 * @file
 * This file contains the declaration of Distribution, which is
 * used to hold distribution functions in the symbol table. It
 * is also used for the distribution attribute of stochastic
 * DAG nodes (unless they are real-valued parameters, in which
 * case DistributionReal is the appropriate interface).
 *
 * @brief Declaration of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-28, version 1.0
 *
 * $Id$
 */

#ifndef Distribution_H
#define Distribution_H

#include "ArgumentRule.h"
#include "RbDataType.h"
#include "RbFunction.h"

using namespace std;

/** This is the base class used to hold distribution functions in
 *  the symbol table. All distribution functions should come in two
 *  different flavors: the d*() and r*() forms. The first one
 *  calculates densities, the second generates random variables.
 *  Distribution on real-valued observations should be derived from
 *  DistributionReal, and they come in two additional flavors:
 *  p*() and q*(), where the former calculates cumulative pro-
 *  babilities while the latter calculates quantiles.
 *
 *  When a Distribution is added to the symbol table, two different
 *  function objects are inserted, one for each form of the function.
 *  The functionType member variable allows the execute method to
 *  find the right function to call for a particular function type.
 *  Similarly, the getArgRules function adjusts the template argument
 *  rules after checking the functionType member variable.
 *
 *  A special variant of the d*() form is used in tilde assignments,
 *  when creating stochastic DAG nodes. It differs from the regular
 *  form mainly in that it does not expect an initial argument con-
 *  taining one or more observations.
 *
 *  @interface Distribution
 *  @package distributions
 */
class Distribution :  public RbFunction {

    public:
        enum functionT = { d, p, q, r, tilde_d };

            Distribution() : RbFunction(), completeArgRules(NULL), functionType(Distribution::d) {} //!< Make d*() form 
	        virtual ~Distribution();    //!< Delete completeArgRules if allocated

#pragma mark Parser info
        ArgumentRule*           getArgRules();              //!< Get complete argument rules based on template
        virtual ArgumentRule*   getTemplateArgRules() = 0;  //!< Get template argument rules

#pragma mark Setup functions
        virtual Distribution*   copy() = 0;                 //!< Return copy
        virtual RbDataType*     execute();                  //!< Call prob. function based on functionType
        virtual RbDataType      getDataType() = 0;          //!< Get data type for type checking
        virtual Distribution*   getTildeCopy(StochasticNode *x); //!< Get fast copy for tilde assign
        virtual void            setResultContainers() {}    //!< Set result containers
        virtual void            setType(functionT type);    //!< Set function type; do not allow p or q

#pragma mark Probability functions
        virtual double          lnProbRatioObs() = 0;       //!< Ln probability ratio of observation(s)
        virtual double          lnProbRatioParams() = 0;    //!< Ln probability ratio of parameter(s)
        virtual RbVector*       pdf() = 0;                  //!< Probability density
        virtual RbDataType*     rv() = 0;                   //!< Draw random variables

    protected:
        ArgumentRule*   completeArgRules;   //!< Container for complete argument rules
        functionT       functionType;       //!< Holds function type
        StochasticNode* stochasticNode;     //!< The stochastic node with this distribution (tilde_d)
};

#endif
