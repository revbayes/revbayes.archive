/**
 * @file
 * This file contains the declaration of Func_contributors, which is
 * used to provide the user with information on contributors.
 *
 * @brief Declaration of Func_contributors
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#ifndef Func_contributors_H
#define Func_contributors_H

#include "Procedure.h"

namespace RevLanguage {
    
    class Func_contributors : public Procedure {
        
    public:
        Func_contributors( void );
        
        // Basic utility functions
        Func_contributors*                      clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_contributors functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevPtr<RevVariable>                     execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
