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

