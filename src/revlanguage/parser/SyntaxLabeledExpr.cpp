#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "SyntaxLabeledExpr.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor from label and expression */
SyntaxLabeledExpr::SyntaxLabeledExpr( const std::string& lbl, SyntaxElement* expr ) :
    SyntaxElement(),
    label( lbl ),
    expression( expr )
{
}


/** Deep copy constructor */
SyntaxLabeledExpr::SyntaxLabeledExpr( const SyntaxLabeledExpr& x ) :
SyntaxElement(x)
{
    label      = x.label;
    expression = x.expression->clone();
}


/** Destructor deletes members */
SyntaxLabeledExpr::~SyntaxLabeledExpr( void )
{
    delete expression;
}


/** Assignment operator */
SyntaxLabeledExpr& SyntaxLabeledExpr::operator=( const SyntaxLabeledExpr& x )
{
    if ( &x != this )
    {
        SyntaxElement::operator=(x);
        
        delete expression;

        label      = x.label;
        expression = x.expression->clone();
    }

    return (*this);
}


/** Clone syntax element */
SyntaxLabeledExpr* SyntaxLabeledExpr::clone( void ) const
{
    return new SyntaxLabeledExpr(*this);
}


/** Get semantic value (not applicable so return NULL) */
RevPtr<Variable> SyntaxLabeledExpr::evaluateContent( Environment& env )
{
    return NULL;
}


/** Is the expression constant?
 *  Only if the argument is constant.
 */
bool SyntaxLabeledExpr::isConstExpression( void ) const
{
    return expression->isConstExpression();
}


/**
 * Is this syntax element safe for inclusion in a function definition? Simply
 * delegate to the expression.
 */
bool SyntaxLabeledExpr::isFunctionSafe( const Environment& env, std::set<std::string>& localVars ) const
{
    return expression->isFunctionSafe( env, localVars );
}


/** Print info about the syntax element */
void SyntaxLabeledExpr::printValue( std::ostream& o ) const
{
    o << "[" << this << "] SyntaxLabeledExpr:" << std::endl;
    o << "label      = " << label << std::endl;
    o << "expression = [" << expression << "]";
    expression->printValue(o);
    o << std::endl;
    o << std::endl;
}


/**
 * Does this syntax element retrieve an external variable? Simply
 * delegate to the expression. The inLHS flag should always be
 * false if we reach this element, and we pass it on as false.
 * This is because procedures are not allowed in LHS expressions,
 * and only procedures need to use this call.
 */
bool SyntaxLabeledExpr::retrievesExternVar( const Environment& env, std::set<std::string>& localVars, bool inLHS ) const
{
    return expression->retrievesExternVar( env, localVars, false );
}

