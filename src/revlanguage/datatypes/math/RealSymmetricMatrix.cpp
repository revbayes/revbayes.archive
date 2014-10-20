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
#include "RlMemberFunction.h"

#include <iomanip>
#include <sstream>

using namespace RevLanguage;

/* Default constructor */
RealSymmetricMatrix::RealSymmetricMatrix(void) : ModelObject<RevBayesCore::MatrixRealSymmetric>( new RevBayesCore::MatrixRealSymmetric(1) ) {
}


/* Construct from double */
RealSymmetricMatrix::RealSymmetricMatrix( RevBayesCore::TypedDagNode<RevBayesCore::MatrixRealSymmetric> * mat ) : ModelObject<RevBayesCore::MatrixRealSymmetric>( mat ) {
}


/* Copy Construct */
RealSymmetricMatrix::RealSymmetricMatrix(const RealSymmetricMatrix& from) : ModelObject<RevBayesCore::MatrixRealSymmetric>( new RevBayesCore::MatrixRealSymmetric(from.getValue().getDim()) ) {
    
}

/** Clone object */
RealSymmetricMatrix* RealSymmetricMatrix::clone(void) const {
    
	return new RealSymmetricMatrix(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* RealSymmetricMatrix::convertTo( const TypeSpec& type ) const {
    
    return RevObject::convertTo( type );
}

/** Get Rev type of object */
const std::string& RealSymmetricMatrix::getClassType(void) {
    
    static std::string revType = "RealSymmetricMatrix";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& RealSymmetricMatrix::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& RealSymmetricMatrix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Make member methods for this class */
void RealSymmetricMatrix::initializeMethods( void ) const
{
    // Insert inherited methods
    ModelObject<RevBayesCore::MatrixRealSymmetric>::initializeMethods();
    
    ArgumentRules* covArgRules = new ArgumentRules();
    covArgRules->push_back(new ArgumentRule("i", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    covArgRules->push_back(new ArgumentRule("j", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    this->methods.addFunction("covariance", new MemberFunction<RealSymmetricMatrix,Real>( this, covArgRules ) );
    
    this->methods.addFunction("precision", new MemberFunction<RealSymmetricMatrix,Real>( this, covArgRules ) );
    
    this->methods.addFunction("correlation", new MemberFunction<RealSymmetricMatrix,Real>( this, covArgRules ) );
    
    this->methods.addFunction("partialCorrelation", new MemberFunction<RealSymmetricMatrix,Real>( this, covArgRules ) );
    
}


/** Print value for user */
void RealSymmetricMatrix::printValue(std::ostream &o) const {
    
    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    std::fixed( o );
    o.precision( 3 );

    dagNode->printValue( o );
    
    o.setf( previousFlags );
    o.precision( previousPrecision );
}


