#include "RlRateMatrix.h"

#include "ArgumentRule.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RlMemberFunction.h"

using namespace RevLanguage;

RateMatrix::RateMatrix(void) : RateGenerator()
{
    initMethods();
}


RateMatrix::RateMatrix( const RevBayesCore::RateMatrix &v) : RateGenerator( v.clone() )
{
    initMethods();
}


RateMatrix::RateMatrix( RevBayesCore::RateMatrix *v) : RateGenerator( v )
{
    initMethods();
}


RateMatrix::RateMatrix( RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator> *m) : RateGenerator( m )
{
    initMethods();
}


RateMatrix* RateMatrix::clone() const
{
    return new RateMatrix( *this );
}


/* Map calls to member methods */
RevPtr<RevVariable> RateMatrix::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "[]")
    {
        found = true;
       
        // get the member with give index
        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
        
        if ( this->dagNode->getValue().getNumberOfStates() < (size_t)(index.getValue()) ) {
            throw RbException("Index out of bounds in []");
        }
        
        RevBayesCore::RbVector<double> elementVector;
        for (size_t i=0; i < this->dagNode->getValue().size(); ++i) {
            double v = this->dagNode->getValue().getRate( (size_t)(index.getValue())-1, i, 0.0, 1.0);
            //                std::cout << v << " ";
            elementVector.push_back( v );
        }
        //            std::cout << "\n";
        
        // MJL: The returned vector is not dynamically updated. Perhaps "[]" should be a MemberFunction, not MemberProcedure?
        return new RevVariable( new ModelVector<Real>( elementVector ) );

    }
//    else if (name == "size")
//    {
//        found = true;
//        
//        int n = (int)this->dagNode->getValue().getNumberOfStates();
//        return new RevVariable( new Natural(n) );
//    }
    
    return RateGenerator::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& RateMatrix::getClassType(void) {
    
    static std::string revType = "RateMatrix";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& RateMatrix::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RateGenerator::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RateMatrix::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}

void RateMatrix::initMethods(void) {
 
    // add method for call "x[]" as a function
//    ArgumentRules* squareBracketArgRules = new ArgumentRules();
//    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the row.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
//    methods.addFunction( new MemberProcedure( "[]", ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the row.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<RateMatrix, ModelVector<Real> >( "[]", this, squareBracketArgRules   ) );
//    methods.addFunction( new MemberFunction( "[]", ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
//    ArgumentRules* avgExtinctionArgRules = new ArgumentRules();
//    methods.addFunction( new DistributionMemberFunction<Dist_heterogeneousRateBirthDeath, ModelVector<RealPos> >( "averageExtinctionRate", variable, avgExtinctionArgRules   ) );

}
