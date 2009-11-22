/** 
 * @file
 * This file contains the declaration of Distribution, which is
 * used to hold distribution functions in the symbol table. It
 * is also used for the distribution attribute of stochastic
 * DAG nodes.
 *
 * @brief Declaration of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Distribution_H
#define Distribution_H

#include <ostream>

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "RbFunction.h"
#include "RbObject.h"


/** This is the base class used to hold distribution functions in
 *  the symbol table. All distribution functions should come in two
 *  different flavors: the dxxx() and rxxx() forms. The first one
 *  calculates densities, the second generates random variables.
 *  Distribution on real-valued observations should be derived from
 *  DistributionReal, and they come in two additional flavors:
 *  pxxx() and qxxx(), where the former calculates cumulative pro-
 *  babilities while the latter calculates quantiles. Distribution
 *  functions are also used in tilde assignments to create stochas-
 *  tic DAG nodes in a model graph.
 *
 *  @interface Distribution
 *  @package distributions
 */
class Distribution :  public RbFunction {

    public:
            typedef enum { d, p, q, r, tilde_d } functionT;

            Distribution(const ArgumentRule** rules, const std::string& valueType)
                 : RbFunction(rules, valueType), functionType(Distribution::d) {}  //!< Constructor, type to "d"

        // Basic utility functions
        virtual void        print(std::ostream &c) const;       //!< Print this object

        // Regular functions
        virtual RbObject*   execute(std::vector<DAGNode*> arguments);   //!< Call appropriate function
        std::vector<DAGNode*>   getArguments() const;                   //!< Get arguments to function
        virtual double      lnPdf(RbObject* value) = 0;                 //!< Ln probability density
        virtual double      pdf(RbObject* value) = 0;                   //!< Probability density
        virtual RbObject*   rv() = 0;                                   //!< Draw random value
        bool                setArguments(std::vector<DAGNode*> args);   //!< Set arguments based on function type
        void                setType(functionT type) { functionType=type; }  //!< Set function type

    protected:
        functionT        functionType;    //!< Holds function type
        bool             lnValue;         //!< Calculate ln prob instead of prob?
};

#endif

