//
//  RealMatrix.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-27.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "RealMatrix.h"


#include "ConstantNode.h"
#include "Integer.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "Probability.h"
#include "RealMatrix.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <iomanip>
#include <sstream>

using namespace RevLanguage;

/* Default constructor */
RealMatrix::RealMatrix(void) : ModelObject<RevBayesCore::MatrixReal>( new RevBayesCore::MatrixReal(1,1,0) ) {
}


/* Construct from double */
RealMatrix::RealMatrix( RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal> * mat ) : ModelObject<RevBayesCore::MatrixReal>( mat )
{
    
}



/* Copy Construct */
RealMatrix::RealMatrix(const RealMatrix& from) : ModelObject<RevBayesCore::MatrixReal>( new RevBayesCore::MatrixReal(from.getValue()) ) {
    
}

/** Clone object */
RealMatrix* RealMatrix::clone(void) const {
    
	return new RealMatrix(*this);
}


/** Get Rev type of object */
const std::string& RealMatrix::getClassType(void) {
    
    static std::string revType = "RealMatrix";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& RealMatrix::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get type spec */
const TypeSpec& RealMatrix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void RealMatrix::printValue(std::ostream &o) const {
    
    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    std::fixed( o );
    o.precision( 3 );

    dagNode->printValue( o );
    
    o.setf( previousFlags );
    o.precision( previousPrecision );
}


