/**
 * @file
 * This file contains the declaration of Func_source, which is
 * the function used to read commands (source) from a file.
 *
 * @brief Declaration of Func_source
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-22 12:06:58 -0700 (Sun, 22 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_source.h 1431 2012-04-22 19:06:58Z hoehna $
 */

#ifndef Func_source_H
#define Func_source_H

#include "RbFunction.h"


class Func_source :  public RbFunction {

    public:
        Func_source( void );
    
        // Basic utility functions
        Func_source*                            clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object

        // Func_source functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return true; }                                         //!< Function may throw exceptions
    
    protected:
        RbPtr<RbLanguageObject>                 executeFunction(const std::vector<const RbObject*>& args);                  //!< Execute function
    
};

#endif

