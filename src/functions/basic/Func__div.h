/**
 * @file
 * This file contains the declaration of Func__div, which is used
 * to divide two variables.
 *
 * @brief Declaration of Func__div
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__div_H
#define Func__div_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType, typename sumType>
class Func__div :  public RbFunction {

    public:
        // Basic utility functions
        Func__div*                  clone(void) const;                                      //!< Clone the object
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
#include "RbMath.h"
#include "RbNames.h"
#include "Real.h"
#include "MatrixReal.h"
#include "VectorString.h"


/** Clone object */
template <typename firstValType, typename secondValType, typename sumType>
Func__div<firstValType, secondValType, sumType>* Func__div<firstValType, secondValType, sumType>::clone(void) const {

    return new Func__div(*this);
}


/** Execute function: Integer <- Integer / Integer */
template <>
RbObject* Func__div<Integer,Integer,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = (double)(((Integer*)(args[0])->getValue())->getValue());
    double val2 = (double)(((Integer*)(args[1])->getValue())->getValue());
    double quot = val1 / val2;
    return new Real(quot);
}


/** Execute function: Real <- Real / Real */
template <>
RbObject* Func__div<Real,Real,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = ((Real*)(args[0])->getValue())->getValue();
    double val2 = ((Real*)(args[1])->getValue())->getValue();
    double quot = val1 / val2;
    return new Real(quot);
}


/** Execute function: Real <- Integer / Real */
template <>
RbObject* Func__div<Integer,Real,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = (double)(((Integer*)(args[0])->getValue())->getValue());
    double val2 = ((Real*)(args[1])->getValue())->getValue();
    double quot = val1 / val2;
    return new Real(quot);
}


/** Execute function: Real <- Real / Integer */
template <>
RbObject* Func__div<Real,Integer,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = ((Real*)(args[0])->getValue())->getValue();
    double val2 = (double)(((Integer*)(args[1])->getValue())->getValue());
    double quot = val1 / val2;
    return new Real(quot);
}


/** Execute function: MatrixReal <- MatrixReal / MatrixReal */
template <>
RbObject* Func__div<MatrixReal,MatrixReal,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    MatrixReal val1 = ((MatrixReal*)(args[0])->getValue())->getValue();
    MatrixReal val2 = ((MatrixReal*)(args[1])->getValue())->getValue();
    MatrixReal val2Inv(val2);
    RbMath::matrixInverse(val2, val2Inv);
    MatrixReal quot = val1 * val2Inv;
    return new MatrixReal(quot);
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename sumType>
const ArgumentRules& Func__div<firstValType, secondValType, sumType>::getArgumentRules(void) const {

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
const VectorString& Func__div<firstValType, secondValType, sumType>::getClass(void) const {

    firstValType*  dummy1 = new firstValType();
    secondValType* dummy2 = new secondValType();
    sumType*       dummy3 = new sumType();
    
    std::string funcAddName = "Func__div<" + dummy1->getType() + "," + dummy2->getType() + "," + dummy3->getType() + ">"; 
    static VectorString rbClass = VectorString(funcAddName) + RbFunction::getClass();
    
    delete dummy1;
    delete dummy2;
    delete dummy3;
    
    return rbClass;
}


/** Get return dimension */
template <typename firstValType, typename secondValType, typename sumType>
int Func__div<firstValType, secondValType, sumType>::getReturnDim(void) const {

    return 1;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename sumType>
const std::string& Func__div<firstValType, secondValType, sumType>::getReturnType(void) const {

    sumType* dummy  = new sumType();
    static std::string retTypeStr = dummy->getType();
    delete dummy;
    return retTypeStr;
}


