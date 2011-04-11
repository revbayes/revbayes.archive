/**
 * @file
 * This file contains the declaration of Func_transpose, which 
 * is a templated class for matrix transpose functions.
 *
 * @brief Declaration of Func_transpose
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_transpose_H
#define Func_transpose_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

template <typename valT, typename retT>
class Func_transpose :  public RbFunction {

    public:
        // Basic utility functions
        Func_transpose*             clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	DAGNode*                    execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

};

#endif

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorInteger.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"


/** Clone object */
template <typename valT, typename retT>
Func_transpose<valT, retT>* Func_transpose<valT, retT>::clone(void) const {

    return new Func_transpose(*this);
}


/** Execute function: MatrixReal <- transpose(MatrixReal) */
template <>
DAGNode* Func_transpose<MatrixReal,MatrixReal>::execute( void ) {

    MatrixReal mat = (((MatrixReal*) (args[0].getValue())))->getValue();
    int n = mat.getNumRows();
    int m = mat.getNumCols();
    MatrixReal* matT = new MatrixReal(m, n, 0.0);
    RbMath::transposeMatrix(mat, *matT);
    return new ConstantNode( matT );
}


/** Get argument rules */
template <typename valT, typename retT>
const ArgumentRules& Func_transpose<valT, retT>::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back(new ValueRule("", valT().getType()));

        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename valT, typename retT>
const VectorString& Func_transpose<valT, retT>::getClass(void) const {

    valT* dummy1 = new valT();
    retT* dummy2 = new retT();
    
    std::string funcName = "Func__transpose<" + dummy1->getType() + "," + dummy2->getType() + ">"; 
    static VectorString rbClass = VectorString(funcName) + RbFunction::getClass();
    
    delete dummy1;
    delete dummy2;
    
    return rbClass;
}


/** Get return type */
template <typename valT, typename retT>
const TypeSpec Func_transpose<valT, retT>::getReturnType(void) const {

    retT* dummy  = new retT();
    static std::string retTypeStr = dummy->getType();
    delete dummy;
    return TypeSpec(retTypeStr);
}


