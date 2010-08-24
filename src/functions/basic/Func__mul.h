/**
 * @file
 * This file contains the declaration of Func__mul, which is used
 * to multiply two variables.
 *
 * @brief Declaration of Func__mul
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__mul_H
#define Func__mul_H

#pragma warning (disable: 4068)

#include "RbFunction.h"

#include <map>
#include <string>

#pragma mark Class Definition

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType, typename sumType>
class Func__mul :  public RbFunction {

    public:
        // Basic utility functions
        Func__mul*                  clone(void) const;                                      //!< Clone the object
    	const VectorString&         getClass(void) const;                                   //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                           //!< Get argument rules
        int                         getReturnDim(void) const;                               //!< Get dim of return value
        const std::string&          getReturnType(void) const;                              //!< Get type of return value

	protected:
        DAGNode*                   executeOperation(const std::vector<DAGNode*>& args);    //!< Execute operation
};

#endif

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "MatrixReal.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"


/** Clone object */
template <typename firstValType, typename secondValType, typename sumType>
Func__mul<firstValType, secondValType, sumType>* Func__mul<firstValType, secondValType, sumType>::clone(void) const {

    return new Func__mul(*this);
}

#pragma mark Integer * Integer = Integer

/** Execute function: Integer <- Integer * Integer */
template <>
DAGNode* Func__mul<Integer,Integer,Integer>::executeOperation(const std::vector<DAGNode*>& args) {

    int val1 = ((Integer*)(args[0])->getValue())->getValue();
    int val2 = ((Integer*)(args[1])->getValue())->getValue();
    int prod = val1 * val2;
    return new ConstantNode( new Integer(prod));
}

#pragma mark Real * Real = Real

/** Execute function: Real <- Real * Real */
template <>
DAGNode* Func__mul<Real,Real,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = ((Real*)(args[0])->getValue())->getValue();
    double val2 = ((Real*)(args[1])->getValue())->getValue();
    double prod = val1 * val2;
    return new ConstantNode( new Real(prod));
}

#pragma mark Integer * Real = Real

/** Execute function: Real <- Integer * Real */
template <>
DAGNode* Func__mul<Integer,Real,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = (double)(((Integer*)(args[0])->getValue())->getValue());
    double val2 = ((Real*)(args[1])->getValue())->getValue();
    double prod = val1 * val2;
    return new ConstantNode( new Real(prod));
}

#pragma mark Real * Integer = Real

/** Execute function: Real <- Real * Integer */
template <>
DAGNode* Func__mul<Real,Integer,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = ((Real*)(args[0])->getValue())->getValue();
    double val2 = (double)(((Integer*)(args[1])->getValue())->getValue());
    double prod = val1 * val2;
    return new ConstantNode( new Real(prod));
}

#pragma mark M(Real) * M(Real) = M(Real)

/** Execute function: MatrixReal <- MatrixReal * MatrixReal */
template <>
DAGNode* Func__mul<MatrixReal,MatrixReal,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    MatrixReal val1 = ((MatrixReal*)(args[0])->getValue())->getValue();
    MatrixReal val2 = ((MatrixReal*)(args[1])->getValue())->getValue();
    MatrixReal prod = val1 * val2;
    return new ConstantNode( new MatrixReal(prod));
}

#pragma mark M(Real) * Real = M(Real)

/** Execute function: MatrixReal <- MatrixReal * Real */
template <>
DAGNode* Func__mul<MatrixReal,Real,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    MatrixReal val1 = ((MatrixReal*)(args[0])->getValue())->getValue();
    double     val2 = ((Real*)(args[1])->getValue())->getValue();
    MatrixReal prod = val1 * val2;
    return new ConstantNode( new MatrixReal(prod));
}

#pragma mark Real * M(Real) = M(Real)

/** Execute function: MatrixReal <- Real * MatrixReal */
template <>
DAGNode* Func__mul<Real,MatrixReal,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    double     val1 = ((Real*)(args[0])->getValue())->getValue();
    MatrixReal val2 = ((MatrixReal*)(args[1])->getValue())->getValue();
    MatrixReal prod = val1 * val2;
    return new ConstantNode( new MatrixReal(prod));
}

#pragma mark V(Real) * V(Real) = M(Real)

/** Execute function: MatrixReal <- VectorReal * VectorReal */
template <>
DAGNode* Func__mul<VectorReal,VectorReal,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    VectorReal val1 = ((VectorReal*)(args[0])->getValue())->getValue();
    VectorReal val2 = ((VectorReal*)(args[1])->getValue())->getValue();
    MatrixReal prod;
    RbMath::vectorMultiplication( val1, val2, prod );
    return new ConstantNode( new MatrixReal(prod));
}

#pragma mark V(Real) * M(Real) = M(Real)

/** Execute function: MatrixReal <- VectorReal * MatrixReal */
template <>
DAGNode* Func__mul<VectorReal,MatrixReal,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    const VectorReal* val1 = (VectorReal*)(args[0])->getValue();
    const VectorReal* val2 = (VectorReal*)(args[1])->getValue();
    MatrixReal prod;
    RbMath::vectorMultiplication( *val1, *val2, prod );
    return new ConstantNode( new MatrixReal(prod));
}

#pragma mark M(Real) * V(Real) = M(Real)

/** Execute function: MatrixReal <- MatrixReal * VectorReal */
template <>
DAGNode* Func__mul<MatrixReal,VectorReal,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    VectorReal val1 = ((VectorReal*)(args[0])->getValue())->getValue();
    VectorReal val2 = ((VectorReal*)(args[1])->getValue())->getValue();
    MatrixReal prod;
    RbMath::vectorMultiplication( val1, val2, prod );
    return new ConstantNode( new MatrixReal(prod));
}

#pragma mark V(Real+) * M(Real) = M(Real)

/** Execute function: MatrixReal <- VectorRealPos * MatrixReal */
template <>
DAGNode* Func__mul<VectorRealPos,MatrixReal,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    VectorRealPos val1 = ((VectorRealPos*)(args[0])->getValue())->getValue();
    VectorRealPos val2 = ((VectorRealPos*)(args[1])->getValue())->getValue();
    MatrixReal prod;
    //RbMath::vectorMultiplication( val1, val2, prod );
    return new ConstantNode( new MatrixReal(prod));
}

#pragma mark M(Real) * V(Real+) = M(Real)

/** Execute function: MatrixReal <- RealMatrix * VectorRealPos */
template <>
DAGNode* Func__mul<MatrixReal,VectorRealPos,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    VectorRealPos val1 = ((VectorRealPos*)(args[0])->getValue())->getValue();
    VectorRealPos val2 = ((VectorRealPos*)(args[1])->getValue())->getValue();
    MatrixReal prod;
    //RbMath::vectorMultiplication( val1, val2, prod );
    return new ConstantNode( new MatrixReal(prod));
}

#pragma mark V(Real+) * V(Real+) = M(Real)

/** Execute function: MatrixReal <- VectorRealPos * VectorRealPos */
template <>
DAGNode* Func__mul<VectorRealPos,VectorRealPos,MatrixReal>::executeOperation(const std::vector<DAGNode*>& args) {

    VectorRealPos val1 = ((VectorRealPos*)(args[0])->getValue())->getValue();
    VectorRealPos val2 = ((VectorRealPos*)(args[1])->getValue())->getValue();
    MatrixReal prod;
    RbMath::vectorMultiplication( val1, val2, prod );
    return new ConstantNode( new MatrixReal(prod));
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename sumType>
const ArgumentRules& Func__mul<firstValType, secondValType, sumType>::getArgumentRules(void) const {

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
const VectorString& Func__mul<firstValType, secondValType, sumType>::getClass(void) const {

    firstValType*  dummy1 = new firstValType();
    secondValType* dummy2 = new secondValType();
    sumType*       dummy3 = new sumType();
    
    std::string funcAddName = "Func__mul<" + dummy1->getType() + "," + dummy2->getType() + "," + dummy3->getType() + ">"; 
    static VectorString rbClass = VectorString(funcAddName) + RbFunction::getClass();
    
    delete dummy1;
    delete dummy2;
    delete dummy3;
    
    return rbClass;
}


/** Get return dimension */
template <typename firstValType, typename secondValType, typename sumType>
int Func__mul<firstValType, secondValType, sumType>::getReturnDim(void) const {

    return 1;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename sumType>
const std::string& Func__mul<firstValType, secondValType, sumType>::getReturnType(void) const {

    sumType* dummy  = new sumType();
    static std::string retTypeStr = dummy->getType();
    delete dummy;
    return retTypeStr;
}


