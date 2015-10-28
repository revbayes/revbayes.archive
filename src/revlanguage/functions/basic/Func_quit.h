#ifndef Func_quit_H
#define Func_quit_H

#include "Procedure.h"

namespace RevLanguage {
    
    class Func_quit : public Procedure {
        
    public:
        Func_quit( void );
        
        // Basic utility functions
        Func_quit*                              clone(void) const;                                          //!< Clone object
        static const std::string&               getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                     //!< Get class type spec
        const std::string&                      getFunctionName(void) const;                                //!< Get the primary name of the function in Rev
        const TypeSpec&                         getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Func_quit functions
        const ArgumentRules&                    getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                        //!< Function may throw exceptions
        
        RevPtr<RevVariable>                     execute(void);                                              //!< Execute function
        
    };
    
}

#endif

