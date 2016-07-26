/**
 * @file
 * This file contains the implementation of Real, which
 * is the primitive RevBayes type for real numbers.
 *
 * @brief Implementation of Real
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "ConstantNode.h"
#include "Integer.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <iomanip>
#include <sstream>

using namespace RevLanguage;

/* Default constructor */
Real::Real(void) : ModelObject<double>( new double(0.0) )
{

    setGuiVariableName("Real Number");
    setGuiLatexSymbol("R");
}


/* Construct from double */
Real::Real(double v) : ModelObject<double>( new double(v) )
{

    setGuiVariableName("Real Number");
    setGuiLatexSymbol("R");
}


/* Construct from double */
Real::Real( RevBayesCore::TypedDagNode<double> *v ) : ModelObject<double>( v )
{

    setGuiVariableName("Real Number");
    setGuiLatexSymbol("R");
}


/* Construct from int */
Real::Real(int v) : ModelObject<double>( new double(v) )
{

    setGuiVariableName("Real Number");
    setGuiLatexSymbol("R");
}


/* Copy Construct */
Real::Real(const Real& x) : ModelObject<double>( x )
{
    
    setGuiVariableName("Real Number");
    setGuiLatexSymbol("R");
}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RevObject* Real::add( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
    {
        return add( static_cast<const Real&>( rhs ) );
    }
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
    {
        return add( static_cast<const Integer&>( rhs ) );
    }
    
    return ModelObject<double>::add( rhs );
}


/**
 * Specialized addition operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
Real* Real::add(const Real &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() + rhs.getValue() );
    
    return n;
}


/**
 * Specialized addition operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
Real* Real::add(const Integer &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() + rhs.getValue() );
    
    return n;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Real* Real::clone(void) const {

	return new Real(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* Real::convertTo( const TypeSpec& type ) const
{

    if ( type == RlBoolean::getClassTypeSpec() )
        return new RlBoolean(dagNode->getValue() == 0.0);
    if ( type == RealPos::getClassTypeSpec() && dagNode->getValue() > 0.0)
        return new RealPos(dagNode->getValue());
    if ( type == Probability::getClassTypeSpec() && dagNode->getValue() >= 0.0 && dagNode->getValue() <= 1.0)
        return new Probability(dagNode->getValue());
    if ( type == Integer::getClassTypeSpec() && dagNode->getValue() == int(dagNode->getValue()) )
        return new Integer( int(dagNode->getValue()) );
    if ( type == Natural::getClassTypeSpec() && dagNode->getValue() >= 0.0 && dagNode->getValue() == int(dagNode->getValue()) )
        return new Natural( int(dagNode->getValue()) );
    
    if ( type == RlString::getClassTypeSpec() ) 
    {
        std::ostringstream o;
        printValue( o, true );
        return new RlString( o.str() );
    }

    return RevObject::convertTo( type );
}


/**
 * Specialized decrement operation.
 */
void Real::decrement( void ) 
{
    
    dagNode->getValue()--;
    
}


/**
 * Generic division operator.
 * We test if the rhs is of a type that we use for a specialized division operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the ratio.
 */
RevObject* Real::divide( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return divide( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return divide( static_cast<const Integer&>( rhs ) );
    
    return ModelObject<double>::divide( rhs );
}


/**
 * Specialized division operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
Real* Real::divide(const Real &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() / rhs.getValue() );
    
    return n;
}


/**
 * Specialized division operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
Real* Real::divide(const Integer &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() / rhs.getValue() );
    
    return n;
}


/** Get Rev type of object */
const std::string& Real::getClassType(void) { 
    
    static std::string revType = "Real";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Real::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Real::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/**
 * Specialized increment operation.
 */
void Real::increment( void ) 
{
    
    dagNode->getValue()++;
    
}


/** Is convertible to type? */
double Real::isConvertibleTo(const TypeSpec& type, bool once) const {

    if (type == RlBoolean::getClassTypeSpec())
        return 0.6;
    
    if (once && type == RealPos::getClassTypeSpec() && dagNode->getValue() > 0.0)
        return 0.4;
    
    if (once && type == Probability::getClassTypeSpec() && dagNode->getValue() >= 0.0 && dagNode->getValue() <= 1.0)
        return 0.1;
    
    if ( once && type == Integer::getClassTypeSpec() && dagNode->getValue() == int(dagNode->getValue()) )
        return 0.3;
    
    if ( once && type == Natural::getClassTypeSpec() && dagNode->getValue() >= 0.0 && dagNode->getValue() == int(dagNode->getValue()) )
        return 0.2;
    
    if ( type == RlString::getClassTypeSpec() )
        return 0.5;

    return RevObject::isConvertibleTo(type, once);
}


/**
 * Generic multiplication operator.
 * We test if the rhs is of a type that we use for a specialized multiplication operation.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
RevObject* Real::multiply( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return multiply( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return multiply( static_cast<const Integer&>( rhs ) );
    
    return ModelObject<double>::multiply( rhs );
}


/**
 * Specialized multiplication operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
Real* Real::multiply(const Real &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() * rhs.getValue() );
    
    return n;
}


/**
 * Specialized multiplication operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
Real* Real::multiply(const Integer &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() * rhs.getValue() );
    
    return n;
}


/**
 * Generic subtraction operator.
 * We test if the rhs is of a type that we use for a specialized subtraction operation.
 *
 * \param[in]   rhs     The right hand side operand of the subtraction operation.
 *
 * \return              A new object holding the difference.
 */
RevObject* Real::subtract( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return subtract( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return subtract( static_cast<const Integer&>( rhs ) );
    
    return ModelObject<double>::subtract( rhs );
}


/**
 * Specialized subtraction operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the subtraction operation.
 *
 * \return              A new object holding the difference.
 */
Real* Real::subtract(const Real &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() - rhs.getValue() );
    
    return n;
}


/**
 * Specialized subtraction operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the subtraction operation.
 *
 * \return              A new object holding the difference.
 */
Real* Real::subtract(const Integer &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() - rhs.getValue() );
    
    return n;
}


