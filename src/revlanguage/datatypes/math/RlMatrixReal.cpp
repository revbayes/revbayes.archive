#include "ConstantNode.h"
#include "Integer.h"
#include "MatrixReal.h"
#include "Natural.h"
#include "ModelVector.h"
#include "Real.h"
#include "Probability.h"
#include "RlMatrixReal.h"
#include "RlMemberFunction.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <iomanip>
#include <sstream>

using namespace RevLanguage;

/* Default constructor */
MatrixReal::MatrixReal(void) : ModelObject<RevBayesCore::MatrixReal>( new RevBayesCore::MatrixReal(1,1,0) )
{
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]", new MemberFunction<MatrixReal,ModelVector<Real> >(this, squareBracketArgRules ) );

}

MatrixReal::MatrixReal(const RevBayesCore::MatrixReal& from) : ModelObject<RevBayesCore::MatrixReal>( from.clone() )
{
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]", new MemberFunction<MatrixReal,ModelVector<Real> >(this, squareBracketArgRules ) );

}

MatrixReal::MatrixReal(RevBayesCore::MatrixReal* m) : ModelObject<RevBayesCore::MatrixReal>( m )
{

    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]", new MemberFunction<MatrixReal,ModelVector<Real> >(this, squareBracketArgRules ) );

}

MatrixReal::MatrixReal( RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal> * mat ) : ModelObject<RevBayesCore::MatrixReal>( mat )
{
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("[]", new MemberFunction<MatrixReal,ModelVector<Real> >(this, squareBracketArgRules ) );

}


/** Clone object */
MatrixReal* MatrixReal::clone(void) const
{
    
	return new MatrixReal(*this);
}


/** Get Rev type of object */
const std::string& MatrixReal::getClassType(void)
{
    
    static std::string revType = "MatrixReal";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& MatrixReal::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


///** Return member rules (no members) */
//const MemberRules& MatrixReal::getParameterRules(void) const
//{
//    
//    static MemberRules memberRules;
//    static bool rulesSet = false;
//    
//    if ( !rulesSet )
//    {
//        
//        memberRules.push_back( new ArgumentRule("x", RevObject::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
//        memberRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
//        
//        rulesSet = true;
//    }
//    
//    return memberRules;
//}


/** Get type spec */
const TypeSpec& MatrixReal::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print value for user */
void MatrixReal::printValue(std::ostream &o) const
{
    
    long previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    std::fixed( o );
    o.precision( 3 );

    dagNode->printValue( o );
    
    o.setf( previousFlags );
    o.precision( previousPrecision );
}


