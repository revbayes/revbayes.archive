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

    /**
     * The UserFunction class holds information about the definition of a user function. Two help classes,
     * UserFunctionCall and UserFunctionArgs are used to populate a RevLanguage::DeterministicNode together
     * with a TypedUserFunction to make a deterministic variable using the user function.
     */
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
        static const std::string&                   getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language type of the object

        // Regular functions   
        virtual RevPtr<Variable>                    execute(void);                                                              //!< Execute function (create det var)
        virtual RevPtr<Variable>                    executeCode(void);                                                          //!< Execute function (execute code)
        const ArgumentRules&                        getArgumentRules(void) const;                                               //!< Get argument rules
        const std::list<SyntaxElement*>&            getCode(void) const;                                                        //!< Get a pointer to the code
        std::set<const RevBayesCore::DagNode*>      getParameters(void) const;                                                  //!< Get parameters
        const TypeSpec&                             getReturnType(void) const;                                                  //!< Get type of return value
        
    protected:

        const ArgumentRules*                        argumentRules;                      //!< The argument rules
        const TypeSpec                              returnType;                         //!< The return type (complete specification)
        std::list<SyntaxElement*>*                  code;                               //!< The code
    };
    
}

#endif

