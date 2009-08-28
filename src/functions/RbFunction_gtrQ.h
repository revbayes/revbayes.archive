/**
 * @file
 * This file contains the declaration of RbFunction_gtrQ, the
 * gtrQ() function that constructs a scaled instantaneous
 * rate matrix from two simplex parameters, one describing the
 * stationary state frequencies, the other the exchangeability
 * rates.
 *
 * Function: gtrQ
 *
 * Parameters:
 * pi      = stationary state frequencies
 * revrate = exchangeability rate parameters
 *
 * @brief Declaration of RbFunction_gtrQ
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

#ifndef RbFunction_gtrQ_H
#define RbFunction_gtrQ_H

#include "RbDataType.h"
#include "RbFunction.h"
#include "SyntaxLabeledExpr.h"
#include <list>

using namespace std;

/** This is the class for the gtrQ() function, which constructs a scaled
 *  instantaneous rate matrix from two simplex parameters, one describing
 *  the stationary state frequencies of the process and the other the
 *  exchangeability rates.
 *
 *  @package    functions
 *  @implements RbFunction
 */
class RbFunction_gtrQ :  public RbFunction {

    public:
            RbFunction_gtrQ() : RbFunction(), qmat(NULL) {} //!< Constructor checking argument rules
	        virtual ~RbFunction_gtrQ() {}          //!< Destructor, does nothing

#pragma mark Parser info
        static ArgumentRule*    argumentRules;                  //!< The argument rules
        ArgumentRule*           getArgumentRules() { return argumentRules; }    //!< Get argument rules

#pragma mark Public functions
        RbDataType* execute();                  //!< Get result
        RbDataType  getDataType();              //!< Get data type for type checking
        void        print(ostream &c) const;    //!< Print this object
        bool        setArguments(list<SyntaxElement *> args);      //!< Set and check arguments

    protected:
        RbQMatrix*  qmat;   //!< Convenient holder of the Q matrix result
};

#endif
