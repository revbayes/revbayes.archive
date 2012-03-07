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

class Func_print :  public RbFunction {
    
public:
    // Basic utility functions
    Func_print*                 clone(void) const;                                  //!< Clone object
    static const std::string&   getClassName(void);                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                             //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
    
    // Func_print functions
    const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                          //!< Get type of return val
    
protected:
    void                        clearArguments(void);                               //!< Clear the arguments of this class
    const RbLanguageObject&     executeFunction(void);                              //!< Execute function
    void                        setArgumentVariable(const std::string& name, const Variable* var);
    
private:

    // Arguments
    RbConstVariablePtr          filename;
    RbConstVariablePtr          append;
    std::vector<RbConstVariablePtr>  elements;

};

#endif

