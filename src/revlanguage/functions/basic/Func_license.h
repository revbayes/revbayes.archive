/**
 * @file
 * This file contains the declaration of Func_license, which is
 * used to provide the user with license information.
 *
 * @brief Declaration of Func_license
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#ifndef Func_license_H
#define Func_license_H

#include "Procedure.h"

namespace RevLanguage {
    
    class Func_license : public Procedure {
        
    public:
        Func_license( void );
        
        // Basic utility functions
        Func_license*                           clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_license functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevPtr<Variable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
