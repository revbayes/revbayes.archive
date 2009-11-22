/**
 * @file
 * This file contains the declaration of BuiltinFunction, which is
 * used to hold builtin functions (functions in C++ code).
 *
 * @brief Declaration of BuiltinFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @extends RbFunction
 * @package functions
 * @since 2009-11-21
 *
 * $Id$
 */

#ifndef BuiltinFunction_H
#define BuiltinFunction_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "RbFunction.h"


class BuiltinFunction :  public RbFunction {

    public:
                BuiltinFunction(const ArgumentRule** rules, const std::string& retType,
                                RbObject* (*fxn)(std::vector<DAGNode*>));   //!< Constructor
                BuiltinFunction(const BuiltinFunction& bf);                 //!< Copy constructor

        static const StringVector   rbClass;                //!< Static class attribute

        // Basic utility functions
        virtual std::string         briefInfo() const;                  //!< Brief info about object
        virtual BuiltinFunction*    clone() const { return new BuiltinFunction(*this); }   //!< Clone object
        virtual bool                equals(const RbObject* o) const;    //!< Equals comparison
        virtual const StringVector& getClass() const { return rbClass; }//!< Get class
        virtual void                print(std::ostream& o) const;       //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;  //!< Print value (for user)

        // Regular functions
        virtual RbObject*       execute(std::vector<DAGNode*> arguments);       //!< Execute function

    protected:
        RbObject*               (*executeFxn) (std::vector<DAGNode*> arguments); //!< Actual function
};

#endif

