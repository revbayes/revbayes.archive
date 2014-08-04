#ifndef UserProcedure_H
#define UserProcedure_H

#include "ArgumentRules.h"
#include "RlFunction.h"
#include "SyntaxElement.h"

#include <list>
#include <string>
#include <vector>

namespace RevLanguage {
    
    
    /**
     * The UserProcedure class holds information about the definition of a user procedure.
     * A procedure need not know how to create a deterministic variable, it just executes
     * the code in static procedure calls.
     */
    class UserProcedure :  public Function {
        
    public:
        UserProcedure(const ArgumentRules*          argRules,
                      const TypeSpec&               retType,
                      std::list<SyntaxElement*>*    stmts);                                 //!< Basic constructor
        UserProcedure(const UserProcedure& x);
        virtual                                     ~UserProcedure();                       //!< Destructor
        
        // Overloaded operators
        UserProcedure&                               operator=(const UserProcedure& f);     //!< Assignment operator
        
        // Basic utility functions
        UserProcedure*                              clone(void) const;                      //!< Clone the object
        static const std::string&                   getClassType(void);                     //!< Get type (static)
        static const TypeSpec&                      getClassTypeSpec(void);                 //!< Get type spec (static)
        const TypeSpec&                             getTypeSpec(void) const;                //!< Get type spec (dynamic)
        
        // Regular functions
        virtual RevPtr<Variable>                    execute(void);                          //!< Execute function
        const ArgumentRules&                        getArgumentRules(void) const;           //!< Get argument rules
        const std::list<SyntaxElement*>&            getCode(void) const;                    //!< Get a reference to the code
        const TypeSpec&                             getReturnType(void) const;              //!< Get type spec of return value
        
    protected:
        
        const ArgumentRules*                        argumentRules;                          //!< The argument rules
        const TypeSpec                              returnType;                             //!< The return type spec
        std::list<SyntaxElement*>*                  code;                                   //!< The code
    };
    
}

#endif

