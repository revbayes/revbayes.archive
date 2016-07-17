#include "CodonState.h"
#include "RbException.h"

#include <sstream>

using namespace RevBayesCore;

/** Default constructor */
CodonState::CodonState(void) : DiscreteCharacterState()
{
    
    setState("---");
}


/** Constructor that sets the observation */
CodonState::CodonState(const std::string &s) : DiscreteCharacterState()
{
    
    setState(s);
}


/** Constructor that sets the observation */
CodonState::CodonState(size_t index) : DiscreteCharacterState()
{
    
    setStateByIndex(index);
}


/* Equals comparison */
bool CodonState::operator==(const CharacterState& x) const
{
    
    const CodonState* derivedX = static_cast<const CodonState*>(&x);
    if ( derivedX != NULL )
    {
        return state == derivedX->state;
    }
    
    return false;
}


/* Not equals comparison */
bool CodonState::operator!=(const CharacterState& x) const
{
    
    return !operator==(x);
}


bool CodonState::operator<(const CharacterState &x) const
{
    
    const CodonState* derivedX = static_cast<const CodonState*>(&x);
    if ( derivedX != NULL )
    {
        unsigned int myState = state;
        unsigned int yourState = derivedX->state;
        
        while ( (myState & 1) == ( yourState & 1 )  )
        {
            myState >>= 1;
            yourState >>= 1;
        }
        
        return (myState & 1) > ( yourState & 1 );
    }
    
    return false;
}

void CodonState::operator++( void )
{
    
    unsigned int codon_pos_0 = state & 0x00F;
    unsigned int codon_pos_1 = state & 0x0F0;
    unsigned int codon_pos_2 = state & 0xF00;
    
    codon_pos_0 <<= 1;
    
    // check for overflow
    if ( (codon_pos_0 & 0x00F) == 0 )
    {
        // reset to first bit
        codon_pos_0 = 0x001;
        
        // increase second codon position
        codon_pos_1 <<= 1;
        
        // check for overflow
        if ( (codon_pos_1 & 0x0F0) == 0 )
        {
            // reset to first bit
            codon_pos_1 = 0x010;
            
            // increase second codon position
            codon_pos_2 <<= 1;
            
            // check for overflow
            if ( (codon_pos_2 & 0xF00) == 0 )
            {
                // reset to first bit
                codon_pos_2 = 0x100;
                
            }
            
        }
    }
    
    state = codon_pos_0 | codon_pos_1 | codon_pos_2;
    
    ++stateIndex;
}

void CodonState::operator++( int i )
{
    unsigned int codon_pos_0 = state & 0x00F;
    unsigned int codon_pos_1 = state & 0x0F0;
    unsigned int codon_pos_2 = state & 0xF00;
    
    codon_pos_0 <<= 1;
    
    // check for overflow
    if ( (codon_pos_0 & 0x00F) == 0 )
    {
        // reset to first bit
        codon_pos_0 = 0x001;
        
        // increase second codon position
        codon_pos_1 <<= 1;
        
        // check for overflow
        if ( (codon_pos_1 & 0x0F0) == 0 )
        {
            // reset to first bit
            codon_pos_1 = 0x010;
            
            // increase second codon position
            codon_pos_2 <<= 1;
            
            // check for overflow
            if ( (codon_pos_2 & 0xF00) == 0 )
            {
                // reset to first bit
                codon_pos_2 = 0x100;
                
            }
            
        }
    }
    
    state = codon_pos_0 | codon_pos_1 | codon_pos_2;
    
    ++stateIndex;
}

void CodonState::operator+=( int i )
{
    state <<= i;
    stateIndex += i;
    
    throw RbException("Missing implementation in codon-state.");
}

void CodonState::operator--( void )
{
    unsigned int codon_pos_0 = state & 0x00F;
    unsigned int codon_pos_1 = state & 0x0F0;
    unsigned int codon_pos_2 = state & 0xF00;
    
    codon_pos_0 >>= 1;
    
    // check for underflow
    if ( codon_pos_0 == 0 )
    {
        // reset to last bit
        codon_pos_0 = 0x008;
        
        // decrease second codon position
        codon_pos_1 >>= 1;
        
        // check for underflow
        if ( codon_pos_1 == 0 )
        {
            // reset to last bit
            codon_pos_1 = 0x080;
            
            // decrease second codon position
            codon_pos_2 >>= 1;
            
            // check for overflow
            if ( codon_pos_2 == 0 )
            {
                // reset to last bit
                codon_pos_2 = 0x800;
                
            }
            
        }
    }
    
    state = codon_pos_0 | codon_pos_1 | codon_pos_2;
    
    --stateIndex;
}


void CodonState::operator--( int i )
{
    unsigned int codon_pos_0 = state & 0x00F;
    unsigned int codon_pos_1 = state & 0x0F0;
    unsigned int codon_pos_2 = state & 0xF00;
    
    codon_pos_0 >>= 1;
    
    // check for underflow
    if ( codon_pos_0 == 0 )
    {
        // reset to last bit
        codon_pos_0 = 0x008;
        
        // decrease second codon position
        codon_pos_1 >>= 1;
        
        // check for underflow
        if ( codon_pos_1 == 0 )
        {
            // reset to last bit
            codon_pos_1 = 0x080;
            
            // decrease second codon position
            codon_pos_2 >>= 1;
            
            // check for overflow
            if ( codon_pos_2 == 0 )
            {
                // reset to last bit
                codon_pos_2 = 0x800;
                
            }
            
        }
    }
    
    state = codon_pos_0 | codon_pos_1 | codon_pos_2;
    
    --stateIndex;
}

void CodonState::operator-=( int i )
{
    state >>= i;
    stateIndex -= i;
    
    throw RbException("Missing implementation in codon-state.");
}

void CodonState::addState(const std::string &symbol)
{
    state |= computeState( symbol );
}


/* Clone object */
CodonState* CodonState::clone(void) const
{
    
    return new CodonState( *this );
}


unsigned int CodonState::computeState(const std::string &symbol) const
{
    
    unsigned int current_state = 0x00000;
    
    /* A C G T */
    char s1 = char( toupper( symbol[2] ) );
    
    switch ( s1 )
    {
        case 'A':
            current_state |= 0x001;
            break;
        case 'C':
            current_state |= 0x002;
            break;
        case 'G':
            current_state |= 0x004;
            break;
        case 'T':
            current_state |= 0x008;
            break;
            
        default:
            current_state |= 0x8FF;
    }
    
    /* A C G T */
    char s2 = char( toupper( symbol[1] ) );
    
    switch ( s2 )
    {
        case 'A':
            current_state |= 0x010;
            break;
        case 'C':
            current_state |= 0x020;
            break;
        case 'G':
            current_state |= 0x040;
            break;
        case 'T':
            current_state |= 0x080;
            break;
            
        default:
            current_state |= 0x8FF;
    }
    
    /* A C G T */
    char s3 = char( toupper( symbol[0] ) );
    
    switch ( s3 )
    {
        case 'A':
            current_state |= 0x100;
            break;
        case 'C':
            current_state |= 0x200;
            break;
        case 'G':
            current_state |= 0x400;
            break;
        case 'T':
            current_state |= 0x800;
            break;
            
        default:
            current_state |= 0x8FF;
    }
    
    return current_state;
}


AminoAcidState CodonState::getAminoAcidState( void ) const
{
    
    AminoAcidState aa;
    
    unsigned int codon_pos_0 = state & 0x00F;
    unsigned int codon_pos_1 = state & 0x0F0;
    unsigned int codon_pos_2 = state & 0xF00;
    
    // test if the first codon position is an 'A'
    if ( (codon_pos_2 & 0x100) == 0x100 )
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos_1 & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 || (codon_pos_0 & 0x004) == 0x004 )
            {
                // we have a Lysine
                aa.addState("K");
            }
            else if ( (codon_pos_0 & 0x002) == 0x002 || (codon_pos_0 & 0x008) == 0x008 )
            {
                // we have a Asparagine
                aa.addState("N");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos_1 & 0x020) == 0x020 ) // test if the second codon position is an 'C'
        {
                
            // we have a Threonine
            aa.addState("T");
            
        }
        else if ( (codon_pos_1 & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 || (codon_pos_0 & 0x004) == 0x004 )
            {
                // we have a Arginine
                aa.addState("R");
            }
            else if ( (codon_pos_0 & 0x002) == 0x002 || (codon_pos_0 & 0x008) == 0x008 )
            {
                // we have a Serine
                aa.addState("S");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos_1 & 0x080) == 0x080 ) // test if the second codon position is an 'T'
        {
            
            // test if the third codon position is an 'A' or a 'C' or a 'T'
            if ( (codon_pos_0 & 0x001) == 0x001 || (codon_pos_0 & 0x002) == 0x002 || (codon_pos_0 & 0x008) == 0x008 )
            {
                // we have a Isoleucine
                aa.addState("I");
            }
            else if ( (codon_pos_0 & 0x004) == 0x004 )
            {
                // we have a Methionine
                aa.addState("M");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else
        {
            throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
        }
        
    }
    else if ( (codon_pos_2 & 0x200) == 0x200 ) // test if the first codon position is a 'C'
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos_1 & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 || (codon_pos_0 & 0x004) == 0x004 )
            {
                // we have a Glutamine
                aa.addState("Q");
            }
            else if ( (codon_pos_0 & 0x002) == 0x002 || (codon_pos_0 & 0x008) == 0x008 )
            {
                // we have a Histidine
                aa.addState("H");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos_1 & 0x020) == 0x020 ) // test if the second codon position is an 'C'
        {
            
            // we have a Proline
            aa.addState("P");
            
        }
        else if ( (codon_pos_1 & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // we have a Arginine
            aa.addState("R");
            
        }
        else if ( (codon_pos_1 & 0x080) == 0x080 ) // test if the second codon position is an 'T'
        {
            
            // we have a Leucine
            aa.addState("L");
            
        }
        else
        {
            throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
        }
        
    }
    else if ( (codon_pos_2 & 0x400) == 0x400 ) // test if the first codon position is a 'G'
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos_1 & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 || (codon_pos_0 & 0x004) == 0x004 )
            {
                // we have a Glutamic Acid
                aa.addState("E");
            }
            else if ( (codon_pos_0 & 0x002) == 0x002 || (codon_pos_0 & 0x008) == 0x008 )
            {
                // we have a Aspartic Acid
                aa.addState("D");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos_1 & 0x020) == 0x020 ) // test if the second codon position is an 'C'
        {
            
            // we have a Alanine
            aa.addState("A");
            
        }
        else if ( (codon_pos_1 & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // we have a Glycine
            aa.addState("G");
            
        }
        else if ( (codon_pos_1 & 0x080) == 0x080 ) // test if the second codon position is an 'T'
        {
            
            // we have a Valine
            aa.addState("V");
            
        }
        else
        {
            throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
        }
        
    }
    else if ( (codon_pos_2 & 0x800) == 0x800 ) // test if the first codon position is an 'T'
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos_1 & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 || (codon_pos_0 & 0x004) == 0x004 )
            {
                // we have a stopp codon
                aa.addState("?");
            }
            else if ( (codon_pos_0 & 0x002) == 0x002 || (codon_pos_0 & 0x008) == 0x008 )
            {
                // we have a Tyrosine
                aa.addState("Y");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos_1 & 0x020) == 0x020 ) // test if the second codon position is an 'C'
        {
            
            // we have a Serine
            aa.addState("S");
            
        }
        else if ( (codon_pos_1 & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 )
            {
                // we have a stopp codon
                aa.addState("?");
            }
            else if ( (codon_pos_0 & 0x004) == 0x004 )
            {
                // we have a Tryptophan
                aa.addState("W");
            }
            else if ( (codon_pos_0 & 0x002) == 0x002 || (codon_pos_0 & 0x008) == 0x008 )
            {
                // we have a Cysteine
                aa.addState("C");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else if ( (codon_pos_1 & 0x080) == 0x080 ) // test if the second codon position is an 'T'
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 || (codon_pos_0 & 0x004) == 0x004)
            {
                // we have a Leucine
                aa.addState("L");
            }
            else if ( (codon_pos_0 & 0x002) == 0x002 || (codon_pos_0 & 0x008) == 0x008 )
            {
                // we have a Phenylalanine
                aa.addState("F");
            }
            else
            {
                throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
            }
            
        }
        else
        {
            throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
        }
        
    }
    else
    {
        throw RbException("Unknown amino acid for codon '" + getStringValue() + "'");
    }

    
    return aa;
}


std::string CodonState::getDataType( void ) const
{
    return "CODON";
}


unsigned int CodonState::getNumberObservedStates( void ) const
{
    return 1;
}


size_t CodonState::getNumberOfStates( void ) const
{
    return 64;
}


unsigned long CodonState::getState( void ) const
{
    return state;
}

size_t CodonState::getStateIndex( void ) const
{
    return stateIndex;
}

const std::string& CodonState::getStateLabels( void ) const
{
    
    static const std::string stateLabels = "AAA...TTT";
    
    return stateLabels;
}

std::string CodonState::getStringValue(void) const
{
    
    if ( isMissingState() )
    {
        return "?";
    }
    
    if ( isGapState() )
    {
        return "-";
    }
    
    std::vector<unsigned int> codon_pos = std::vector<unsigned int>(3,0);
    codon_pos[0] = state & 0x00F;
    codon_pos[1] = state & 0x0F0;
    codon_pos[2] = state & 0xF00;
    
    std::string str_val = "";
    
    for (size_t i=0; i<3; ++i)
    {
        
        switch ( codon_pos[i] )
        {
            case 0x0:
                str_val += "-";
                break;
            case 0x1:
                str_val += "A";
                break;
            case 0x2:
                str_val += "C";
                break;
            case 0x3:
                str_val += "M";
                break;
            case 0x4:
                str_val += "G";
                break;
            case 0x5:
                str_val += "R";
                break;
            case 0x6:
                str_val += "S";
                break;
            case 0x7:
                str_val += "V";
                break;
            case 0x8:
                str_val += "T";
                break;
            case 0x9:
                str_val += "W";
                break;
            case 0xA:
                str_val += "Y";
                break;
            case 0xB:
                str_val += "H";
                break;
            case 0xC:
                str_val += "K";
                break;
            case 0xD:
                str_val += "D";
                break;
            case 0xE:
                str_val += "B";
                break;
            case 0xF:
                str_val += "N";
                break;
                
            default:
                str_val += "?";
        }
    }
    
    return str_val;
}


std::vector<unsigned int> CodonState::getTripletStates( void ) const
{
    
    
    std::vector<unsigned int> codon_pos = std::vector<unsigned int>(3,0);
    codon_pos[0] = state & 0x00F;
    codon_pos[1] = state & 0x0F0;
    codon_pos[2] = state & 0xF00;
    
    return codon_pos;
}


bool CodonState::isStopCodon( void ) const 
{
    
    unsigned int codon_pos_0 = state & 0x00F;
    unsigned int codon_pos_1 = state & 0x0F0;
    unsigned int codon_pos_2 = state & 0xF00;
    
    
    if ( (codon_pos_2 & 0x800) == 0x800 ) // test if the first codon position is an 'T'
    {
        
        // test if the second codon position is an 'A'
        if ( (codon_pos_1 & 0x010) == 0x010 )
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 || (codon_pos_0 & 0x004) == 0x004 )
            {
                // we have a stopp codon
                return true;
            }
            
        }
        else if ( (codon_pos_1 & 0x040) == 0x040 ) // test if the second codon position is an 'G'
        {
            
            // test if the third codon position is an 'A' or a 'G'
            if ( (codon_pos_0 & 0x001) == 0x001 )
            {
                // we have a stopp codon
                return true;
            }
            
        }
        
    }
    
    
    return false;
}


void CodonState::setState(const std::string &symbol)
{
    state = computeState( symbol );
    
    unsigned int codon_pos_0 = state & 0x00F;
    unsigned int codon_pos_1 = state & 0x0F0;
    unsigned int codon_pos_2 = state & 0xF00;
    
    stateIndex = 0;
    switch ( codon_pos_0 )
    {
        case 0x001: { stateIndex = 0; break; }
        case 0x002: { stateIndex = 1; break; }
        case 0x004: { stateIndex = 2; break; }
        case 0x008: { stateIndex = 3; break; }
        default: stateIndex = -1;
    }
    switch ( codon_pos_1 )
    {
        case 0x010: { stateIndex += 0; break; }
        case 0x020: { stateIndex += 4; break; }
        case 0x040: { stateIndex += 8; break; }
        case 0x080: { stateIndex += 12; break; }
        default: stateIndex = -1;
    }
    switch ( codon_pos_2 )
    {
        case 0x100: { stateIndex += 0; break; }
        case 0x200: { stateIndex += 16; break; }
        case 0x400: { stateIndex += 32; break; }
        case 0x800: { stateIndex += 48; break; }
        default: stateIndex = -1;
    }
}



void CodonState::setStateByIndex(size_t index)
{
    
    stateIndex = index;
}


void CodonState::setToFirstState( void )
{
    state = 0x1;
    stateIndex = 0;
}


