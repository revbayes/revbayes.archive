/**
 * @file
 * This file contains the declaration of UserFunction, which is
 * used to hold user-defined functions.
 *
 * @brief Declaration of UserFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
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
class Frame;
class RbObject;
class RbString;
class VectorString;
class SyntaxElement;

class UserFunction :  public RbFunction {

    public:
                UserFunction(   const ArgumentRules&        argRules,
                                const std::string           retType,
                                std::list<SyntaxElement*>*  stmts,
                                Frame*                      defineEnv);     //!< Constructor
                UserFunction(const UserFunction& x);                        //!< Copy constructor
                virtual ~UserFunction();                                    //!< Delete the code

        // Basic utility functions
        std::string                 briefInfo() const;                  //!< Brief info about object
        UserFunction*               clone() const;                      //!< Clone object
        bool                        equals(const RbObject* x) const;    //!< Equals comparison
        const VectorString&         getClass() const;                   //!< Get class vector
        std::string                 toString() const;                   //!< Complete info about object

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;       //!< Get arg rules
        const std::string&          getReturnType(void) const;          //!< Get return type

	protected:
        const ArgumentRules         argumentRules;      //!< The argument rules
        std::string                 returnType;         //!< The return type
        std::list<SyntaxElement*>*  code;               //!< The code
        Frame*                      defineEnvironment;  //!< The definition environment

		DAGNode*                   executeOperation(const std::vector<DAGNode*>& args);    //!< Execute operation
};

#endif

