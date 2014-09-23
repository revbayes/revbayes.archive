/**
 * @file
 * This file contains the declaration of HelloWorld, which is
 * used to provide the user with basic help information.
 *
 * @brief Declaration of HelloWorld
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#ifndef HelloWorld_H
#define HelloWorld_H

#include "RlFunction.h"

namespace RevLanguage {
    
    class HelloWorld : public Function {
        
    public:
        HelloWorld( void );
        
        // Basic utility functions
        HelloWorld*                              clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // HelloWorld functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevPtr<Variable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
