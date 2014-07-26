#ifndef UserFunctionCall_H
#define UserFunctionCall_H

#include "ArgumentRules.h"
#include "DagNode.h"
#include "RlFunction.h"
#include "SyntaxElement.h"
#include "UserFunction.h"

#include <list>
#include <string>
#include <vector>

namespace RevLanguage {
    
    class UserFunctionCall :  public Function {
        
    public:
                                                    UserFunctionCall( UserFunction* fxn);
                                                    UserFunctionCall(const UserFunctionCall& x);
        virtual                                    ~UserFunctionCall();
        
        // Overloaded operators
        UserFunctionCall&                           operator=(const UserFunctionCall& f);
        
        // Basic utility functions
        UserFunctionCall*                           clone(void) const;                                                          //!< Clone the object
        static const std::string&                   getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Regular functions
        virtual RevPtr<Variable>                    execute(void);                                                              //!< Execute function
        const ArgumentRules&                        getArgumentRules(void) const;                                               //!< Get argument rules
        std::set<const RevBayesCore::DagNode*>      getParameters(void) const;                                                  //!< Get parameters
        const TypeSpec&                             getReturnType(void) const;                                                  //!< Get type of return value
        
    protected:
        
        // Member variables
        UserFunction*                               userFunction;                       //!< Pointer to function definition (access to argument rules etc)
        Environment*                                functionFrame;                      //!< Function execution frame with arguments
    };
    
}

#endif
