/*
 *  ChromosomesState.cpp
 *  revbayes-proj
 *
 *  Created by mucho mass on 9/8/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ChromosomesState.h"
#include "RbException.h"
#include <assert.h>
#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>


using namespace RevBayesCore;

/** Default constructor */
ChromosomesState::ChromosomesState(void) : DiscreteCharacterState(), state( 1 ) {
    // default state = 1
}



/** Copy constructor */
ChromosomesState::ChromosomesState(const ChromosomesState& s) : DiscreteCharacterState(), state( s.state ) {
    
}


/** Constructor that sets the observation */
ChromosomesState::ChromosomesState(std::string s) : DiscreteCharacterState() {
    setState(s);
}


/** Equals comparison */
bool ChromosomesState::operator==(const CharacterState& x) const {
    
    const ChromosomesState* derivedX = dynamic_cast<const ChromosomesState*>( &x );
    
    if (derivedX != NULL) {
        return derivedX->state == state;
    }
    
    return false;
}


/** Not equals comparison */
bool ChromosomesState::operator!=(const CharacterState& x) const {
    
    return !operator==(x);
}


bool ChromosomesState::operator<(const CharacterState &x) const {
    
    const ChromosomesState* derivedX = static_cast<const ChromosomesState*>(&x);
    if ( derivedX != NULL )
    {
        unsigned int myState = state;
        unsigned int yourState = derivedX->state;
        return ( myState < yourState );
    }
    return false;
}


void ChromosomesState::operator++( void ) {
    
    state += 1;
    
}


void ChromosomesState::operator++( int i ) {
    
    state += 1;
    
}


void ChromosomesState::operator--( void ) {
    
    state -= 1;
    
}


void ChromosomesState::operator--( int i ) {
    
    state -= 1;
    
}


void ChromosomesState::addState(std::string symbol) {
    
    state = computeState( symbol );
    
}

void ChromosomesState::addState(char symbol) {
    
    state = computeState( boost::lexical_cast<std::string>( symbol )  );
}


ChromosomesState* ChromosomesState::clone( void ) const {
    
    return new ChromosomesState( *this );
}


unsigned int ChromosomesState::computeState(std::string symbol) const {
	
	try {
		return boost::lexical_cast<int>( symbol );
	} catch( boost::bad_lexical_cast const& ) {
		throw RbException( "Chromosome state was not valid integer." );
	}
    
}


std::string ChromosomesState::getDatatype( void ) const {
    
    return "Chromosomes";
}


unsigned int ChromosomesState::getNumberObservedStates(void) const  {
    
	// currently this always return 1 because we only allow one observation per tip
	return 1;
}


size_t ChromosomesState::getNumberOfStates( void ) const {
    
	return RevBayesCore::g_MAX_NUM_CHROMOSOMES;
}


unsigned long ChromosomesState::getState( void ) const {
    return (int)state;
}


size_t  ChromosomesState::getStateIndex(void) const {
    return (size_t)state-1;
}


const std::string& ChromosomesState::getStateLabels( void ) const {
	
    return boost::lexical_cast<std::string>(state);

}

std::string ChromosomesState::getStringValue(void) const  {
    
	return boost::lexical_cast<std::string>(state);
	
}



bool ChromosomesState::isAmbiguous( void ) const {
    
    return getNumberObservedStates() > 1;
}


bool ChromosomesState::isGapState( void ) const {
    
    return state == 0x0;
}


void ChromosomesState::setGapState(bool tf) {
    
    if ( tf )
    {
        state = 0x0;
    }
    else
    {
        state = 0xF;
    }
}


void ChromosomesState::setState(size_t pos, bool val) {
    
    throw RbException( "setState(size_t pos, bool val) is not implemented in ChromosomesState" );
}

void ChromosomesState::setState(std::string symbol) {
    state = computeState( symbol ) ;
}


void ChromosomesState::setState(char symbol) {
    state = computeState(  boost::lexical_cast<std::string>(symbol) ) ;
}

void ChromosomesState::setState(size_t stateIndex) {
    state = (int)stateIndex+1;
}


void ChromosomesState::setToFirstState( void ) {
    
    state = 1;
    
}

// set to 1 by default, this should be reset by phyloCTMC
size_t RevBayesCore::g_MAX_NUM_CHROMOSOMES = 1;



