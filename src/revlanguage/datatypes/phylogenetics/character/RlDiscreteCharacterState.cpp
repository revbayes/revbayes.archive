#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "Natural.h"
#include "NaturalNumbersState.h"
#include "RlDiscreteCharacterState.h"
#include "RbUtil.h"
#include "StandardState.h"
#include "StringUtilities.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
DiscreteCharacterState::DiscreteCharacterState(void) : ModelObject<RevBayesCore::DiscreteCharacterState>()
{
    
}

/** Construct from bool */
DiscreteCharacterState::DiscreteCharacterState(const RevBayesCore::DiscreteCharacterState &d) : ModelObject<RevBayesCore::DiscreteCharacterState>( d.clone() )
{
    
}


/** Clone object */
DiscreteCharacterState* DiscreteCharacterState::clone(void) const
{
    
    return new DiscreteCharacterState(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* DiscreteCharacterState::convertTo( const TypeSpec& type ) const
{
    
    
    if ( type == Natural::getClassTypeSpec() )
    {
        const RevBayesCore::NaturalNumbersState* ns =dynamic_cast<const RevBayesCore::NaturalNumbersState*>( &getValue() );
        if ( ns != NULL )
        {
            return new Natural( StringUtilities::asIntegerNumber( ns->getStringValue() ) );
        }
        
        const RevBayesCore::StandardState* ss =dynamic_cast<const RevBayesCore::StandardState*>( &getValue() );
        if ( ss != NULL && StringUtilities::isIntegerNumber( ss->getStringValue() ) )
        {
            return new Natural( StringUtilities::asIntegerNumber( ss->getStringValue() ) );
        }
        
    }
    
    return ModelObject<RevBayesCore::DiscreteCharacterState>::convertTo( type );
}



/** Get Rev type of object */
const std::string& DiscreteCharacterState::getClassType(void)
{
    
    static std::string revType = "DiscreteCharacter";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& DiscreteCharacterState::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/** Get type spec */
const TypeSpec& DiscreteCharacterState::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Is convertible to type? */
double DiscreteCharacterState::isConvertibleTo( const TypeSpec& type, bool once ) const
{
    
    if ( once == true && type == Natural::getClassTypeSpec() )
    {
        const RevBayesCore::NaturalNumbersState* ns =dynamic_cast<const RevBayesCore::NaturalNumbersState*>( &getValue() );
        if ( ns != NULL )
        {
            return 0.1;
        }
        
        const RevBayesCore::StandardState* ss =dynamic_cast<const RevBayesCore::StandardState*>( &getValue() );
        if ( ss != NULL && StringUtilities::isIntegerNumber( ss->getStringValue() ) )
        {
            return 0.1;
        }
        
    }
    
    return ModelObject<RevBayesCore::DiscreteCharacterState>::isConvertibleTo( type, once );
}

