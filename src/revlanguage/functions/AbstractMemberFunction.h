/**
 * @file
 * This file contains the declaration of AbstractMemberFunction, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular Function objects
 * implementing the member functions.
 *
 * @brief Declaration of AbstractMemberFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-15 18:59:11 +0200 (Tue, 15 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: AbstractMemberFunction.h 1544 2012-05-15 16:59:11Z hoehna $
 */

#ifndef AbstractMemberFunction_H
#define AbstractMemberFunction_H

#include "RlFunction.h"

namespace RevLanguage {

    class ArgumentRule;
    class RevObject;

    class AbstractMemberFunction :  public Function {
    
    public:
        AbstractMemberFunction(const TypeSpec retType, ArgumentRules* argRules);                                        //!< Constructor

        // pure virtual functions
        virtual AbstractMemberFunction*     clone(void) const = 0;                                                      //!< Clone the object
        virtual RevPtr<Variable>            execute(void) = 0;                                                          //!< Execute function
        virtual void                        setMemberObject(const RevPtr<Variable> &obj) = 0;                           //!< Set the member object to which this function belongs
        
        // Basic utility functions
        static const std::string&           getClassType(void);                                                         //!< Get class name
        static const TypeSpec&              getClassTypeSpec(void);                                                     //!< Get class type spec
        virtual const TypeSpec&             getTypeSpec(void) const;                                                    //!< Get language type of the object
    
        // Regular functions
        const ArgumentRules&                getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                     getReturnType(void) const;                                                  //!< Get type of return value
        void                                setMethodName(const std::string& name);                                     //!< Set name of member method
    

    
    protected:
    
        const ArgumentRules*                argumentRules;                                                              //!< Argument rules (different for different member functions)
        std::string                         funcName;                                                                   //!< Name of member method
        RevPtr<Variable>                    object;
        const TypeSpec                      returnType;                                                                  //!< Return type (different for different member functions)
    
    };
    
}

#endif

