/**
 * @file
 * This file contains the declaration of Func_print, which 
 * prints some content.
 *
 * @brief Declaration of Func_print
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 20012-02-19
 *
 * $Id$
 */

#ifndef Func_print_H
#define Func_print_H

#include "RbFunction.h"

const std::string Func_print_name = "print function";

class Func_print :  public RbFunction {
    
public:
    // Basic utility functions
    Func_print*                 clone(void) const;                                  //!< Clone object
    const VectorString&         getClass(void) const;                               //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
    
    // Func_print functions
    const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                          //!< Get type of return val
    
protected:
    const RbLanguageObject&     executeFunction(void);                              //!< Execute function
    
private:
    static const TypeSpec       typeSpec;
    static const TypeSpec       returnTypeSpec;
};

#endif

