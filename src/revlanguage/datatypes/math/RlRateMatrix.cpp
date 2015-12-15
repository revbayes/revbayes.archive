#include "RlRateMatrix.h"

#include "ArgumentRule.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlBoolean.h"

using namespace RevLanguage;

RateMatrix::RateMatrix(void) : RateGenerator()
{

}


RateMatrix::RateMatrix( const RevBayesCore::RateMatrix &v) : RateGenerator( v.clone() )
{
    
}


RateMatrix::RateMatrix( RevBayesCore::RateMatrix *v) : RateGenerator( v )
{
    
}


RateMatrix::RateMatrix( RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator> *m) : RateGenerator( m )
{
    
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

        
        throw RbException("Currently deprecated. Blame Michael (or Sebastian)!");
        // get the member with give index
//        const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
//        
//        if (this->dagNode->getValue().getNumberOfStates() < (size_t)(index.getValue()) ) {
//            throw RbException("Index out of bounds in []");
//        }
//        
//        const std::vector<double>& element = this->dagNode->getValue()[ size_t(index.getValue()) - 1];
//        RevBayesCore::RbVector<double> elementVector;
//        for (size_t i=0; i < this->dagNode->getValue().size(); ++i) {
//            elementVector.push_back( element[i] );
//        }
//        
//        return new RevVariable( new ModelVector<Real>( elementVector ) );
    }
    else if (name == "size")
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
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RateMatrix::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}

