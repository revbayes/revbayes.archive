#include "RbUtil.h"
#include "SyntaxElement.h"

using namespace RevLanguage;


/**
 * This function evaluates the semantic value of the element if it is part of an indirect
 * reference deterministic expression. It allows variable syntax elements to produce lookups
 * of themselves. The default behavior is to evaluate the semantic value in the standard way
 * for dynamic expressions, which is sufficient for all other types of syntax elements.
 */
RevPtr<Variable> SyntaxElement::evaluateIndirectReferenceContent(Environment& env)
{
    return evaluateDynamicContent(env);
}


/**
 * This function evaluates the semantic value of the element if it is part of a dynamic
 * expression, or potentially part of a dynamic expression (function arguments in dynamic
 * expressions, before matching to argument rules). The default behavior is to defer to
 * the standard evaluateContent, which must then handle both cases. In function calls,
 * however, it is important to distinguish the two.
 */
RevPtr<Variable> SyntaxElement::evaluateDynamicContent(Environment& env)
{
    return evaluateContent(env);
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

