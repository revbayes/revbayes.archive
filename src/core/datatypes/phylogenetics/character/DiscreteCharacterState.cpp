#include "DiscreteCharacterState.h"


using namespace RevBayesCore;


bool DiscreteCharacterState::isAmbiguous( void ) const
{
    
    return isMissingState() || isGapState() || ( getNumberObservedStates() > 1 );
}
