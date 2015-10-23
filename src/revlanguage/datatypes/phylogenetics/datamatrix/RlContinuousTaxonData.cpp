#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "Real.h"
#include "RlBoolean.h"
#include "RlContinuousTaxonData.h"
#include "RlString.h"

using namespace RevLanguage;

ContinuousTaxonData::ContinuousTaxonData(void) : ModelObject< RevBayesCore::ContinuousTaxonData >()
{
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "[]", Real::getClassTypeSpec(), squareBracketArgRules) );

}


ContinuousTaxonData::ContinuousTaxonData( RevBayesCore::ContinuousTaxonData *v) : ModelObject< RevBayesCore::ContinuousTaxonData >( v )
{
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), "The index of the taxon.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    this->methods.addFunction( new MemberProcedure( "[]", Real::getClassTypeSpec(), squareBracketArgRules) );

}



ContinuousTaxonData* RevLanguage::ContinuousTaxonData::clone() const
{
    return new ContinuousTaxonData( *this );
}


/* Map calls to member methods */
RevPtr<RevLanguage::RevVariable> ContinuousTaxonData::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found) {
    
    if ( name == "[]")
    {
        found = true;
        
        // get the member with give index
        const Natural &index = static_cast<const Natural &>( args[0].getVariable()->getRevObject() );
    
        if (this->dagNode->getValue().size() < (size_t)(index.getValue()) )
        {
            throw RbException("Index out of bounds in []");
        }
        
        RevObject* element = new Real( this->dagNode->getValue().getCharacter( size_t(index.getValue()) - 1) );
        return new RevVariable( element );
            
    }
    
    return ModelObject< RevBayesCore::ContinuousTaxonData >::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& ContinuousTaxonData::getClassType(void) {
    
    static std::string revType = "ContinuousTaxonData";
    
    return revType;
}


/* Get class type spec describing type of object */
const TypeSpec& ContinuousTaxonData::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& ContinuousTaxonData::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}

