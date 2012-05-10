/**
 * @file
 * This file contains the declaration of Func_rep, which is used
 * to replicate a variable n times.
 *
 * @brief Declaration of Func_rep
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 10:23:29 -0700 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_rep.h 1411 2012-04-20 17:23:29Z hoehna $
 */

#ifndef Func_rep_H
#define Func_rep_H

#include "RbFunction.h"
#include "RbVector.h"

#include <string>


class Func_rep :  public RbFunction {
    
public:
    Func_rep( void );
    
    // Basic utility functions
    Func_rep*                   clone(void) const;                                          //!< Clone the object
    static const std::string&   getClassName(void);                                         //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
    
    // Regular functions
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
    
protected:
    RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);  //!< Execute function
    
};

#endif

