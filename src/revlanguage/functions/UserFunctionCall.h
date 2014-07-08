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
                                                    UserFunctionCall(const UserFunction* fxn);
                                                    UserFunctionCall(const UserFunctionCall& x);
        virtual                                    ~UserFunctionCall();
        
        // Overloaded operators
        UserFunctionCall&                           operator=(const UserFunctionCall& f);
        
        // Basic utility functions
        UserFunctionCall*                           clone(void) const;                                                          //!< Clone the object
        static const std::string&                   getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Regular functions
        virtual RevPtr<Variable>                    execute(void);                                                              //!< Execute function
        const ArgumentRules&                        getArgumentRules(void) const;                                               //!< Get argument rules
        std::vector<const RevBayesCore::DagNode*>   getParameters(void) const;                                                  //!< Get parameters
        const TypeSpec&                             getReturnType(void) const;                                                  //!< Get type of return value
        
    protected:
        
        // Member variables
        const UserFunction*                         userFunction;                       //!< Smart pointer to function definition (access to argument rules etc)
        Environment*                                functionFrame;                      //!< Function frame with arguments
    };
    
}

#endif
