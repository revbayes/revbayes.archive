
#include "StandardState.h"
#include "RbException.h"
#include <sstream>
#include <cstdlib>
#include <cassert>

using namespace RevBayesCore;

/** Default constructor */
StandardState::StandardState(void) : DiscreteCharacterState( 32 ),
    labels( "0123456789ABCDEFGHIJKLMNOPQRSTUV" )
{

}


/** Default constructor */
StandardState::StandardState(const std::string &s) : DiscreteCharacterState( 32 ),
    labels( "0123456789ABCDEFGHIJKLMNOPQRSTUV" )
{
    setState(s);
    
}

StandardState::StandardState(const std::string& s, const std::string &l) : DiscreteCharacterState( l.size() ),
    labels( l )
{
    setState(s);
}


std::string StandardState::getDataType( void ) const
{
    
    return "Standard";
}



StandardState* StandardState::clone( void ) const
{
    return new StandardState( *this );
}


const std::string& StandardState::getStateLabels( void ) const
{
    
    return labels;
}


