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
    
    if (name == "size")
    {
        found = true;
        
        int n = (int)this->dagNode->getValue().getNumberOfStates();
        return new RevVariable( new Natural(n) );
    }
    
    return RateGenerator::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& RateMatrix::getClassType(void) {
    
    static std::string revType = "RateMatrix";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& RateMatrix::getClassTypeSpec(void) {
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RateGenerator::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RateMatrix::getTypeSpec(void) const {
    
    static TypeSpec type_spec = getClassTypeSpec();
    return type_spec;
}

void RateMatrix::initMethods(void) {
 
    // member procedures
    ArgumentRules* sizeArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "size", Natural::getClassTypeSpec(), sizeArgRules) );
    
    // member functions
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the row.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<RateMatrix, ModelVector<Real> >( "[]", this, squareBracketArgRules   ) );
    
    // member functions
    ArgumentRules* transitionProbabilityArgRules = new ArgumentRules();
    transitionProbabilityArgRules->push_back( new ArgumentRule( "rate", RealPos::getClassTypeSpec(), "The rate of the process (or duration of the process assuming rate=1).", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    transitionProbabilityArgRules->push_back( new ArgumentRule( "startAge", RealPos::getClassTypeSpec(), "The start age of the process.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos(1.0) ) );
    transitionProbabilityArgRules->push_back( new ArgumentRule( "endAge", RealPos::getClassTypeSpec(), "The end age of the process.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos(0.0) ) );
    methods.addFunction( new MemberFunction<RateMatrix, ModelVector<ModelVector<RealPos> > >( "getTransitionProbabilities", this, transitionProbabilityArgRules   ) );
}
