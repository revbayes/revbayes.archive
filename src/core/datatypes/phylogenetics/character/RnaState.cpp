#include "RnaState.h"
#include <sstream>

using namespace RevBayesCore;

/** Default constructor */
RnaState::RnaState(void) : DiscreteCharacterState( 4 )
{
    
}


/** Constructor that sets the observation */
RnaState::RnaState(const std::string &s) : DiscreteCharacterState( 4 )
{
    
    setState(s);
}


std::string RnaState::getDataType( void ) const
{
    return "RNA";
}



RnaState* RnaState::clone( void ) const
{
    return new RnaState( *this );
}


const std::string& RnaState::getStateLabels( void ) const
{
    
    static std::string labels = "ACGU";
    
    return labels;
}

std::string RnaState::getStringValue(void) const
{
    
    if ( isMissingState() )
    {
        return "?";
    }
    
    if ( isGapState() )
    {
        return "-";
    }
    
    
    unsigned int val = 0x0;
    for ( size_t i=0; i<state.size(); ++i )
    {
        val <<= 1;
        if ( state.isSet(i) == true )
        {
            val &= 1;
        }
    }
    
    switch ( val )
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
            return "U";
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


void RnaState::setState(const std::string &symbol)
{

    char s = char( toupper( symbol[0] ) );
    
    switch ( s )
    {
        case '-':
            break;
        case 'A':
            state.set(0);
            index_single_state = 0;
            num_observed_states = 1;
            break;
        case 'C':
            state.set(1);
            index_single_state = 1;
            num_observed_states = 1;
            break;
        case 'M':
            state.set(0);
            state.set(1);
            num_observed_states = 2;
            break;
        case 'G':
            state.set(2);
            index_single_state = 2;
            num_observed_states = 1;
            break;
        case 'R':
            state.set(0);
            state.set(2);
            num_observed_states = 2;
            break;
        case 'S':
            state.set(1);
            state.set(2);
            num_observed_states = 2;
            break;
        case 'V':
            state.set(0);
            state.set(1);
            state.set(2);
            num_observed_states = 3;
            break;
        case 'U':
            state.set(3);
            index_single_state = 3;
            num_observed_states = 1;
            break;
        case 'W':
            state.set(0);
            state.set(3);
            num_observed_states = 2;
            break;
        case 'Y':
            state.set(1);
            state.set(3);
            num_observed_states = 2;
            break;
        case 'H':
            state.set(0);
            state.set(1);
            state.set(3);
            num_observed_states = 3;
            break;
        case 'K':
            state.set(2);
            state.set(3);
            num_observed_states = 2;
            break;
        case 'D':
            state.set(0);
            state.set(2);
            state.set(3);
            num_observed_states = 3;
            break;
        case 'B':
            state.set(1);
            state.set(2);
            state.set(3);
            num_observed_states = 3;
            break;
        case 'N':
            state.set(0);
            state.set(1);
            state.set(2);
            state.set(3);
            num_observed_states = 4;
            break;
            
        default:
            state.set(0);
            state.set(1);
            state.set(2);
            state.set(3);
            num_observed_states = 4;
    }
}

