#include "AminoAcidState.h"

#include <stdio.h>
#include <iostream>

using namespace RevBayesCore;

/** Default constructor */
<<<<<<< HEAD
AminoAcidState::AminoAcidState(size_t n) : DiscreteCharacterState( 20 )
=======
AminoAcidState::AminoAcidState(size_t n) : DiscreteCharacterState( 20 ),
    is_gap( false ),
    is_missing( false ),
    index_single_state( 0 ),
    num_observed_states( 0 ),
    state(20)
>>>>>>> development
{
    
}


/** Constructor that sets the observation */
<<<<<<< HEAD
AminoAcidState::AminoAcidState(const std::string &s) : DiscreteCharacterState( 20 )
=======
AminoAcidState::AminoAcidState(const std::string &s) : DiscreteCharacterState( 20 ),
    is_gap( false ),
    is_missing( false ),
    index_single_state( 0 ),
    num_observed_states( 0 ),
    state(20)
>>>>>>> development
{
    
    setState(s);
}


/* Clone object */
AminoAcidState* AminoAcidState::clone(void) const
{
    
	return new AminoAcidState( *this );
}


std::string AminoAcidState::getDataType( void ) const
{
    return "Protein";
}


std::string AminoAcidState::getStateLabels( void ) const
{
    
    static const std::string stateLabels = "ARNDCQEGHILKMFPSTWYV";
   
    return stateLabels;
}


<<<<<<< HEAD
=======
void AminoAcidState::addState(const std::string &symbol)
{
    ++num_observed_states;
    
    std::string labels = getStateLabels();
    size_t pos = labels.find(symbol);
    
    state.set( pos );
    index_single_state = pos;
}


RbBitSet AminoAcidState::getState(void) const
{
    return state;
}


bool AminoAcidState::isGapState( void ) const
{
    return is_gap;
}


bool AminoAcidState::isMissingState( void ) const
{
    return is_missing;
}


void AminoAcidState::setGapState( bool tf )
{
    is_gap = tf;
}


void AminoAcidState::setMissingState( bool tf )
{
    is_missing = tf;
}


void AminoAcidState::setToFirstState(void)
{
    num_observed_states = 1;
    index_single_state = 0;
    state.clear();
    state.set( 0 );
}



void AminoAcidState::setState(const std::string &s)
{
    
    std::string labels = getStateLabels();
    
    num_observed_states = 0;
    state.clear();
    
    for (size_t i = 0; i < s.size(); i++)
    {
        ++num_observed_states;
        
        size_t pos = labels.find(s[i]);
        state.set(pos);
        index_single_state = pos;
    }
    
}


void AminoAcidState::setStateByIndex(size_t index)
{
    
    num_observed_states = 1;
    index_single_state = index;
    state.clear();
    state.set( index );
}


>>>>>>> development
