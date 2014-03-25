/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for syntax elements.
 *
 * @brief Partial implementation of SyntaxElement
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 *
 * $Id$
 */

#include "RbUtil.h"
#include "SyntaxElement.h"

using namespace RevLanguage;


/**
 *  This function evaluates the semantic value of the element if it is part of a deterministic
 *  expression. It allows variable syntax elements to produce lookups of themselves.
 *  The default behavior is to evaluate the semantic value in the standard way, which is
 *  sufficient for all other types of syntax elements.
 *
 */
RbPtr<Variable> SyntaxElement::evaluateDeterministicExpressionContent(Environment& env) {

    return evaluateContent(env);
}


/** Is the syntax expression constant?
 *  This is used for replacing the expression than by a constant.
 *  By default a syntax element is not constant.
 */
bool SyntaxElement::isConstExpression( void ) const {
    return false;
}


bool SyntaxElement::isAssignment( void ) const {
    return false;
}




