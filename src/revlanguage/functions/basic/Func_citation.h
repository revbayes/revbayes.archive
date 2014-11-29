#ifndef Func_citation_H
#define Func_citation_H

#include "Procedure.h"

namespace RevLanguage {
    
    /**
     * @brief Procedure that provides information on citation
     *
     * This procedure provides information on how to cite the program.
     *
     * @author Fredrik Ronquist
     */
    class Func_citation : public Procedure {
        
    public:
        Func_citation(void);                                                                                                //!< Constructor
        
        // Basic utility functions
        Func_citation*                          clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type (static)
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get Rev type spec (static)
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get Rev type spec (dynamic)

        // Func_citation functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                        //!< Will not throw exceptions
        
        RevPtr<RevVariable>                     execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
