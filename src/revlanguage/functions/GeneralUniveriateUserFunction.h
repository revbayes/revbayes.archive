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

#ifndef GeneralUnivariateUserFunction_H
#define GeneralUnivariateUserFunction_H

#include "ArgumentRules.h"
#include "RlUnivariateFunction.h"

#include <list>
#include <string>
#include <vector>

namespace RevLanguage {
    
    class SyntaxElement;
    
    class GeneralUnivariateUserFunction :  public UnivariateFunction {
        
    public:
        GeneralUnivariateUserFunction(const ArgumentRules*        argRules,
                     const TypeSpec&             retType,
                     std::list<SyntaxElement*>*  stmts,
                     Environment*                defineEnv);           //!< Constructor        
        GeneralUnivariateUserFunction(const GeneralUnivariateUserFunction& x);                                    //!< Copy constructor
        virtual ~GeneralUnivariateUserFunction();                                                //!< Delete the code
        
        // overloaded operators
        GeneralUnivariateUserFunction&              operator=(const GeneralUnivariateUserFunction& f);
        
        // Basic utility functions
        GeneralUnivariateUserFunction*              clone(void) const;                                                          //!< Clone the object
        static const std::string&                   getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Regular functions   
        RevBayesCore::UnivariateFunction*           createFunction(void) const;                                                              //!< Execute function
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

