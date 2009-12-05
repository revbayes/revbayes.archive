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
#include <vector>

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Frame.h"
#include "RbFunction.h"
#include "RbObject.h"
#include "SyntaxElement.h"
#include "StringVector.h"


class UserFunction :  public RbFunction {

    public:
                UserFunction(const ArgumentRule** rules, const std::string& retType,
                            std::vector<SyntaxElement*> code);              //!< Constructor
                UserFunction(const UserFunction& uf);                       //!< Copy constructor
                virtual ~UserFunction();                                    //!< Delete the code

        static const StringVector   rbClass;                //!< Static class attribute

        // Basic utility functions
        virtual std::string         briefInfo() const;                  //!< Brief info about object
        virtual UserFunction*       clone() const { return new UserFunction(*this); }   //!< Clone object
        virtual bool                equals(const RbObject* o) const;    //!< Equals comparison
        virtual const StringVector& getClass() const { return rbClass; }//!< Get class
        virtual void                print(std::ostream& o) const;       //!< Print complete object info
        virtual void                printValue(std::ostream& o) const;  //!< Print value (for user)

        // Regular functions
        virtual RbObject*           execute(std::vector<DAGNode*> arguments);       //!< Execute function

    protected:
        std::vector<SyntaxElement*> code;             //!< The code
        Frame*                      frame;            //!< The innermost frame of the evaluation environment
};

#endif

