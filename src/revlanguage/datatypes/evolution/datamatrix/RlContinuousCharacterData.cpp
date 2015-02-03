#include "RlContinuousCharacterData.h"

#include "ConstantNode.h"
#include "Natural.h"
#include "ModelVector.h"
#include "RlContinuousTaxonData.h"
#include "RlString.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
ContinuousCharacterData::ContinuousCharacterData(void) :
    ModelObject<RevBayesCore::ContinuousCharacterData>(),
    AbstractCharacterData( NULL )
{
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("[]",  new MemberProcedure( ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("getTaxon",  new MemberProcedure( ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );

    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );

}

/** Construct from core data type */
ContinuousCharacterData::ContinuousCharacterData(const RevBayesCore::ContinuousCharacterData &d) :
    ModelObject<RevBayesCore::ContinuousCharacterData>( d.clone() ),
    AbstractCharacterData( NULL )
{
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("[]",  new MemberProcedure( ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("getTaxon",  new MemberProcedure( ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );

    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
}

/** Construct from core data type */
ContinuousCharacterData::ContinuousCharacterData(RevBayesCore::ContinuousCharacterData *d) :
    ModelObject<RevBayesCore::ContinuousCharacterData>( d ),
    AbstractCharacterData( NULL )
{
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("[]",  new MemberProcedure( ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("getTaxon",  new MemberProcedure( ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );

    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
}


ContinuousCharacterData::ContinuousCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::ContinuousCharacterData> *d) :
    ModelObject<RevBayesCore::ContinuousCharacterData>( d ),
    AbstractCharacterData( NULL )
{
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    // Add method for call "x[]" as a function
    ArgumentRules* squareBracketArgRules = new ArgumentRules();
    squareBracketArgRules->push_back( new ArgumentRule( "index" , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("[]",  new MemberProcedure( ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules) );
    
    ArgumentRules* squareBracketArgRules2 = new ArgumentRules();
    squareBracketArgRules2->push_back( new ArgumentRule( "name" , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    this->methods.addFunction("getTaxon",  new MemberProcedure( ContinuousTaxonData::getClassTypeSpec(), squareBracketArgRules2) );

    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
}


ContinuousCharacterData* ContinuousCharacterData::concatenate(const RevObject &d) const
{
    const ContinuousCharacterData* tmp = dynamic_cast<const ContinuousCharacterData*>( &d );
    if ( tmp != NULL )
    {
        return concatenate( *tmp );
    }
    else
    {
        throw RbException("Cannot add an object of type '" + d.getType() + "' to a ContinuousCharacterData object.");
    }
}



ContinuousCharacterData* ContinuousCharacterData::concatenate(const ContinuousCharacterData &d) const
{
    ContinuousCharacterData* cloneObj = clone();
    
    cloneObj->getDagNode()->getValue().concatenate( d.getValue() );
    // return the copy
    return cloneObj;
}


/** Clone object */
ContinuousCharacterData* ContinuousCharacterData::clone(void) const {
    
	return new ContinuousCharacterData(*this);
}


/* Map calls to member methods */
RevPtr<RevVariable> ContinuousCharacterData::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if ( this->getDagNode() != NULL )
    {
        // set the internal value pointer
        setCharacterDataObject( &this->getDagNode()->getValue() );
    }
    
    RevPtr<RevVariable> retVal = executeCharacterDataMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    else if (name == "[]" || name == "getTaxon")
    {
        found = true;
        
        if ( args[0].getVariable()->getRevObject().isType( Natural::getClassTypeSpec() ) )
        {
            // get the member with give index
            const Natural& index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() );
        
            if (this->dagNode->getValue().getNumberOfTaxa() < (size_t)(index.getValue()) )
            {
                throw RbException("Index out of bounds in []");
            }
        
            const RevBayesCore::ContinuousTaxonData& element = static_cast< RevBayesCore::ContinuousCharacterData& >( this->dagNode->getValue() ).getTaxonData(size_t(index.getValue()) - 1);
        
            return new RevVariable( new ContinuousTaxonData( new RevBayesCore::ContinuousTaxonData( element ) ) );
        }
        else
        {
            // get the member with give index
            const std::string &index = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
            
            const RevBayesCore::ContinuousTaxonData& element = static_cast< RevBayesCore::ContinuousCharacterData& >( this->dagNode->getValue() ).getTaxonData(index);            
            return new RevVariable( new ContinuousTaxonData( new RevBayesCore::ContinuousTaxonData( element ) ) );
        }
    }
    
    
    return ModelObject<RevBayesCore::ContinuousCharacterData>::executeMethod( name, args, found );
    
}


/** Get Rev type of object */
const std::string& ContinuousCharacterData::getClassType(void) {
    
    static std::string revType = "ContinuousCharacterData";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& ContinuousCharacterData::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( ModelObject<RevBayesCore::ContinuousCharacterData>::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get type spec */
const TypeSpec& ContinuousCharacterData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


