/**
 * @file
 * This file contains the declaration of UserFunction, which is
 * used to hold user-defined functions.
 *
 * @brief Declaration of UserFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @extends RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#ifndef UserFunction_H
#define UserFunction_H

#include <ostream>
#include <string>
#include <list>

#include "RbFunction.h"

class ArgumentRule;
class DAGNode;
class Environment;
class RbObject;
class RbString;
class TypeSpec;
class SyntaxElement;

class UserFunction :  public RbFunction {

    public:
                UserFunction(   const ArgumentRules*  argRules,
                                const TypeSpec&             retType,
                                std::list<SyntaxElement*>*  stmts,
                                Environment*                defineEnv);           //!< Constructor
                UserFunction(const UserFunction& x);                                    //!< Copy constructor
                virtual ~UserFunction();                                                //!< Delete the code

        // overloaded operators
        UserFunction&                               operator=(const UserFunction& f);
        
        // Basic utility functions
        UserFunction*                               clone() const;                      //!< Clone object
        static const std::string&                   getClassName(void);                 //!< Get class name
        static const TypeSpec&                      getClassTypeSpec(void);             //!< Get class type spec
        const TypeSpec&                             getTypeSpec(void) const;            //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;  //!< Print the value

        // Regular functions
        const ArgumentRules&                        getArgumentRules(void) const;       //!< Get arg rules
        const TypeSpec&                             getReturnType(void) const;          //!< Get return type

	protected:
        const RbLanguageObject&                     executeFunction(void);              //!< Execute function
        void                                        setArgumentVariable(const std::string& name, const Variable* var);

        // function return value
        RbLanguageObject*                           retValue;
    
        const ArgumentRules*                        argumentRules;                      //!< The argument rules
        const TypeSpec                              returnType;                         //!< The return type (complete specification)
        std::list<SyntaxElement*>*                  code;                               //!< The code
        Environment*                                defineEnvironment;                  //!< The definition environment
    
};

#endif

