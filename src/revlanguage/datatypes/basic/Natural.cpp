#include "ConstantNode.h"
#include "RlBoolean.h"
#include "Natural.h"
#include "Integer.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"
#include <climits>
#include <sstream>
#include <climits>

using namespace RevLanguage;

/** Default constructor */
Natural::Natural( void ) : Integer( 0 ) {
}


Natural::Natural( RevBayesCore::TypedDagNode<int> *v ) : Integer( v ) {
    
}


/** Construct from int */
Natural::Natural( int x ) : Integer( x ) {

    if ( x < 0 )
        throw RbException( "Negative value for " + getClassName() );
}


/* Construct from unsigned int */
Natural::Natural( unsigned int x ) : Integer( x ) {
        
}


/* Construct from unsigned long */
Natural::Natural( unsigned long x) : Integer( int(x) ) {

    if ( x > INT_MAX )
        throw RbException( "Value out of range for " + getClassName() );

}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RbLanguageObject* Natural::add( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec() == Natural::getClassTypeSpec() )
        return add( static_cast<const Natural&>( rhs ) );
    
    if ( rhs.getTypeSpec() == RealPos::getClassTypeSpec() )
        return add( static_cast<const RealPos&>( rhs ) );
    
    return Integer::add( rhs );
}


/**
 * Specialized addition operation between two natural numbers.
 * The return value is also of type natural number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
Natural* Natural::add(const RevLanguage::Natural &rhs) const
{
    
    Natural *n = new Natural( value->getValue() + rhs.getValue() );
    
    return n;
}


RealPos* Natural::add(const RevLanguage::RealPos &rhs) const
{
    
    RealPos *n = new RealPos( value->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone object */
Natural* Natural::clone( void ) const {

	return new Natural( *this );
}


/** Convert to type. The caller manages the returned object. */
RbLanguageObject* Natural::convertTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec() )
        return new RlBoolean( value->getValue() == 0 );

    if ( type == Real::getClassTypeSpec() )
        return new Real( value->getValue() );

    if ( type == RealPos::getClassTypeSpec() )
        return new RealPos( value->getValue() );

    if ( type == RlString::getClassTypeSpec() ) {

        std::ostringstream o;
        printValue( o );
        return new RlString( o.str() );
    }

    return Integer::convertTo( type );
}


/** Get class name of object */
const std::string& Natural::getClassName(void) { 
    
    static std::string rbClassName = "Natural";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Natural::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Integer::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Natural::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Is convertible to type? */
bool Natural::isConvertibleTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec() )
        return true;

    if ( type == Real::getClassTypeSpec() )
        return true;

    if ( type == RealPos::getClassTypeSpec() )
        return true;

    if ( type == RlString::getClassTypeSpec() )
        return true;

    return Integer::isConvertibleTo( type );
}
