/**
 * @file
 * This file contains the declaration of RbFunction, which is
 * the interface for REvBayes functions.
 *
 * @brief Declaration of RbFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#ifndef RbFunction_H
#define RbFunction_H

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "RbDataType.h"
#include "RbObject.h"
#include "SyntaxElement.h"
#include "SyntaxLabeledExpr.h"
#include <set>
#include <string>
#include <vector>

/* Forward declaration (contains reference to RbFunction) */
class SymbolTable;


/** This is the interface for functions in REvBayes. 'Bare' function
 *  objects are put in the global symbol table where they act as
 *  templates for the parser. The parser uses the template to create
 *  specific function instances that are matched to the relevant
 *  arguments and then inserted into the syntax tree during construc-
 *  tion of the latter.
 *
 *  A derived function class needs to be able to return an array of
 *  argument rules when the getArgRules() method is called. These
 *  rules are used when labeled expressions returned by the parser
 *  are matched to actual function arguments.
 *
 *  Argument matching happens in the setArguments() function, which
 *  returns true if the argument matching is successful, otherwise
 *  false. When the arguments have been set, the setArguments()
 *  function will call the setWorkspace() function so that the func-
 *  tion can set up a workspace for rapid and convenient multiple
 *  evaluation of the function, if it wishes to do so.
 *
 *  The getDAGNodes() function is used to construct a model DAG and
 *  the copy() function is used to create a copy for fast multiple
 *  execution.
 *
 *  Function objects come in three different flavors. They can be
 *  accessor functions, which accesses an object attribute for
 *  examination or setting. They can also be regular functions.
 *  Finally, they can be distribution functions, which are used as
 *  templates to generate functions that perform probability cal-
 *  culations on a given distribution. An accessor or distribution
 *  function can also be used as a regular function. The methods
 *  isAccessorFxn() and isDistributionFxn() return flags signalling
 *  what type of function object it is. An accessor function needs
 *  to be able to set an attribute of an RbDataType object when
 *  setAttribute() is called.
 */
class RbFunction :  public RbObject {

    public:

#pragma mark Parser help
        virtual RbFunction*         copy() const = 0;   //!< Return copy
        virtual const ArgumentRule* getArgRules() const = 0; //!< Get argument rules for the function
        virtual std::set<DAGNode*>  getDAGNodes() const = 0; //!< Get DAGNode terminals in arguments
        virtual const string&       getDataType() const = 0; //!< Get data type of result
        virtual bool                isAccessorFxn() const = 0;  //! Is this a distribution function?
        virtual bool                isDistributionFxn() const = 0;  //! Is this a distributin function?
        virtual bool                setArguments(std::vector<SyntaxLabeledExpr*> labeledArgs) = 0;  //! Set arguments
        virtual void                setWorkspace() = 0;      //!< Set workspace

#pragma mark Regular functions
        virtual RbDataType*         execute() = 0;           //!< Execute function to get result
        virtual void                print(std::ostream& c=std::cout) const = 0;     //!< Print some info
        virtual bool                setAttribute(RbDataType* newValue) const = 0;   //!< Set attribute
};

#endif
