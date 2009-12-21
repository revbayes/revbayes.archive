/**
 * @file
 * This file contains the declaration of RbFunction_sqrt, the
 * sqrt() function.
 *
 * @brief Declaration of RbFunction_sqrt
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

#ifndef RbFunction_sqrt_H
#define RbFunction_sqrt_H

#include "ArgumentRule.h"
#include "RbDouble.h"
#include "RbFunction.h"
#include "RbObject.h"
#include <iostream>
#include <string>
#include <vector>

class RbDumpState;

/** This is the class for the sqrt() function, which takes a single
 *  scalar real or int.
 *
 *  @package    functions
 *  @implements RbFunction, RbStandardFunction
 */
class RbFunction_sqrt :  public RbFunction {

    public:
            RbFunction_sqrt(void);                              //!< Default constructor, allocate workspace
            RbFunction_sqrt(const RbFunction_sqrt& s);      //!< Copy constructor
	        ~RbFunction_sqrt();                             //!< Destructor, delete workspace

        // implemented abstract/virtual functions from base classes
        RbObject*           clone(void) const ;                                 //!< clone this object
        void                print(std::ostream &c) const;                       //!< Print the value to ostream c
        void                dump(std::ostream& c);                              //!< Dump to ostream c
        void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
        bool                equals(const RbObject* o) const;                    //!< Comparison

        const StringVector& getClass() const;                                   //!< Get class
        void                printValue(std::ostream& o) const;          //!< Print value (for user)
        std::string         toString(void) const;                       //!< General info on object

        // Type conversion
        bool                isConvertibleTo(const std::string& type) const;
        RbObject*           convertTo(const std::string& type) const;

        const RbObject*             executeOperation(const std::vector<RbObjectWrapper*>& arguments) const; //!< Get result
        const ArgumentRule**        getArgumentRules(void) const;                                   //!< Get the number of argument rules
        const std::string           getReturnType(void) const;

        // overloaded operators
        RbFunction_sqrt&    operator=(const RbFunction_sqrt& obj);

    protected:
        RbDouble*                     value;              //!< Workspace for result
};

#endif
