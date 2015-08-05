//
//  Func_varianceCovarianceMatrix.cpp
//  RevBayesCore
//
//  Created by John Huelsenbeck on 11/17/14.
//

#include "Func_decomposedVarianceCovarianceMatrix.h"
#include "MatrixReal.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "DecomposedVarianceCovarianceFunction.h"

using namespace RevLanguage;


/** default constructor */
Func_decomposedVarianceCovarianceMatrix::Func_decomposedVarianceCovarianceMatrix(void) : TypedFunction<MatrixRealSymmetric>()
{
    
}


/** Clone object */
Func_decomposedVarianceCovarianceMatrix* Func_decomposedVarianceCovarianceMatrix::clone(void) const {
    
    return new Func_decomposedVarianceCovarianceMatrix(*this);
}


RevBayesCore::TypedFunction< RevBayesCore::MatrixReal >* Func_decomposedVarianceCovarianceMatrix::createFunction(void) const {

    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* sd = static_cast<const ModelVector<RealPos>& >( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal >* r = static_cast<const MatrixReal& >( this->args[1].getVariable()->getRevObject() ).getDagNode();
    if ( sd->getValue().size() != r->getValue().getNumberOfColumns() )
        {
        throw RbException("The dimension between the standard deviations and the correlation coefficients does not match.");
        }
    for (int i=0; i<r->getValue().getNumberOfRows(); i++)
        {
        for (int j=i+1; j<r->getValue().getNumberOfColumns(); j++)
            {
            if ( r->getValue()[i][j] < -1.0 || r->getValue()[i][j] > 1.0)
                {
                throw RbException("A correlation coefficient is out-of-bounds (-1,+1).");
                }
            }
        }
    RevBayesCore::DecomposedVarianceCovarianceFunction* f = new RevBayesCore::DecomposedVarianceCovarianceFunction( sd, r );
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_decomposedVarianceCovarianceMatrix::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    if ( !rulesSet )
        {
        argumentRules.push_back( new ArgumentRule( "standardDeviations"  , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "correlationCoefficients", MatrixReal::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
        }
    return argumentRules;
}


const std::string& Func_decomposedVarianceCovarianceMatrix::getClassType(void) {
    
    static std::string revType = "Func_decomposedVarianceCovarianceMatrix";
	return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& Func_decomposedVarianceCovarianceMatrix::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
	return revTypeSpec;
}


const TypeSpec& Func_decomposedVarianceCovarianceMatrix::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}
