#include "ConstantNode.h"
#include "RlBoolean.h"
#include "Integer.h"
#include "Natural.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/* Default constructor */
Integer::Integer(void) : RlModelVariableWrapper<int>() {
    
}


Integer::Integer( RevBayesCore::TypedDagNode<int> *v ) : RlModelVariableWrapper<int>( v ) {
    
}



/* Construct from int */
Integer::Integer(int v) : RlModelVariableWrapper<int>( new int(v) ) {

}


/* Construct from unsigned int (ambiguous between int and bool otherwise) */
Integer::Integer(unsigned int v) : RlModelVariableWrapper<int>( new int(v) ) {
    
}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RbLanguageObject* Integer::add( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return add( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return add( static_cast<const Integer&>( rhs ) );
    
    return RlModelVariableWrapper<int>::add( rhs );
}


/**
 * Specialized addition operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
Real* Integer::add(const Real &rhs) const
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
Integer* Integer::add(const Integer &rhs) const
{
    
    Integer *n = new Integer( value->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone object */
Integer* RevLanguage::Integer::clone(void) const {

	return new Integer(*this);
}


/** Convert to type. The caller manages the returned object. */
RbLanguageObject* Integer::convertTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec() ) 
        return new RlBoolean( value->getValue() == 0 );

    if ( type == Real::getClassTypeSpec() )
        return new Real( value->getValue() );

    if ( type == RlString::getClassTypeSpec() ) 
    {
        std::ostringstream o;
        printValue( o );
        return new RlString( o.str() );
    }

    if ( type == RealPos::getClassTypeSpec() && value->getValue() > 0 )
        return new RealPos( value->getValue() );

    if ( type == Natural::getClassTypeSpec() && value->getValue() >= 0)
        return new Natural( value->getValue() );

    return RbLanguageObject::convertTo( type );
}


/** Get class name of object */
const std::string& Integer::getClassName(void) { 
    
    static std::string rbClassName = "Integer";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Integer::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}





/** Get type spec */
const TypeSpec& Integer::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Is convertible to language object of type? */
bool Integer::isConvertibleTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec())
        return true;

    if ( type == Real::getClassTypeSpec() )
        return true;

    if ( type == RlString::getClassTypeSpec() )
        return true;

    if ( type == RealPos::getClassTypeSpec() && value->getValue() > 0 )
        return true;

    if ( type == Natural::getClassTypeSpec() && value->getValue() >= 0 )
        return true;

    return RbLanguageObject::isConvertibleTo( type );
}
