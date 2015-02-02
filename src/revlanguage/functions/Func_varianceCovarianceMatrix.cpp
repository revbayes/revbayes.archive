//
//  Func_varianceCovarianceMatrix.cpp
//  RevBayesCore
//
//  Created by John Huelsenbeck on 11/17/14.
//

#include "Func_varianceCovarianceMatrix.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "VarianceCovarianceFunction.h"

using namespace RevLanguage;


/** default constructor */
Func_varianceCovarianceMatrix::Func_varianceCovarianceMatrix(void) : TypedFunction<MatrixReal>()
{
    
}


/** Clone object */
Func_varianceCovarianceMatrix* Func_varianceCovarianceMatrix::clone(void) const {
    
    return new Func_varianceCovarianceMatrix(*this);
}


RevBayesCore::TypedFunction< RevBayesCore::MatrixReal >* Func_varianceCovarianceMatrix::createFunction(void) const {

    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* sd = static_cast<const ModelVector<RealPos>& >( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* ce = static_cast<const ModelVector<Real>& >( this->args[1].getVariable()->getRevObject() ).getDagNode();
    if ( sd->getValue().size() != (ce->getValue().size() * (ce->getValue().size()-1) / 2.0) )
        {
        throw RbException("The dimension between the standard deviations and the correlation coefficients does not match.");
        }
    for (int i=0; i<ce->getValue().size(); i++)
        {
        if ( ce->getValue()[i] < -1.0 || ce->getValue()[i] > 1.0)
            {
            throw RbException("A correlation coefficient is out-of-bounds (-1,+1).");
            }
        }
    RevBayesCore::VarianceCovarianceFunction* f = new RevBayesCore::VarianceCovarianceFunction( sd, ce );
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_varianceCovarianceMatrix::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    if ( !rulesSet )
        {
        argumentRules.push_back( new ArgumentRule( "standardDeviations"  , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "correlationCoefficients", ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
        }
    return argumentRules;
}


const std::string& Func_varianceCovarianceMatrix::getClassType(void) {
    
    static std::string revType = "Func_varianceCovarianceMatrix";
	return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_varianceCovarianceMatrix::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
	return revTypeSpec;
}


const TypeSpec& Func_varianceCovarianceMatrix::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}
