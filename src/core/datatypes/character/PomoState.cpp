/**
 * @file
 * This file contains the implementation of PomoState, which is
 * the base class for the DNA character data type plus two-state polymorphic states in RevBayes.
 *
 * @brief Implementation of PomoState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: PomoState.cpp 1568 2012-05-24 07:58:04Z hoehna $
 */

#include "PomoState.h"
#include "RbException.h"
#include <assert.h>
#include <sstream>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace RevBayesCore;

/** Default constructor */
PomoState::PomoState(void) : DiscreteCharacterState(), state( 0xFF ), virtualPopulationSize ( 10 ) {
    
}

/** Constructor with virtual population size */
PomoState::PomoState(unsigned int vps): DiscreteCharacterState(), state( 0xFF ), virtualPopulationSize ( vps ) {
    
    
}


/** Constructor that sets the observation */
PomoState::PomoState(std::string s) : DiscreteCharacterState() {
    
    //assert( s <= 15 );
    
    setState(s);
}


/** Equals comparison */
bool PomoState::operator==(const CharacterState& x) const {
    
    const PomoState* derivedX = dynamic_cast<const PomoState*>( &x );
    
    if (derivedX != NULL) {
        return derivedX->state == state;
    }
    
    return false;
}


/** Not equals comparison */
bool PomoState::operator!=(const CharacterState& x) const {
    
    return !operator==(x);
}


bool PomoState::operator<(const CharacterState &x) const {
    
    const PomoState* derivedX = static_cast<const PomoState*>(&x);
    if ( derivedX != NULL )
    {
        unsigned int myState = state;
        unsigned int yourState = derivedX->state;
        return ( myState < yourState );
    }
    return false;
}


void PomoState::operator++( void ) {
    
    state += 1;
    
}


void PomoState::operator++( int i ) {
    
    state += 1;
    
}

void PomoState::operator+=( int i ) {
    
    state += i;
    
}


void PomoState::operator--( void ) {
    
    state -= 1;
    
}


void PomoState::operator--( int i ) {
    
    state -= 1;
    
}

void PomoState::operator-=( int i ) {
    
    state -= i;
    
}

void PomoState::addState(std::string symbol) {
    
    state = computeState( symbol );
    
}

void PomoState::addState(char symbol) {
    
    state = computeState( boost::lexical_cast<std::string>( symbol )  );
}


PomoState* PomoState::clone( void ) const {
    
    return new PomoState( *this );
}


unsigned int PomoState::computeState(std::string symbol) const {
    /* Example with only ten states:
     A C G T A10C90 A20C80 A30C70...A90C10 A10G90 A20G80...A10T90...C10G90...C10T90...G10T90 
     */
    if (symbol.length()==1) {
            if (symbol == "-")
                return 0;
            if (symbol ==  "A")
                return 1;
            if (symbol ==  "C")
                return 2;
            if (symbol ==  "G")
                return 3;
            if (symbol ==  "T")
                return 4;
            else return 0;
    }
    else if (symbol.length()!=6 ) {
        throw RbException( "Pomo string state with fewer or more than 6 characters. Should be 6, no more, no less." );
    }
    
    std::string firstChar = symbol.substr(0,1);
    int firstFreq = atoi ( symbol.substr(1,2).c_str() );
    std::string secondChar = symbol.substr(3,1);
    int secondFreq = atoi ( symbol.substr(4,2).c_str() );
    if ( firstFreq + secondFreq > virtualPopulationSize ) {
        throw RbException( "Pomo string state with frequencies that do not add up to the current virtual population size." );
    }
    if ( firstChar >= secondChar ) {
        throw RbException( "Pomo string state with first state greater or equal to second state." );
    }
    
    int stepSize = 100 / virtualPopulationSize;
    int numStep = firstFreq / stepSize -1;

    if (firstChar ==  "A") {
        if (secondChar ==  "C")
            return 5 + numStep;
        if (secondChar ==  "G")
            return 5 + virtualPopulationSize - 1 + numStep;
        if (secondChar ==  "T")
            return 5 + 2*(virtualPopulationSize - 1) + numStep;
        else {
            throw RbException( "Pomo string state with incorrect second state: should be A, C, G or T." );
        }
    }
    if (firstChar ==  "C") {
        if (secondChar ==  "G")
            return 5 + 3*(virtualPopulationSize - 1) + numStep;
        if (secondChar ==  "T")
            return 5 + 4*(virtualPopulationSize - 1) + numStep;
        else {
            throw RbException( "Pomo string state with incorrect second state: should be A, C, G or T." );
        }
    }
    if (firstChar ==  "G") {
        if (secondChar ==  "T")
            return 5 + 5*(virtualPopulationSize - 1) + numStep;
        else {
            throw RbException( "Pomo string state with incorrect second state: should be A, C, G or T." );
        }

    }
    else {
        throw RbException( "Pomo string state with incorrect first state: should be A, C, G or T." );
    }

    
}


std::string PomoState::getDatatype( void ) const {
    
    return "Pomo";
}


unsigned int PomoState::getNumberObservedStates(void) const  {
    ///PROBABLY DOES NOT WORK FOR POLYMORPHIC STATES.
    char v = state;     // count the number of bits set in v
    char c;             // c accumulates the total bits set in v
    
    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    
    return (unsigned int)c;
}


size_t PomoState::getNumberOfStates( void ) const {
    
    return 4 + 6 * (virtualPopulationSize - 1);
}


unsigned long PomoState::getState( void ) const {
    return (unsigned long)state;
}


size_t  PomoState::getStateIndex(void) const {
    return (size_t)state;
}


const std::string& PomoState::getStateLabels( void ) const {
    
    static std::string labels = "A C G T ";
    std::string acgt( "ACGT" );
    std::vector< size_t > frequencies;
    int stepSize = 100 / virtualPopulationSize;
    for (size_t i = 1; i < virtualPopulationSize; ++i) {
        frequencies.push_back(i*stepSize);
    }
    BOOST_FOREACH( char ch, acgt )
    {
        BOOST_FOREACH( char ch2, acgt.substr(1, 3 ) )
        {
            for (size_t i = 0; i < virtualPopulationSize-1; ++i) {
                labels += ch + boost::lexical_cast<std::string>(frequencies[i]) + ch2 + boost::lexical_cast<std::string>(frequencies[virtualPopulationSize - 2 - i]) + " ";
            }
        }
    }
    return labels;
}

std::string PomoState::getStringValue(void) const  {
    
    if ( isMissingState() )
    {
        return "?";
    }
    
    if ( isGapState() )
    {
        return "-";
    }
    
    int stepSize = 100 / virtualPopulationSize - 1;
    if (state < 5) {
        switch ( state )
        {
            case 0:
                return "-";
            case 1:
                return "A";
            case 2:
                return "C";
            case 3:
                return "G";
            case 4:
                return "T";
        }
    }
    int stateMinus5 = state - 5;
    int typeOfPomoState = stateMinus5 / stepSize;
    int typeOfFrequency = stateMinus5 % stepSize +1;
    int freqi = typeOfFrequency*virtualPopulationSize;
    int freqj = 100 - freqi;
    switch ( typeOfPomoState )
    {
        case 0: //AC
            return "A"+ boost::lexical_cast<std::string>(freqi) + "C" + boost::lexical_cast<std::string>(freqj) ;
        case 1: //AG
            return "A"+ boost::lexical_cast<std::string>(freqi) + "G" + boost::lexical_cast<std::string>(freqj) ;
        case 2: //AT
            return "A"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
        case 3: //CG
            return "C"+ boost::lexical_cast<std::string>(freqi) + "G" + boost::lexical_cast<std::string>(freqj) ;
        case 4: //CT
            return "C"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
        case 5: //GT
            return "G"+ boost::lexical_cast<std::string>(freqi) + "T" + boost::lexical_cast<std::string>(freqj) ;
    }
     throw RbException( "getStringValue called on a non-standard state." );
}



bool PomoState::isAmbiguous( void ) const
{
    
    return getNumberObservedStates() > 1;
}


void PomoState::setState(size_t pos, bool val) {
    
    throw RbException( "setState(size_t pos, bool val) is not implemented in PomoState" );
}

void PomoState::setState(std::string symbol)
{
    state = computeState( symbol ) ;
}


void PomoState::setState(char symbol) {
    state = computeState(  boost::lexical_cast<std::string>(symbol) ) ;
}

void PomoState::setState(size_t stateIndex) {
    state = (int)stateIndex ;
}


void PomoState::setToFirstState( void ) {
    
    state = 1;
    
}

void PomoState::setVirtualPopulationSize(unsigned int populationSize)
{
    if (populationSize > 100) {
        throw RbException( "The virtual population size should be < 100 and should be a divisor of 100." );
    }
    if (100 % populationSize != 0) {
        throw RbException( "The virtual population size should be a divisor of 100." );
    }
    virtualPopulationSize = populationSize;
}

