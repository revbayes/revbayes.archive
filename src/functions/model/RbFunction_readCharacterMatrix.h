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

#ifndef RbFunction_readCharacterMatrix_H
#define RbFunction_readCharacterMatrix_H

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
class RbFunction_readCharacterMatrix :  public RbFunction {

    public:
		static const StringVector   rbClass;            //!< Static class attribute

            RbFunction_readCharacterMatrix(void);                              //!< Default constructor, allocate workspace
            RbFunction_readCharacterMatrix(const RbFunction_readCharacterMatrix& s);      //!< Copy constructor
	        ~RbFunction_readCharacterMatrix();                             //!< Destructor, delete workspace

        // implemented abstract/virtual functions from base classes
        RbObject*           clone(void) const ;                                 //!< clone this object
        void                dump(std::ostream& c);                              //!< Dump to ostream c
        void                resurrect(const RbDumpState& x);                    //!< Resurrect from dumped state
        bool                equals(const RbObject* o) const;                    //!< Comparison

        const StringVector& getClass() const { return rbClass; }        //!< Get class
        void                printValue(std::ostream& o) const;          //!< Print value (for user)
        std::string         toString(void) const;                       //!< General info on object

        RbObject*                 executeOperation(const std::vector<DAGNode*>& arguments);              //!< Get result

        // overloaded operators
        RbObject&           operator=(const RbObject& obj);
        RbFunction_readCharacterMatrix&    operator=(const RbFunction_readCharacterMatrix& obj);

    protected:
};

#endif
