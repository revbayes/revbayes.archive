/**
 * @file
 * This file contains the declaration of RbFunction, which is
 * used to hold functions in the symbol table.
 *
 * @brief Declaration of RbFunction
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

#ifndef RbFunction_H
#define RbFunction_H

#include "ArgumentRule.h"
#include "RbObject.h"
#include "SyntaxElement.h"

using namespace std;

/** This is the class used to hold functions in the symbol table.
 *  It can be used both for pre-specified and user-defined functions.
 *  It can hold both distribution functions and regular functions;
 *  the latter may or may not be accessor functions. An accessor
 *  function needs to implement the setAttribute function so that
 *  we can set the relevant attribute, as specified in the derived
 *  interface RbAccessor. A distribution function needs to imple-
 *  ment a set of functions, as specified in the Distribution and
 *  DistributionReal classes.
 *
 *  @interface RbFunction
 */
class RbFunction :  public RbObject {

    public:
        enum functionT = { FUNCTION, ACCESSOR, DISTRIBUTION };

            RbFunction() : RbObject() {}                //!< Default constructor
            RbFunction(const ArgumentRule* argRules);   //!< Constructor that checks argument rules
	        virtual ~RbFunction() { delete result; }    //!< Destructor; delete result container

#pragma mark Parser info
        virtual ArgumentRule*   getArgumentRules() = 0; //!< Get argument rules

#pragma mark Public functions
        virtual RbDataType* execute() = 0;              //!< Get result
        virtual RbDataType  getDataType() = 0;          //!< Get data type for type checking
        functionT           getFunctionType() { return functionType; }     //!< Get function type
        virtual bool        setArguments(list<SyntaxElement *> args);      //!< Set and check arguments
        virtual void        print(ostream &c) const;    //!< Print this object
    
    protected:
        list<SyntaxElement*>    arguments;   //!< The arguments of the function (expressions)
        static functionT        functionType=FUNCTION;  //!< Holds function type
        RbDataType*             result;                 //!< Holds result
};

#endif
