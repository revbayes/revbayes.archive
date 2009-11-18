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
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#ifndef RbFunction_c_H
#define RbFunction_c_H

#include "RbDataType.h"
#include "RbStandardFxn.h"
#include "SymbolTable.h"
#include "SyntaxLabeledExpr.h"
#include <vector>

/** This is the class for the c() function, which takes any number
 *  of arguments, extracts their elements and puts them in a vector.
 *  We rely on the RbDataType as() function to do type conversion,
 *  and the getVectorType() function to get the appropriate vector
 *  version of the data type.
 *
 *  @package    functions
 *  @implements RbFunction, RbStandardFxn
 */
class RbFunction_c :  public RbStandardFxn {

    public:
            RbFunction_c();                             //!< Default constructor, allocate workspace
            RbFunction_c(const RbFunction_c& s);        //!< Copy constructor
	        ~RbFunction_c();                            //!< Destructor, delete workspace

#pragma mark Parser help functions
        RbFunction_c*       copy() const;                                   //!< Return copy
        RbDataType*         execute();                                      //!< Get result
        const std::string&  getDataType() const;                            //!< Get data type of result
        void                print(std::ostream &c) const;                   //!< Print this object
        bool                setArguments(std::vector<SyntaxLabeledExpr *> args);    //!< Set and check arguments

    protected:
        RbDataType*     resultVec;      //!< Workspace for result
};

#endif
