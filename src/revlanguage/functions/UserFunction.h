#ifndef UserFunction_H
#define UserFunction_H

#include "ArgumentRules.h"
#include "DagNode.h"
#include "RlFunction.h"
#include "SyntaxElement.h"

#include <list>
#include <string>
#include <vector>

namespace RevLanguage {

    class UserFunctionArgs;
    class UserFunctionCall;

    class UserFunction :  public Function {
        
    public:
                                                    UserFunction(const ArgumentRules*        argRules,
                                                                 const TypeSpec&             retType,
                                                                 std::list<SyntaxElement*>*  stmts);
                                                    UserFunction(const UserFunction& x);
        virtual                                    ~UserFunction();
        
        // Overloaded operators
        UserFunction&                               operator=(const UserFunction& f);
 
        // Basic utility functions
        UserFunction*                               clone(void) const;                                                          //!< Clone the object
        static const std::string&                   getClassType(void);                                                         //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language type of the object

        // Regular functions   
        virtual RevPtr<Variable>                    execute(void);                                                              //!< Execute function
        const ArgumentRules&                        getArgumentRules(void) const;                                               //!< Get argument rules
        const std::list<SyntaxElement*>&            getCode(void) const;                                                        //!< Get a pointer to the code
        const TypeSpec&                             getReturnType(void) const;                                                  //!< Get type of return value
        
    protected:

        const ArgumentRules*                        argumentRules;                      //!< The argument rules
        const TypeSpec                              returnType;                         //!< The return type (complete specification)
        std::list<SyntaxElement*>*                  code;                               //!< The code
    };
    
}

#endif

