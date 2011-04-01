/**
 * @file
 * This file contains the declaration of Func__add, which is used
 * to add two variables.
 *
 * @brief Declaration of Func__add
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__add_H
#define Func__add_H

#include "ContainerNode.h"
#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType, typename sumType>
class Func__add :  public RbFunction {

    public:
        // Basic utility functions
        Func__add*                  clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

	protected:
        DAGNode*                    executeOperation(const std::vector<VariableSlot>& args);    //!< Execute operation
};

#endif

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Integer.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "TypeSpec.h"
#include "VectorString.h"


/** Clone object */
template <typename firstValType, typename secondValType, typename sumType>
Func__add<firstValType, secondValType, sumType>* Func__add<firstValType, secondValType, sumType>::clone(void) const {

    return new Func__add(*this);
}


/** Execute function: Integer <- Integer + Integer */
template <>
DAGNode* Func__add<Integer,Integer,Integer>::executeOperation(const std::vector<VariableSlot>& args) {

    int val1 = ((Integer*)(args[0].getValue()))->getValue();
    int val2 = ((Integer*)(args[1].getValue()))->getValue();
    int sum  = val1 + val2;
    return new ConstantNode( new Integer(sum));
}


/** Execute function: Real <- Real + Real */
template <>
DAGNode* Func__add<Real,Real,Real>::executeOperation(const std::vector<VariableSlot>& args) {

    double val1 = ((Real*)(args[0].getValue()))->getValue();
    double val2 = ((Real*)(args[1].getValue()))->getValue();
    double sum  = val1 + val2;
    return new ConstantNode( new Real(sum));
}


/** Execute function: Real <- Integer + Real */
template <>
DAGNode* Func__add<Integer,Real,Real>::executeOperation(const std::vector<VariableSlot>& args) {

    int    val1 = static_cast<const Integer*>( args[0].getValue() )->getValue();
    double val2 = static_cast<const Real*>   ( args[1].getValue() )->getValue();
    double sum  = val1 + val2;
    return new ConstantNode( new Real(sum));
}


/** Execute function: Real <- Real + Integer */
template <>
DAGNode* Func__add<Real,Integer,Real>::executeOperation(const std::vector<VariableSlot>& args) {

    double val1 = ((Real*)(args[0].getValue()))->getValue();
    int    val2 = ((Integer*)(args[1].getValue()))->getValue();
    double sum  = val1 + val2;
    return new ConstantNode( new Real(sum));
}


/** Execute function: RealMatrix <- RealMatrix + RealMatrix */
template <>
DAGNode* Func__add<MatrixReal,MatrixReal,MatrixReal>::executeOperation(const std::vector<VariableSlot>& args) {

    MatrixReal val1 = ((MatrixReal*)(args[0].getValue()))->getValue();
    MatrixReal val2 = ((MatrixReal*)(args[1].getValue()))->getValue();
    MatrixReal sum  = val1 + val2;
    return new ContainerNode( new MatrixReal(sum));
}


/** Get argument rules */
template <typename firstValType, typename secondValType, typename sumType>
const ArgumentRules& Func__add<firstValType, secondValType, sumType>::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
        {
        argumentRules.push_back(new ArgumentRule("", firstValType() ));
        argumentRules.push_back(new ArgumentRule("", secondValType()));
        rulesSet = true;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType, typename sumType>
const VectorString& Func__add<firstValType, secondValType, sumType>::getClass(void) const {

    static std::string  rbName  = "Func__add<" + firstValType().getType() + "," + secondValType().getType() + "," + sumType().getType() + ">"; 
    static VectorString rbClass = VectorString(rbName) + RbFunction::getClass();
    
    return rbClass;
}


/** Get return type */
template <typename firstValType, typename secondValType, typename sumType>
const TypeSpec Func__add<firstValType, secondValType, sumType>::getReturnType(void) const {

    return sumType().getTypeSpec();
}

