#include "Natural.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;
    
/** Default constructor */
RealPos::RealPos( void ) : Real( 1.0 ) {
}


/** Construct from double */
RealPos::RealPos( RevBayesCore::TypedDagNode<double> *x ) : Real( x ) {
    
    if ( x->getValue() < 0.0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/** Construct from double */
RealPos::RealPos( double x ) : Real( x ) {

    if ( x < 0.0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/** Construct from int */
RealPos::RealPos( int x ) : Real( x ) {

    if ( x < 0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RbLanguageObject* RealPos::add( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec() == RealPos::getClassTypeSpec() )
        return add( static_cast<const RealPos&>( rhs ) );
    
    if ( rhs.getTypeSpec() == Natural::getClassTypeSpec() )
        return add( static_cast<const Natural&>( rhs ) );
    
    return Real::add( rhs );
}


/**
 * Specialized addition operation between two positive real numbers.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RealPos* RealPos::add(const RevLanguage::Natural &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() + rhs.getValue() );
    
    return n;
}


/**
 * Specialized addition operation between two positive real numbers.
 * The return value is also of type positive real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RealPos* RealPos::add(const RevLanguage::RealPos &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone object */
RealPos* RealPos::clone( void ) const {

	return  new RealPos( *this );
}


/** Get class name of object */
const std::string& RealPos::getClassName(void) { 
    
    static std::string rbClassName = "RealPos";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RealPos::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Real::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RealPos::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

