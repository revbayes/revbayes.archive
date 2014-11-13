#include "RlContinuousCharacterData.h"

#include "ConstantNode.h"
#include "Natural.h"
#include "ModelVector.h"
#include "RlContinuousCharacterData.h"
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

}

/** Construct from core data type */
ContinuousCharacterData::ContinuousCharacterData(const RevBayesCore::ContinuousCharacterData &d) :
    ModelObject<RevBayesCore::ContinuousCharacterData>( d.clone() ),
    AbstractCharacterData( NULL )
{
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
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
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
}


ContinuousCharacterData* ContinuousCharacterData::add(const RevObject &d) const
{
    const ContinuousCharacterData* tmp = dynamic_cast<const ContinuousCharacterData*>( &d );
    if ( tmp != NULL )
    {
        return add( *tmp );
    }
    else
    {
        throw RbException("Cannot add an object of type '" + d.getType() + "' to a ContinuousCharacterData object.");
    }
}



ContinuousCharacterData* ContinuousCharacterData::add(const ContinuousCharacterData &d) const
{
    ContinuousCharacterData* cloneObj = clone();
    
    cloneObj->getDagNode()->getValue().add( d.getValue() );
    // return the copy
    return cloneObj;
}


/** Clone object */
ContinuousCharacterData* ContinuousCharacterData::clone(void) const {
    
	return new ContinuousCharacterData(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* ContinuousCharacterData::convertTo(const TypeSpec& type) const {
    
    return RevObject::convertTo(type);
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
    else
    {
        return ModelObject<RevBayesCore::ContinuousCharacterData>::executeMethod(name, args, found);
    }
    
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



/** Is convertible to type? */
bool ContinuousCharacterData::isConvertibleTo(const TypeSpec& type, bool once) const {
    
    return RevObject::isConvertibleTo(type, once);
}


