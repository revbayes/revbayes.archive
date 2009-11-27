/**
 * @file
 * This file contains the declaration of RbFunction_rexp, the
 * rexp() function.
 *
 * @brief Declaration of RbFunction_rexp
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

#ifndef RbFunction_rexp_H
#define RbFunction_rexp_H

#include "ArgumentRule.h"
#include "RbDouble.h"
#include "RbFunction.h"
#include "RbObject.h"
#include <iostream>
#include <string>
#include <vector>

class RbDumpState;

/** This is the class for the rexp() function, which takes a single
 *  scalar real or int.
 *
 *  @package    functions
 *  @implements RbFunction, RbStandardFunction
 */
class RbFunction_rexp :  public RbFunction {

    public:
        static const StringVector   rbClass;            //!< Static class attribute

            RbFunction_rexp(void);                              //!< Default constructor, allocate workspace
            RbFunction_rexp(const RbFunction_rexp& s);      //!< Copy constructor
	        ~RbFunction_rexp();                             //!< Destructor, delete workspace

        // implemented abstract/virtual functions from base classes
        RbObject*           clone(void) const ;                                 //!< clone this object
        void                print(std::ostream &c) const;                       //!< Print the value to ostream c
        void                dump(std::ostream& c);                              //!< Dump to ostream c
        void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
        bool                equals(const RbObject* o) const;                    //!< Comparison

        const StringVector& getClass() const { return rbClass; }        //!< Get class
        void                printValue(std::ostream& o) const;          //!< Print value (for user)
        std::string         toString(void) const;                       //!< General info on object

        // Type conversion
        bool                isConvertibleTo(const std::string& type) const;
        RbObject*           convertTo(const std::string& type) const;
        virtual const int           getNumberOfRules() const;                   //!< Get number of argument rules for the function

        RbObject*                 executeOperation(const std::vector<DAGNode*>& arguments);              //!< Get result

        // overloaded operators
        RbObject&           operator=(const RbObject& o);
        RbFunction_rexp&   operator=(const RbFunction_rexp& o);

    protected:
        RbDouble*                     value;              //!< Workspace for result
};

#endif
