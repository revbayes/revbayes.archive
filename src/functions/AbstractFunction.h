/**
 * @file
 * This file contains the declaration of AbstractFunction, which
 * is the abstract base class for REvBayes functions.
 *
 * @brief Declaration of AbstractFunction
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

#ifndef AbstractFunction_H
#define AbstractFunction_H

#include "RbFunction.h"

/** This is the abstract base class used to hold regular
 *  functions in the symbol table. The base class deals with
 *  all regular argument rules in a generic way; functions only
 *  need to override setArguments if they allow a variable
 *  number of arguments.
 */
class AbstractFunction :  public RbFunction {

public:
   virtual ~AbstractFunction();                   //!< Destructor deletes arguments

#pragma mark Parser help functions
//        static const ArgumentRule   argRules[];                 //!< Default argument rules (no args)
//        virtual const ArgumentRule*  getArgumentRules() const;    //!< Get argument rules
        virtual bool                setArguments(std::vector<DAGNode*> labeledArgs);  //! Set arguments
        virtual bool                isAccessorFunction() const;    //!< Is this an accessor fxn?
        virtual bool                isDistributionFunction() const; //!< Is this a distribution fxn?
//        virtual void                setWorkspace() {}           //!< Do not use workspace by default

#pragma mark Regular functions

    protected:
    	AbstractFunction() : RbFunction() {}           //!< Default constructor; set arguments later
    	AbstractFunction(AbstractFunction& f) : RbFunction() {}           //!< Default constructor; set arguments later
    	AbstractFunction(const AbstractFunction& s);      //!< Copy constructor

};

#endif
