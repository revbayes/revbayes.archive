#include "ConstantNode.h"
#include "Integer.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "Probability.h"
#include "RlMatrixRealSymmetric.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "RlMemberFunction.h"

#include <iomanip>
#include <sstream>

using namespace RevLanguage;

/* Default constructor */
MatrixRealSymmetric::MatrixRealSymmetric(void) : MatrixReal()
{
    
//    ArgumentRules* covArgRules = new ArgumentRules();
//    covArgRules->push_back(new ArgumentRule("i", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
//    covArgRules->push_back(new ArgumentRule("j", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
//    this->methods.addFunction("covariance", new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
//    
//    this->methods.addFunction("precision", new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
//    
//    this->methods.addFunction("correlation", new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
//    
//    this->methods.addFunction("partialCorrelation", new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );

}


/* Construct from double */
MatrixRealSymmetric::MatrixRealSymmetric( RevBayesCore::MatrixReal * mat ) : MatrixReal( mat )
{
    
    //    ArgumentRules* covArgRules = new ArgumentRules();
    //    covArgRules->push_back(new ArgumentRule("i", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    //    covArgRules->push_back(new ArgumentRule("j", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
    //
    //    methods.addFunction("covariance",           new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
    //    methods.addFunction("precision",            new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
    //    methods.addFunction("correlation",          new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
    //    methods.addFunction("partialCorrelation",   new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
    
}


/* Construct from double */
MatrixRealSymmetric::MatrixRealSymmetric( RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal> * mat ) : MatrixReal( mat )
{
    
//    ArgumentRules* covArgRules = new ArgumentRules();
//    covArgRules->push_back(new ArgumentRule("i", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
//    covArgRules->push_back(new ArgumentRule("j", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
//    
//    methods.addFunction("covariance",           new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
//    methods.addFunction("precision",            new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
//    methods.addFunction("correlation",          new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );
//    methods.addFunction("partialCorrelation",   new MemberFunction<MatrixRealSymmetric,Real>( this, covArgRules ) );

}


/** Clone object */
MatrixRealSymmetric* MatrixRealSymmetric::clone(void) const
{
    
	return new MatrixRealSymmetric(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* MatrixRealSymmetric::convertTo( const TypeSpec& type ) const
{
    
    return RevObject::convertTo( type );
}

/** Get Rev type of object */
const std::string& MatrixRealSymmetric::getClassType(void)
{
    
    static std::string revType = "MatrixRealSymmetric";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& MatrixRealSymmetric::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( MatrixReal::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& MatrixRealSymmetric::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void MatrixRealSymmetric::printValue(std::ostream &o) const
{
    
    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    std::fixed( o );
    o.precision( 3 );

    dagNode->printValue( o );
    
    o.setf( previousFlags );
    o.precision( previousPrecision );
}


