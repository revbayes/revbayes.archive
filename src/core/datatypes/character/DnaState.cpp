#include "DnaState.h"
#include "RbException.h"
#include <assert.h>
#include <sstream>
#include <iostream>

using namespace RevBayesCore;

/** Default constructor */
DnaState::DnaState(void) : DiscreteCharacterState(),
    state( char(0xFF) ),
    stateIndex(4)
{
    
}


/** Constructor that sets the observation */
DnaState::DnaState(const std::string &s) : DiscreteCharacterState()
{
    
    assert( s.size() == 1 && s[0] <= 15 );
    
    setState(s);
}


/** Equals comparison */
bool DnaState::operator==(const CharacterState& x) const
{
    
    const DnaState* derivedX = dynamic_cast<const DnaState*>( &x );
    
    if (derivedX != NULL)
    {
        return derivedX->state == state;
    }
    
    return false;
}


/** Not equals comparison */
bool DnaState::operator!=(const CharacterState& x) const
{
    
    return !operator==(x);
}


bool DnaState::operator<(const CharacterState &x) const
{
    
    const DnaState* derivedX = static_cast<const DnaState*>(&x);
    if ( derivedX != NULL ) 
    {
        char myState = state;
        char yourState = derivedX->state;
        
        while ( myState != yourState && (myState & 1) == ( yourState & 1 )  ) 
        {
            myState >>= 1;
            yourState >>= 1;
        }
        
        return (myState & 1) > ( yourState & 1 );
    }    
    
    return false;
}


void DnaState::operator++( void )
{
    
    state <<= 1;
    ++stateIndex;

}


void DnaState::operator++( int i )
{

    state <<= 1;
    ++stateIndex;

}

void DnaState::operator+=( int i )
{
    
    state <<= i;
    stateIndex += i;
    
}

void DnaState::operator--( void )
{
    
    state >>= 1;
    --stateIndex;

}


void DnaState::operator--( int i )
{
    
    state >>= 1;
    --stateIndex;

}

void DnaState::operator-=( int i )
{
    
    state >>= i;
    stateIndex -= i;
    
}

void DnaState::addState(const std::string &symbol)
{
    
    // check if the state was previously set
    if ( state == 0x0 ) // it was not set
    {
        setState( symbol ); // this will also set the stateIndex
    }
    else // it was set
    {
        state |= computeState( symbol ); // we cannot have two indices
    }

}



DnaState* DnaState::clone( void ) const
{
    
    return new DnaState( *this );
}


unsigned int DnaState::computeState(const std::string &symbol) const
{
    
    char s = char( toupper( symbol[0] ) );
    switch ( s )
    {
        case '-':
            return 0x00;
        case 'A':
            return 0x01;
        case 'C':
            return 0x02;
        case 'M':
            return 0x03;
        case 'G':
            return 0x04;
        case 'R':
            return 0x05;
        case 'S':
            return 0x06;
        case 'V':
            return 0x07;
        case 'T':
            return 0x08;
        case 'W':
            return 0x09;
        case 'Y':
            return 0x0A;
        case 'H':
            return 0x0B;
        case 'K':
            return 0x0C;
        case 'D':
            return 0x0D;
        case 'B':
            return 0x0E;
        case 'N':
            return 0x0F;
            
        default:
            return 0x0F;
    }
}


std::string DnaState::getDataType( void ) const
{
    
    return "DNA";
}


unsigned int DnaState::getNumberObservedStates(void) const
{
    
    char v = state;     // count the number of bits set in v
    char c;             // c accumulates the total bits set in v
    
    if ( state == '\x80' && stateIndex == 7)
    {
        std::cerr << "ohoh\n";
    }
    
    for (c = 0; v; v >>= 1)
    {
        c += v & 1;
    }
    
    return (unsigned int)c;
}


size_t DnaState::getNumberOfStates( void ) const
{
    
    return 4;
}


unsigned long DnaState::getState( void ) const
{
    
    return (unsigned long)state;
}

size_t DnaState::getStateIndex( void ) const
{
    
    return stateIndex;
}

const std::string& DnaState::getStateLabels( void ) const
{
    
    static std::string labels = "ACGT";
    
    return labels;
}

std::string DnaState::getStringValue(void) const
{
    
    if ( isMissingState() )
    {
        return "?";
    }
    
    if ( isGapState() )
    {
        return "-";
    }
    
    switch ( state )
    {
        case 0x0:
            return "-";
        case 0x1:
            return "A";
        case 0x2:
            return "C";
        case 0x3:
            return "M";
        case 0x4:
            return "G";
        case 0x5:
            return "R";
        case 0x6:
            return "S";
        case 0x7:
            return "V";
        case 0x8:
            return "T";
        case 0x9:
            return "W";
        case 0xA:
            return "Y";
        case 0xB:
            return "H";
        case 0xC:
            return "K";
        case 0xD:
            return "D";
        case 0xE:
            return "B";
        case 0xF:
            return "N";
            
        default:
            return "?";
    }
}



bool DnaState::isAmbiguous( void ) const
{
    
    return getNumberObservedStates() > 1;
}


void DnaState::setStateByIndex(size_t index)
{
    
    stateIndex = index;
    
    // compute the state from the index
    state = 0x1;
    state <<= (index-1);
    
}

void DnaState::setState(const std::string &symbol)
{
    state = char( computeState( symbol ) );
    switch ( state )
    {
        case 0x1: { stateIndex = 0; break; }
        case 0x2: { stateIndex = 1; break; }
        case 0x4: { stateIndex = 2; break; }
        case 0x8: { stateIndex = 3; break; }
        default: stateIndex = 4;
    }
    
}

void DnaState::setToFirstState( void )
{
    
    state = 0x1;
    stateIndex = 0;

}

