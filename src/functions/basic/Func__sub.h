/**
 * @file
 * This file contains the declaration of Func__sub, which is used
 * to subtract two variables.
 *
 * @brief Declaration of Func__sub
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__sub_H
#define Func__sub_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType, typename sumType>
class Func__sub :  public RbFunction {

    public:
        // Basic utility functions
        Func__sub*                  clone(void) const;                                      //!< Clone the object
    	const VectorString&         getClass(void) const;                                   //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                           //!< Get argument rules
        int                         getReturnDim(void) const;                               //!< Get dim of return value
        const std::string&          getReturnType(void) const;                              //!< Get type of return value

	protected:
        RbObject*                   executeOperation(const std::vector<DAGNode*>& args);    //!< Execute operation
};

#endif

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "MatrixReal.h"
#include "VectorString.h"


/** Clone object */
template <typename firstValType, typename secondValType, typename sumType>
Func__sub<firstValType, secondValType, sumType>* Func__sub<firstValType, secondValType, sumType>::clone(void) const {

    return new Func__sub(*this);
}


/** Execute function: Integer <- Integer - Integer */
template <>
RbObject* Func__sub<Integer,Integer,Integer>::executeOperation(const std::vector<DAGNode*>& args) {

    int val1 = ((Integer*)(args[0])->getValue())->getValue();
    int val2 = ((Integer*)(args[1])->getValue())->getValue();
    int res  = val1 - val2;
    return new Integer(res);
}


/** Execute function: Real <- Real - Real */
template <>
RbObject* Func__sub<Real,Real,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = ((Real*)(args[0])->getValue())->getValue();
    double val2 = ((Real*)(args[1])->getValue())->getValue();
    double res  = val1 - val2;
    return new Real(res);
}


/** Execute function: Real <- Integer - Real */
template <>
RbObject* Func__sub<Integer,Real,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = (double)(((Integer*)(args[0])->getValue())->getValue());
    double val2 = ((Real*)(args[1])->getValue())->getValue();
    double res  = val1 - val2;
    return new Real(res);
}


/** Execute function: Real <- Real - Integer */
template <>
RbObject* Func__sub<Real,Integer,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = ((Real*)(args[0])->getValue())->getValue();
    double val2 = (double)(((Integer*)(args[1])->getValue())->getValue());
    double res  = val1 - val2;
    return new Real(res);
}


/** Execute function: RealMatrix <- RealMatrix - RealMatrix */
template <>
RbObject* Func__sub<MatrixReal,MatrixReal,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    MatrixReal val1 = ((MatrixReal*)(args[0])->getValue())->getValue();
    MatrixReal val2 = ((MatrixReal*)(args[1])->getValue())->getValue();
    MatrixReal res  = val1 - val2;
    return new MatrixReal(res);
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename sumType>
const ArgumentRules& Func__sub<firstValType, secondValType, sumType>::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
        {
        firstValType*  dummy1 = new firstValType();
        secondValType* dummy2 = new secondValType();
        argumentRules.push_back(new ArgumentRule("", dummy1->getType()));
        argumentRules.push_back(new ArgumentRule("", dummy2->getType()));
        rulesSet = true;
        delete dummy1;
        delete dummy2;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType, typename sumType>
const VectorString& Func__sub<firstValType, secondValType, sumType>::getClass(void) const {

    firstValType*  dummy1 = new firstValType();
    secondValType* dummy2 = new secondValType();
    sumType*       dummy3 = new sumType();
    
    std::string funcAddName = "Func__sub<" + dummy1->getType() + "_" + dummy2->getType() + "_" + dummy3->getType() + ">_name"; 
    static VectorString rbClass = VectorString(funcAddName) + RbFunction::getClass();
    
    delete dummy1;
    delete dummy2;
    delete dummy3;
    
    return rbClass;
}


/** Get return dimension */
template <typename firstValType, typename secondValType, typename sumType>
int Func__sub<firstValType, secondValType, sumType>::getReturnDim(void) const {

    return 1;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename sumType>
const std::string& Func__sub<firstValType, secondValType, sumType>::getReturnType(void) const {

    sumType* dummy  = new sumType();
    static std::string retTypeStr = dummy->getType();
    delete dummy;
    return retTypeStr;
}


