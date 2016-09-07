#include "AminoAcidState.h"

#include <stdio.h>
#include <iostream>

using namespace RevBayesCore;

/** Default constructor */
AminoAcidState::AminoAcidState(void) : DiscreteCharacterState( 20 )
{
    
    setState("-");
}


/** Constructor that sets the observation */
AminoAcidState::AminoAcidState(const std::string &s) : DiscreteCharacterState( 20 )
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


const std::string& AminoAcidState::getStateLabels( void ) const
{
    
    static const std::string stateLabels = "ARNDCQEGHILKMFPSTWYV";
   
    return stateLabels;
}


