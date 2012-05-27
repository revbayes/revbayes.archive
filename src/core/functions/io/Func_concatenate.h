/**
 * @file
 * This file contains the declaration of Func_concatenate, which is used to concatenate character matrices
 * 
 * @brief Declaration of Func_concatenate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-25 01:17:07 -0800 (Sat, 25 Feb 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_concatenate.h 1247 2012-02-25 09:17:07Z hoehna $
 */

#ifndef Func_concatenate_H
#define Func_concatenate_H

#include "RbFunction.h"



class Func_concatenate :  public RbFunction {
    
    public:
        // Basic utility functions
        Func_concatenate*           clone(void) const;                                                       //!< Clone the object
        static const std::string&   getClassName(void);                                                      //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                                  //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                                 //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                                            //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                               //!< Get type of return value

    protected:
        RbPtr<RbLanguageObject>     executeFunction(const std::vector<const RbObject*>& args);              //!< Execute function

};

#endif

