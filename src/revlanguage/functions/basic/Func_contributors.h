#ifndef Func_contributors_H
#define Func_contributors_H

#include "RlFunction.h"

namespace RevLanguage {
    
    /**
     * @brief Func_contacts: print details of core contributors
     *
     * This function class prints the details of the core
     * contributors to the RevBayes project.
     */
    class Func_contributors : public Function {
        
    public:
        Func_contributors( void );
        
        // Basic utility functions
        Func_contributors*                      clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_contacts functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                        //!< Function may throw exceptions
        
        RevPtr<Variable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
