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
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Distribution_H
#define Distribution_H

#include "ArgumentRule.h"
#include "RbObject.h"
#include "SyntaxElement.h"

using namespace std;

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
        enum functionT = { d, p, q, r, tilde_d };

            Distribution() : RbFunction(), functionType(Distribution::d) {}  //!< Constructor sets type to "d"
	        virtual ~Distribution() {}    //!< Destructor does nothing

        virtual RbDataType* execute();                  //!< Call appropriate function based on functionType
        virtual RbDataType  getDataType() = 0;          //!< Get data type for type checking
        virtual RbVector*   lnPdf() = 0;                //!< Ln probability density
        virtual RbVector*   pdf() = 0;                  //!< Probability density
        virtual void        print(ostream &c) const;    //!< Print this object
        virtual RbDataType* rv() = 0;                   //!< Draw random variables
        bool                setArguments(list<SyntaxLabeledExpr*> args);    //!< Set arguments based on function type
        void                setType(functionT type) { functionType=type; }  //!< Set function type

    protected:
        functionT        functionType;    //!< Holds function type
        bool             lnValue;         //!< Calculate ln prob instead of prob?
};

#endif

/** Get result depending on distribution type */
RbDataType* Distribution::execute() {

    if ( functionType == d ) {
        if ( !lnValue )
            return pdf();
        else
            return lnPdf();
    }
    else if ( functionType == r )
        return rv();

    return NULL;
}

/** Set arguments depending on distribution type */
bool Distribution::setArguments(list<SyntaxLabeledExpr*> args) {

    ArgumentRule*   argRules = getArgumentRules();

    if ( functionType == d ) {
        if ( args.begin()->getExpression()
    }
    else if ( functionType == tilde_d {
    }
    else if ( functionType == r ) {
    
    }
}
