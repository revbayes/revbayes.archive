/**
 * @file
 * This file contains the declaration of RbFunction_c, the
 * c() function.
 *
 * @brief Declaration of RbFunction_c
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

#ifndef RbFunction_c_H
#define RbFunction_c_H

#include "RbDataType.h"
#include "RbFunction.h"
#include "SyntaxLabeledExpr.h"
#include <list>

using namespace std;

/** This is the class for the c() function, which takes any number
 *  of arguments, extracts their elements and puts them in a vector.
 *  We rely on the RbDataType as() function to do type conversion,
 *  and the getVectorType() function to get the appropriate vector
 *  version of the data type.
 *
 *  @package    functions
 *  @implements RbFunction
 */
class RbFunction_c :  public RbFunction {

    public:
            RbFunction_c() : RbFunction() {}    //!< Constructor
	        virtual ~RbFunction_c() {}          //!< Destructor, does nothing

#pragma mark Public functions
        RbDataType* execute();                  //!< Get result
        RbDataType  getDataType();              //!< Get data type for type checking
        void        print(ostream &c) const;    //!< Print this object
        bool        setArguments(list<SyntaxLabeledExpr *> args);      //!< Set and check arguments
};

#endif
