/**
 * @file
 * This file contains the declaration of RbAccessor, the
 * abstract base class for REvBayes accessor functions.
 *
 * @brief Declaration of RbAccessor
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

#ifndef RbAccessorFxn_H
#define RbAccessorFxn_H

#include "RbDataType.h"
#include "RbStandardFxn.h"
#include "SyntaxLabeledExpr.h"
#include <list>

using namespace std;

/** This is the insterface class for accessor functions, which can be used
 *  both to access attributes and to set them.
 *
 *  @package    functions
 *  @implements RbFunction (partially)
 */
class RbAccessorFxn :  public RbStandardFxn {

    public:
            RbAccessorFxn() : RbStandardFxn() {}    //!< Default constructor, set arguments later
            RbAccessorFxn(const RbAccessorFxn& s);  //!< Copy constructor
	        virtual ~RbAccessorFxn() {}             //!< Destructor, does nothing

#pragma mark Parser help functions
        static const ArgumentRule argRules[];   //!< Argument rules, the same for all accessor functions
        virtual RbAccessorFxn*    copy() const = 0;                     //!< Return fast copy
        const ArgumentRule*       getArgRules() { return argRules; }    //!< Get argument rules
        virtual const string&     getDataType() const = 0;              //!< Get data type of result

#pragma mark Regular functions
        virtual RbDataType* execute() = 0;                                  //!< Get result (get attribute)
        virtual bool        setAttribute(const RbDataType *newValue) = 0;   //!< Set attribute
};

#endif
