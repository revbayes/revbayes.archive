#ifndef Func_setwd_H
#define Func_setwd_H

#include "Procedure.h"

namespace RevLanguage {
    
    class Func_setwd : public Procedure {
        
    public:
        Func_setwd( void );
        
        // Basic utility functions
        Func_setwd*                             clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_source functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevPtr<RevVariable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

