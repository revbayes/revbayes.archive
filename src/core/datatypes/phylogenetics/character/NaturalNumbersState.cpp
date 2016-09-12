#include "NaturalNumbersState.h"
#include "RbException.h"
#include <assert.h>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>


using namespace RevBayesCore;

/** Default constructor */
NaturalNumbersState::NaturalNumbersState(void) : DiscreteCharacterState( 10 )
{

}



/** Constructor that sets the observation */
NaturalNumbersState::NaturalNumbersState(const std::string &s, int m) : DiscreteCharacterState( m )
{
    setState(s);
}


/** Constructor that sets the observation */
NaturalNumbersState::NaturalNumbersState(int s, int m) : DiscreteCharacterState( m )
{
    setStateByIndex( s );
}


NaturalNumbersState* NaturalNumbersState::clone( void ) const
{
    
    return new NaturalNumbersState( *this );
}


void NaturalNumbersState::addState(int s)
{
    
    state.set( s );
    ++num_observed_states;
}


std::string NaturalNumbersState::getDataType( void ) const
{
    
    return "NaturalNumbers";
}


const std::string& NaturalNumbersState::getStateLabels( void ) const
{
    std::string labels = "";
    size_t n = getNumberOfStates();
    for (size_t i=0; i<n; ++i)
    {
        labels += boost::lexical_cast<std::string>(n);
    }
    return labels;
	
}

std::string NaturalNumbersState::getStringValue(void) const
{
    
    if ( isMissingState() )
    {
        return "?";
    }
    
    if ( isGapState() )
    {
        return "-";
    }
    
	return boost::lexical_cast<std::string>(state);
	
}


void NaturalNumbersState::setState(const std::string &symbol)
{
    
    if (symbol == "-")
    {
        setGapState( true );
    }
    else if ( symbol == "?")
    {
        setMissingState( true );
    }
    else
    {
        try
        {
            state.clear();
            size_t pos = boost::lexical_cast<size_t>( symbol );
            state.set( pos );
            num_observed_states = 1;
            index_single_state = pos;
        }
        catch( boost::bad_lexical_cast const& )
        {
            
            throw RbException( "NaturalNumbers state was not valid integer." );
        }
        
    }
    
}

