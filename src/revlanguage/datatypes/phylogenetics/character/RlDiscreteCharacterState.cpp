#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RlDiscreteCharacterState.h"
#include "RbUtil.h"
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

