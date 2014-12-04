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
NaturalNumbersState::NaturalNumbersState(void) : DiscreteCharacterState(), state( 0 ) {
    // default state = 0
}



/** Copy constructor */
NaturalNumbersState::NaturalNumbersState(const NaturalNumbersState& s) : DiscreteCharacterState(), state( s.state ) {
    
}


/** Constructor that sets the observation */
NaturalNumbersState::NaturalNumbersState(std::string s) : DiscreteCharacterState() {
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


void NaturalNumbersState::operator--( void ) {
    
		state -= 1;
	
}


void NaturalNumbersState::operator--( int i ) {
    
    state -= 1;
    
}


void NaturalNumbersState::addState(std::string symbol) {
    
    state = computeState( symbol );
    
}

void NaturalNumbersState::addState(char symbol) {
    
    state = computeState( boost::lexical_cast<std::string>( symbol )  );
}


NaturalNumbersState* NaturalNumbersState::clone( void ) const {
    
    return new NaturalNumbersState( *this );
}


unsigned int NaturalNumbersState::computeState(std::string symbol) const {
	
	try {
		return boost::lexical_cast<int>( symbol );
	} catch( boost::bad_lexical_cast const& ) {
		throw RbException( "NaturalNumbers state was not valid integer." );
	}
    
}


std::string NaturalNumbersState::getDatatype( void ) const {
    
    return "NaturalNumbers";
}


unsigned int NaturalNumbersState::getNumberObservedStates(void) const  {
    
	// currently this always return 1 because we only allow one observation per tip
	return 1;
}


size_t NaturalNumbersState::getNumberOfStates( void ) const {
    
	return RevBayesCore::g_MAX_NAT_NUM_STATES;
}


unsigned long NaturalNumbersState::getState( void ) const {
    return (int)state;
}


size_t  NaturalNumbersState::getStateIndex(void) const {
    return (size_t)state;
}


const std::string& NaturalNumbersState::getStateLabels( void ) const {
	
    return boost::lexical_cast<std::string>(state);
	
}

std::string NaturalNumbersState::getStringValue(void) const  {
    
	return boost::lexical_cast<std::string>(state);
	
}



bool NaturalNumbersState::isAmbiguous( void ) const {
    
    return getNumberObservedStates() > 1;
}


bool NaturalNumbersState::isGapState( void ) const {
    
	return false;
}


void NaturalNumbersState::setGapState(bool tf) {
    
    if ( tf )
    {
        state = 0x0;
    }
    else
    {
        state = 0xF;
    }
}


void NaturalNumbersState::setState(size_t pos, bool val) {
    
    throw RbException( "setState(size_t pos, bool val) is not implemented in NaturalNumbersState" );
}

void NaturalNumbersState::setState(std::string symbol) {
    state = computeState( symbol ) ;
}


void NaturalNumbersState::setState(char symbol) {
    state = computeState(  boost::lexical_cast<std::string>(symbol) ) ;
}

void NaturalNumbersState::setState(size_t stateIndex) {
    state = (int)stateIndex;
}


void NaturalNumbersState::setToFirstState( void ) {
    
    state = 0;
    
}

// set to 1 by default, this should be reset by phyloCTMC
size_t RevBayesCore::g_MAX_NAT_NUM_STATES = 1;



