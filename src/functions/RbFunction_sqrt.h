/**
 * @file
 * This file contains the declaration of RbFunction_sqrt, the
 * sqrt() function.
 *
 * @brief Declaration of RbFunction_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-26, version 1.0
 *
 * $Id$
 */

#ifndef RbFunction_sqrt_H
#define RbFunction_sqrt_H

#include "RbDataType.h"
#include "RbFunction.h"
#include "SyntaxLabeledExpr.h"
#include <list>

using namespace std;

/** This is the class for the sqrt() function, which takes a single
 *  vector (or scalar) of reals or ints.
 *
 *  @package    functions
 *  @implements RbFunction
 */
class RbFunction_sqrt :  public RbFunction {

    public:
            RbFunction_sqrt() : RbFunction(), vec(NULL) {} //!< Constructor
	        virtual ~RbFunction_sqrt() {}          //!< Destructor, does nothing

#pragma mark Parser info
        static const ArgumentRule*  argumentRules;          //!< The argument rules
        ArgumentRule*               getArgumentRules( return argumentRules; }   //!< Get argument rules  

#pragma mark Public functions
        RbDataType* execute();                  //!< Get result
        RbDataType  getDataType();              //!< Get data type for type checking
        bool        setArguments(list<SyntaxLabeledExpr*> args);    //! Set and check arguments
        void        print(ostream &c) const;    //!< Print this object

    protected:
        RbVector*   vec;        //!< Convenient pointer to container for result
};

#endif
