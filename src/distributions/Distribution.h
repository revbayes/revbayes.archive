/**
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
#include "SyntaxLabeledExpr.h"

using namespace std;

/** This is the base class used to hold distribution functions on
 *  real-valued parameters in the symbol table. These functions should
 *  come in four different flavors: the dxxx(), pxxx(), qxxx(), and
 *  rxxx() forms. The first one calculates densities, the second
 *  calculates cumulative probabilities, the third calculates quan-
 *  tiles, and the last generates random variables.
 *
 *  When a DistributionReal is added to the symbol table, four
 *  different function objects are inserted, one for each form of
 *  the function. The functionType member variable allows the execute
 *  method to find the right function to call for a particular function
 *  type. Similarly, the setArguments function behaves appropriately
 *  by checking the functionType member variable.
 *
 *  A special variant of the dxxx() form is used in tilde assignments,
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

        virtual RbVector*   cdf() = 0;                  //!< Cumulative density   
        virtual RbDataType* execute();                  //!< Call appropriate function based on functionType
        virtual RbDataType  getDataType() { return RbVector(); }  //!< Get data type for type checking
        virtual RbVector*   lnCdf() = 0;                //!< Ln cumulative density   
        virtual RbVector*   lnPdf() = 0;                //!< Ln probability density
        virtual RbVector*   pdf() = 0;                  //!< Probability density
        virtual void        print(ostream &c) const;    //!< Print this object
        virtual RbVector*   quantile() = 0;             //!< Calculate quantiles 
        virtual RbVector*   rv() = 0;                   //!< Draw random variables
        bool                setArguments(list<SyntaxLabeledExpr*> args);    //!< Set arguments based on function type
        void                setType(functionT type) { functionType=type; }  //!< Set function type

    protected:
        functionT        functionType;    //!< Holds function type
        bool             lnValue;         //!< Calculate ln prob instead of prob?
        RbVector*        vec;             //!< Holds result
};

#endif

RbDataType* DistributionReal::execute() {

    if (functionType == d) {
        if ( !lnValue )
            return pdf();
        else
            return lnPdf();
    }
    else if (functionType == p) {
        if ( !lnValue )
            return cdf();
        else
            return lnCdf();
    }
    else if (functionType == q) {
        return quantile();
        if ( !lnValue )
            return quantile();
        else
            return lnQuantile();
    }
    else if (functionType == r)
        return rv();

    return NULL;
}

bool DistributionReal::setArguments(list<SyntaxLabeledExpr*> args) {

    


}
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
