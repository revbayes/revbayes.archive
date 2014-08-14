#include "RlAbstractDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTaxonData.h"


using namespace RevLanguage;

AbstractDiscreteCharacterData::AbstractDiscreteCharacterData(void) : AbstractCharacterData() 
{
    
}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( RevBayesCore::AbstractDiscreteCharacterData *v) : AbstractCharacterData( v )
{
    
}


AbstractDiscreteCharacterData::AbstractDiscreteCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractCharacterData> *d) : AbstractCharacterData( d ) 
{

}


AbstractDiscreteCharacterData* AbstractDiscreteCharacterData::clone() const {
    
    return new AbstractDiscreteCharacterData( *this );
}


/* Get class name of object */
const std::string& AbstractDiscreteCharacterData::getClassType(void) {
    
    static std::string revClassType = "AbstractDiscreteCharacterData";
    
	return revClassType; 
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractDiscreteCharacterData::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( AbstractCharacterData::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}


/**
 * Get member methods. We construct the appropriate static member
 * function table here.
 */
const MethodTable& AbstractDiscreteCharacterData::getMethods( void ) const
{
    static MethodTable  myMethods   = MethodTable();
    static bool         methodsSet  = false;
    
    if ( !methodsSet )
    {
        myMethods = makeMethods();
        methodsSet = true;
    }
    
    return myMethods;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& AbstractDiscreteCharacterData::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


const RevBayesCore::AbstractDiscreteCharacterData& RevLanguage::AbstractDiscreteCharacterData::getValue( void ) const 
{
    
    return static_cast<const RevBayesCore::AbstractDiscreteCharacterData&>( this->dagNode->getValue() );
}



