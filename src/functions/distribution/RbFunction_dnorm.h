/**
 * @file
 * This file contains the declaration of RbFunction_dnorm, the
 * dnorm() function.
 *
 * @brief Declaration of RbFunction_dnorm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#ifndef RbFunction_dnorm_H
#define RbFunction_dnorm_H

#include "RbFunction.h"
#include "RbObject.h"
#include <iostream>
#include <string>
#include <vector>

class ArgumentRule;
class RbDouble;
class RbDumpState;

/** This is the class for the dnorm() function, which takes a single
 *  scalar real or int.
 */
class RbFunction_dnorm :  public RbFunction {

    public:
									RbFunction_dnorm(void);                                   //!< Default constructor, allocate workspace
                                    RbFunction_dnorm(const RbFunction_dnorm& s);              //!< Copy constructor
	                                ~RbFunction_dnorm(void);                                  //!< Destructor, delete workspace

        RbObject*                   clone(void) const ;                                       //!< clone this object
        bool                        equals(const RbObject* o) const;                          //!< Comparison
        const StringVector&         getClass(void) const;                                     //!< Get class
        void                        printValue(std::ostream& o) const;                        //!< Print value (for user)
        std::string                 toString(void) const;                                     //!< General info on object
        void                        dump(std::ostream& c);                                    //!< Dump to ostream c
        void                        resurrect(const RbDumpState& x);                          //!< Resurrect from dumped state

        RbObject*                   executeOperation(const std::vector<DAGNode*>& arguments); //!< Get result
		const ArgumentRule**        getArgumentRules(void) const;                             //!< Get the number of argument rules
        const int                   getNumberOfRules(void) const;                             //!< Get number of argument rules for the function
        const std::string           getReturnType(void) const;                                //!< Get return type

    protected:
        RbDouble*                   value;                                                    //!< Workspace for result
};

#endif
