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
	std::cout << boost::lexical_cast<std::string>(state) << " = ChromosomesState - default" << std::endl;
    // Should the default state be 1? Why was it originally 0xFF? Does 0xFF=int(255)?
}



/** Copy constructor */
ChromosomesState::ChromosomesState(const ChromosomesState& s) : DiscreteCharacterState(), state( s.state ) {
    
}


/** Constructor that sets the observation */
ChromosomesState::ChromosomesState(std::string s) : DiscreteCharacterState() {
    
	std::cout << s << " = ChromosomesState - observed" << std::endl;
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
		std::cout << symbol << " = computeState - input string" << std::endl;
		return boost::lexical_cast<int>( symbol );
	} catch( boost::bad_lexical_cast const& ) {
		throw RbException( "Chromosome state was not valid integer." );
	}
    
}


std::string ChromosomesState::getDatatype( void ) const {
    
    return "Chromosomes";
}


unsigned int ChromosomesState::getNumberObservedStates(void) const  {
    
	// since chromosomes always have 1 state (at least currently) s
	// should this always return 1???
	
    //char v = state;     // count the number of bits set in v
    //char c;             // c accumulates the total bits set in v
    
    //for (c = 0; v; v >>= 1)
    //{
    //    c += v & 1;
    //}
    
    //return (unsigned int)c;
	return 1;
}


size_t ChromosomesState::getNumberOfStates( void ) const {
    
    // TODO: need to get this value from user
	return (size_t)35;
}


unsigned long ChromosomesState::getState( void ) const {
	std::cout << boost::lexical_cast<std::string>(state) << " = getState" << std::endl;
    return (int)state;
}


size_t  ChromosomesState::getStateIndex(void) const {
	std::cout << boost::lexical_cast<std::string>(state) << " = getStateIndex" << std::endl;
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
	std::cout << symbol << " = setState - string" << std::endl;
    state = computeState( symbol ) ;
	std::cout << boost::lexical_cast<std::string>(state) << " = setState - int set as state!!" << std::endl;
}


void ChromosomesState::setState(char symbol) {
	std::cout << boost::lexical_cast<std::string>(state) << " = setState - char !" << std::endl;
    state = computeState(  boost::lexical_cast<std::string>(symbol) ) ;
}

void ChromosomesState::setState(size_t stateIndex) {
	// when is stateIndex used?
	std::cout << boost::lexical_cast<std::string>(state) << " = setState - index" << std::endl;
    //state = (int)stateIndex;
}


void ChromosomesState::setToFirstState( void ) {
    
    state = 1;
    
}



