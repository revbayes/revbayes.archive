/**
 * @file
 * This file contains the implementation of NaturalNumbersState, which is
 * the base class for character data types that are represented
 * as natural numbers (such as morphological characters) in RevBayes.
 *
 * @brief Implementation of NaturalNumbersState
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */

#include "NaturalNumbersState.h"
#include "RbException.h"
#include <assert.h>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>


using namespace RevBayesCore;

/** Default constructor */
NaturalNumbersState::NaturalNumbersState(void) : DiscreteCharacterState(),
    state( 0 )
{
    // default state = 0
}



/** Constructor that sets the observation */
NaturalNumbersState::NaturalNumbersState(const std::string &s) : DiscreteCharacterState()
{
    setState(s);
}


/** Equals comparison */
bool NaturalNumbersState::operator==(const CharacterState& x) const {
    
    const NaturalNumbersState* derivedX = dynamic_cast<const NaturalNumbersState*>( &x );
    
    if (derivedX != NULL) {
        return derivedX->state == state;
    }
    
    return false;
}


/** Not equals comparison */
bool NaturalNumbersState::operator!=(const CharacterState& x) const {
    
    return !operator==(x);
}


bool NaturalNumbersState::operator<(const CharacterState &x) const {
    
    const NaturalNumbersState* derivedX = static_cast<const NaturalNumbersState*>(&x);
    if ( derivedX != NULL )
    {
        unsigned int myState = state;
        unsigned int yourState = derivedX->state;
        return ( myState < yourState );
    }
    return false;
}


void NaturalNumbersState::operator++( void ) {
    
    state += 1;
    
}


void NaturalNumbersState::operator++( int i ) {
    
		state += 1;
	
}

void NaturalNumbersState::operator+=( int i ) {
    
    state += i;
	
}

void NaturalNumbersState::operator--( void ) {
    
		state -= 1;
	
}


void NaturalNumbersState::operator--( int i )
{
    
    state -= 1;
    
}

void NaturalNumbersState::operator-=( int i )
{
    
    state -= i;
    
}


void NaturalNumbersState::addState(const std::string &symbol)
{
    
    state = computeState( symbol );
    
}


NaturalNumbersState* NaturalNumbersState::clone( void ) const
{
    
    return new NaturalNumbersState( *this );
}


int NaturalNumbersState::computeState(const std::string &symbol) const
{
	
	if (symbol == "-" || symbol == "?")
    {
		return -1;
    }
    else
    {
		try
        {
			return boost::lexical_cast<int>( symbol );
		}
        catch( boost::bad_lexical_cast const& )
        {
            
			throw RbException( "NaturalNumbers state was not valid integer." );
		}
    }
}


std::string NaturalNumbersState::getDataType( void ) const
{
    
    return "NaturalNumbers";
}


unsigned int NaturalNumbersState::getNumberObservedStates(void) const
{
    
	// currently this always return 1 because we only allow one observation per tip
	return 1;
}


size_t NaturalNumbersState::getNumberOfStates( void ) const
{
    
	return RevBayesCore::g_MAX_NAT_NUM_STATES;
}


unsigned long NaturalNumbersState::getState( void ) const
{
    return (int)state;
}


size_t NaturalNumbersState::getStateIndex(void) const
{
    return (size_t)state;
}


const std::string& NaturalNumbersState::getStateLabels( void ) const
{
    return boost::lexical_cast<std::string>(state);
	
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



bool NaturalNumbersState::isAmbiguous( void ) const
{
    
    return getNumberObservedStates() > 1;
}


void NaturalNumbersState::setState(const std::string &symbol)
{
    state = computeState( symbol ) ;
}


void NaturalNumbersState::setStateByIndex(size_t stateIndex)
{
    state = (int)stateIndex;
}


void NaturalNumbersState::setToFirstState( void )
{
    
    state = 0;
    
}

// set to 1 by default, this should be reset by phyloCTMC
size_t RevBayesCore::g_MAX_NAT_NUM_STATES = 1;



