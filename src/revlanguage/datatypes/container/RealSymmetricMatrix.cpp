//
//  RealSymmetricMatrix.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "RealSymmetricMatrix.h"


#include "ConstantNode.h"
#include "Integer.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "Probability.h"
#include "RealSymmetricMatrix.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <iomanip>
#include <sstream>

using namespace RevLanguage;

/* Default constructor */
RealSymmetricMatrix::RealSymmetricMatrix(void) : RlModelVariableWrapper<RevBayesCore::PrecisionMatrix>( new RevBayesCore::PrecisionMatrix(1) ) {
}


/* Construct from double */
RealSymmetricMatrix::RealSymmetricMatrix( RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix> * mat ) : RlModelVariableWrapper<RevBayesCore::PrecisionMatrix>( mat ) {
}


/* Copy Construct */
RealSymmetricMatrix::RealSymmetricMatrix(const RealSymmetricMatrix& from) : RlModelVariableWrapper<RevBayesCore::PrecisionMatrix>( new RevBayesCore::PrecisionMatrix(from.getValue().getDim()) ) {
    
}

/** Clone object */
RealSymmetricMatrix* RealSymmetricMatrix::clone(void) const {
    
	return new RealSymmetricMatrix(*this);
}


/** Convert to type. The caller manages the returned object. */
RbLanguageObject* RealSymmetricMatrix::convertTo( const TypeSpec& type ) const {
    
    return RbLanguageObject::convertTo( type );
}

/** Get class name of object */
const std::string& RealSymmetricMatrix::getClassName(void) {
    
    static std::string rbClassName = "RealSymmetricMatrix";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& RealSymmetricMatrix::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass;
}

/** Get type spec */
const TypeSpec& RealSymmetricMatrix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Is convertible to type? */
/*
bool RealSymmetricMatrix::isConvertibleTo(const TypeSpec& type) const {
    
    return RbLanguageObject::isConvertibleTo(type);
}
*/

/** Print value for user */
void RealSymmetricMatrix::printValue(std::ostream &o) const {
    
    size_t previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    std::fixed( o );
    o.precision( 3 );
    o << value->getValue();
    
    o.setf( previousFlags );
    o.precision( previousPrecision );
}


