#ifndef Func_history_H
#define Func_history_H

#include "Procedure.h"

namespace RevLanguage {
    
    /**
     * @brief Func_contacts: print contact details of core contributors
     *
     * This function class prints the contact details of the core
     * contributors to the RevBayes project.
     */
    class Func_contacts : public Procedure {
        
    public:
        Func_contacts( void );
        
        // Basic utility functions
        Func_contacts*                          clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_contacts functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                        //!< Function may throw exceptions
        
        RevPtr<RevVariable>                     execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
