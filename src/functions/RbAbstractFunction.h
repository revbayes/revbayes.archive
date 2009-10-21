/**
 * @file
 * This file contains the declaration of RbAbstractFunction, which
 * is the abstract base class for REvBayes functions.
 *
 * @brief Declaration of RbAbstractFunction
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

#ifndef RbAbstractFunction_H
#define RbAbstractFunction_H

#include "RbFunction.h"
#include "Argument.h"

/** This is the abstract base class used to hold regular
 *  functions in the symbol table. The base class deals with
 *  all regular argument rules in a generic way; functions only
 *  need to override setArguments if they allow a variable
 *  number of arguments.
 */
class RbAbstractFunction :  public RbFunction {

public:
   virtual ~RbAbstractFunction();                   //!< Destructor deletes arguments

#pragma mark Parser help functions
//        static const ArgumentRule   argRules[];                 //!< Default argument rules (no args)
//        virtual const ArgumentRule*  getArgumentRules() const;    //!< Get argument rules
        virtual bool                setArguments(std::vector<Argument*> labeledArgs);  //! Set arguments
        virtual bool                isAccessorFunction() const;    //!< Is this an accessor fxn?
        virtual bool                isDistributionFunction() const; //!< Is this a distribution fxn?
//        virtual void                setWorkspace() {}           //!< Do not use workspace by default

#pragma mark Regular functions

    protected:
    	RbAbstractFunction() ;                               //!< Default constructor; set arguments later
    	RbAbstractFunction(const RbAbstractFunction& s);       //!< Copy constructor

};

#endif
