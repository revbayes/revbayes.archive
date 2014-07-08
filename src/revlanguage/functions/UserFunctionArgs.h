#ifndef UserFunctionArgs_H
#define UserFunctionArgs_H

#include "ArgumentRules.h"
#include "DagNode.h"
#include "RlFunction.h"
#include "SyntaxElement.h"
#include "UserFunction.h"

#include <list>
#include <string>
#include <vector>

namespace RevLanguage {
    
    /**
     * UserFunctionArgs
     *
     * This class is used to hold the arguments of a userdefined Rev function.
     * It points to the definition to be able to retrieve the code of the function.
     *
     */
    class UserFunctionArgs :  public Function {
        
    public:
                                                    UserFunctionArgs(const UserFunction* f);
                                                    UserFunctionArgs(const UserFunctionArgs& x);
        virtual                                    ~UserFunctionArgs();
        
        // Overloaded operators
        UserFunctionArgs&                           operator=(const UserFunctionArgs& f);
        
        // Basic utility functions
        UserFunctionArgs*                           clone(void) const;                                                          //!< Clone the object
        static const std::string&                   getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language type of the object

        // Regular functions
        virtual RevPtr<Variable>                    execute(void);                                                              //!< Execute function
        const ArgumentRules&                        getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                             getReturnType(void) const;                                                  //!< Get type of return value
        
    protected:
        const UserFunction*                         userFunction;                       //!< Pointer to the user function
    };
    
}

#endif

