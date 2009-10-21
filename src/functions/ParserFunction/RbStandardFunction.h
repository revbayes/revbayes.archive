/**
 * @file
 * This file contains the declaration of RbStandardFunction, which
 * is the abstract base class for REvBayes functions.
 *
 * @brief Declaration of RbStandardFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @implements RbFunction (partially)
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#ifndef RbStandardFunction_H
#define RbStandardFunction_H

#include "RbFunction.h"
#include "ArgumentRule.h"

/** This is the abstract base class used to hold regular
 *  functions in the symbol table. The base class deals with
 *  all regular argument rules in a generic way; functions only
 *  need to override setArguments if they allow a variable
 *  number of arguments.
 */
class RbStandardFunction :  public RbFunction {

    public:
            RbStandardFunction() : RbFunction() {}           //!< Default constructor; set arguments later
            RbStandardFunction(const RbStandardFunction& s);      //!< Copy constructor
	        virtual ~RbStandardFunction();                   //!< Destructor deletes arguments

#pragma mark Parser help functions
        static const ArgumentRule   argRules[];                 //!< Default argument rules (no args)
        virtual RbStandardFunction* clone() const = 0;           //!< Return copy
        virtual const ArgumentRule* getArgRules() const { return argRules; }    //!< Get argument rules
        std::set<DAGNode*>          getDAGNodes() const;        //!< Get DAGNode terminals in arguments
        virtual const string&       getDataType() const = 0;    //!< Get data type of result
        virtual bool                isAccessorFunction() const { return false; }     //!< Is this an accessor Function?
        virtual bool                isDistributionFunction() const { return false; } //!< Is this a distribution Function?
        virtual bool                setArguments(std::vector<SyntaxLabeledExpr*> labeledArgs);    //! Set arguments
        virtual void                setWorkspace() {}           //!< Do not use workspace by default

#pragma mark Regular functions
        virtual RbDataType*         execute() = 0;                  //!< Execute function to get result
        virtual void                print(std::ostream& c) const {} //!< Print some info about the function
        virtual bool                setAttribute(RbDataType *newValue) const { return false; }  //! Set attribute

    protected:
        std::vector<SyntaxElement*> arguments;  //!< Processed arguments
};

#endif
