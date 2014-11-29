#ifndef Func_contributors_H
#define Func_contributors_H

#include "Procedure.h"

namespace RevLanguage {
    
    /**
     * @brief Procedure that provides information on contributors
     *
     * This procedure provides information on the history of the project
     * and contributors up to the summer of 2014.
     *
     * @author Fredrik Ronquist
     */
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
        bool                                    throws(void) const { return false; }                                        //!< Will not throw exceptions
        
        RevPtr<RevVariable>                     execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
