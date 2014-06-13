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
Real::Real(void) : RlModelVariableWrapper<double>( new double(0.0) ) {
}


/* Construct from double */
Real::Real(double v) : RlModelVariableWrapper<double>( new double(v) ) {
}


/* Construct from double */
Real::Real( RevBayesCore::TypedDagNode<double> *v ) : RlModelVariableWrapper<double>( v ) {
}


/* Construct from int */
Real::Real(int v) : RlModelVariableWrapper<double>( new double(v) ) {
}


/* Copy Construct */
Real::Real(const Real& x) : RlModelVariableWrapper<double>( x ) {
    
}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RbLanguageObject* Real::add( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return add( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return add( static_cast<const Integer&>( rhs ) );
    
    return RlModelVariableWrapper<double>::add( rhs );
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
    
    Real *n = new Real( value->getValue() + rhs.getValue() );
    
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
    
    Real *n = new Real( value->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone object */
Real* Real::clone(void) const {

	return new Real(*this);
}


/** Convert to type. The caller manages the returned object. */
RbLanguageObject* Real::convertTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec() )
        return new RlBoolean(value->getValue() == 0.0);
    if ( type == RealPos::getClassTypeSpec() && value->getValue() > 0.0)
        return new RealPos(value->getValue());
    if ( type == Probability::getClassTypeSpec() && value->getValue() >= 0.0 && value->getValue() <= 1.0)
        return new Probability(value->getValue());
    if ( type == Integer::getClassTypeSpec() && value->getValue() == int(value->getValue()) )
        return new Integer( int(value->getValue()) );
    if ( type == Natural::getClassTypeSpec() && value->getValue() >= 0.0 && value->getValue() == int(value->getValue()) )
        return new Natural( int(value->getValue()) );
    
    if ( type == RlString::getClassTypeSpec() ) 
    {
        std::ostringstream o;
        printValue( o );
        return new RlString( o.str() );
    }

    return RbLanguageObject::convertTo( type );
}


/**
 * Specialized decrement operation.
 */
void Real::decrement( void ) 
{
    
    value->getValue()--;
    
}


/**
 * Generic division operator.
 * We test if the rhs is of a type that we use for a specialized division operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the ratio.
 */
RbLanguageObject* Real::divide( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return divide( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return divide( static_cast<const Integer&>( rhs ) );
    
    return RlModelVariableWrapper<double>::divide( rhs );
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
    
    Real *n = new Real( value->getValue() / rhs.getValue() );
    
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
    
    Real *n = new Real( value->getValue() / rhs.getValue() );
    
    return n;
}


/** Get class name of object */
const std::string& Real::getClassName(void) { 
    
    static std::string rbClassName = "Real";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Real::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
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
    
    value->getValue()++;
    
}


/** Is convertible to type? */
bool Real::isConvertibleTo(const TypeSpec& type) const {

    if (type == RlBoolean::getClassTypeSpec())
        return true;
    if (type == RealPos::getClassTypeSpec() && value->getValue() > 0.0)
        return true;
    if (type == Probability::getClassTypeSpec() && value->getValue() >= 0.0 && value->getValue() <= 1.0)
        return true;
    if ( type == Integer::getClassTypeSpec() && value->getValue() == int(value->getValue()) )
        return true;
    if ( type == Natural::getClassTypeSpec() && value->getValue() >= 0.0 && value->getValue() == int(value->getValue()) )
        return true;
    if ( type == RlString::getClassTypeSpec() )
        return true;

    return RbLanguageObject::isConvertibleTo(type);
}


/**
 * Generic multiplication operator.
 * We test if the rhs is of a type that we use for a specialized multiplication operation.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
RbLanguageObject* Real::multiply( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return multiply( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return multiply( static_cast<const Integer&>( rhs ) );
    
    return RlModelVariableWrapper<double>::multiply( rhs );
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
    
    Real *n = new Real( value->getValue() * rhs.getValue() );
    
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
    
    Real *n = new Real( value->getValue() * rhs.getValue() );
    
    return n;
}



/** Print value for user */
void Real::printValue(std::ostream &o) const {

    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    std::fixed( o );
    o.precision( 3 );
    o << value->getValue();

    o.setf( previousFlags );
    o.precision( previousPrecision );
}


/**
 * Generic subtraction operator.
 * We test if the rhs is of a type that we use for a specialized subtraction operation.
 *
 * \param[in]   rhs     The right hand side operand of the subtraction operation.
 *
 * \return              A new object holding the difference.
 */
RbLanguageObject* Real::subtract( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return subtract( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return subtract( static_cast<const Integer&>( rhs ) );
    
    return RlModelVariableWrapper<double>::subtract( rhs );
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
    
    Real *n = new Real( value->getValue() - rhs.getValue() );
    
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
    
    Real *n = new Real( value->getValue() - rhs.getValue() );
    
    return n;
}


