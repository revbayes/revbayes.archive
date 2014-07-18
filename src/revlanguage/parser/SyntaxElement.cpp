#include "RbUtil.h"
#include "SyntaxElement.h"

using namespace RevLanguage;


/**
 * This function evaluates the semantic value of the element if it occurs on the left-hand
 * side of an assignment. It allows variable syntax elements to do the right thing when they
 * appear on the left-hand side of an assignment. The default behavior is to evaluate the
 * semantic value in the standard way for constant expressions, which is appropriate for all
 * other types of syntax elements when they appear on the left-hand sign of an assignment.
 */
RevPtr<Variable> SyntaxElement::evaluateLHSContent( Environment& env, const std::string& varType )
{
    return evaluateContent( env );
}


/**
 * This function evaluates the semantic value of the element if it is part of a dynamic
 * expression, or potentially part of a dynamic expression (function arguments in dynamic
 * expressions, before matching to argument rules). The default behavior is to defer to
 * the standard evaluateContent, which must then handle both cases. In function calls,
 * however, it is important to distinguish the two contexts. It is also critical for
 * variables.
 */
RevPtr<Variable> SyntaxElement::evaluateDynamicContent( Environment& env )
{
    return evaluateContent( env );
}


/**
 * Is the syntax element a constant expression?
 * By default a syntax element is not constant.
 */
bool SyntaxElement::isConstExpression( void ) const
{
    return false;
}


/**
 * Is the syntax element an assignment element?
 * By default the answer is no.
 */
bool SyntaxElement::isAssignment( void ) const
{
    return false;
}

