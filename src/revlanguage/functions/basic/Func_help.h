/**
 * @file
 * This file contains the declaration of Func_help, which is
 * used to provide the user with basic help information.
 *
 * @brief Declaration of Func_help
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#ifndef Func_help_H
#define Func_help_H

#include "Procedure.h"

namespace RevLanguage {
    
    class Func_help : public Procedure {
        
    public:
        Func_help( void );
        
        // Basic utility functions
        Func_help*                              clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_help functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevPtr<RevVariable>                     execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
