/**
 * @file
 * This file contains the declaration of UserFunction, which is used
 * to hold user defined functions.
 *
 * @brief Declaration of UserFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-15 18:59:11 +0200 (Tue, 15 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: MemberFunction.h 1544 2012-05-15 16:59:11Z hoehna $
 */

#ifndef UserFunction_H
#define UserFunction_H

#include "ArgumentRules.h"
#include "RlFunction.h"
#include "SyntaxElement.h"

#include <list>
#include <string>
#include <vector>

namespace RevLanguage {
    
    class UserFunction :  public Function {
        
    public:
        UserFunction(const ArgumentRules*        argRules,
                     const TypeSpec&             retType,
                     std::list<SyntaxElement*>*  stmts,
                     Environment*                defineEnv);           //!< Constructor        
        UserFunction(const UserFunction& x);                                    //!< Copy constructor
        virtual ~UserFunction();                                                //!< Delete the code
        
        // overloaded operators
        UserFunction&                               operator=(const UserFunction& f);
 
        // Basic utility functions
        UserFunction*                               clone(void) const;                                                          //!< Clone the object
        static const std::string&                   getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Regular functions   
        virtual RbLanguageObject*                   execute(void);                                                              //!< Execute function
        const ArgumentRules&                        getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                             getReturnType(void) const;                                                  //!< Get type of return value
        void                                        setArgumentVariable(const std::string& name, const RbPtr<const Variable> &var);             
        
        
    protected:
        const ArgumentRules*                        argumentRules;                      //!< The argument rules
        const TypeSpec                              returnType;                         //!< The return type (complete specification)
        std::list<SyntaxElement*>*                  code;                               //!< The code
        Environment*                                defineEnvironment;                  //!< The definition environment
        
    };
    
}

#endif

