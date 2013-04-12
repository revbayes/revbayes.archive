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
#include "RlBoolean.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RbUtil.h"
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

    return RbLanguageObject::convertTo( type );
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


/** Is convertible to type? */
bool Real::isConvertibleTo(const TypeSpec& type) const {

    if (type == RlBoolean::getClassTypeSpec())
        return true;
    if (type == RealPos::getClassTypeSpec() && value->getValue() > 0.0)
        return true;
    if (type == Probability::getClassTypeSpec() && value->getValue() >= 0.0 && value->getValue() <= 1.0)
        return true;

    return RbLanguageObject::isConvertibleTo(type);
}



/** Print value for user */
void Real::printValue(std::ostream &o) const {

    size_t previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    std::fixed( o );
    o.precision( 3 );
    o << value->getValue();

    o.setf( previousFlags );
    o.precision( previousPrecision );
}


