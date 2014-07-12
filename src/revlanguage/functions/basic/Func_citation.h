/**
 * @file
 * This file contains the declaration of Func_citation, which is
 * used to provide the user with information on how to cite the program.
 *
 * @brief Declaration of Func_citation
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#ifndef Func_citation_H
#define Func_citation_H

#include "RlFunction.h"

namespace RevLanguage {
    
    class Func_citation : public Function {
        
    public:
        Func_citation( void );
        
        // Basic utility functions
        Func_citation*                          clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_citation functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                        //!< Function may throw exceptions
        
        RevPtr<Variable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif
